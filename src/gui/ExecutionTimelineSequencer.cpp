#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"

#include "ExecutionTimelineSequencer.h"


const char* ExecutionTimelineSequencer::GetItemTypeName(int typeIndex) const
{
    switch (typeIndex) {
        case 0: return "UC";
        case 1: return "ES";
        case 2: return "W";
        default: return "";
    }
}

const char* ExecutionTimelineSequencer::GetItemLabel(int index) const
{
    if (index < 0 || index >= (int)mRows.size()) return "";
    return mRows[index].name.c_str();
}

unsigned int ExecutionTimelineSequencer::ColorFor(OperationProcessus op)
{
    // Format ImU32: 0xAABBGGRR (ImGui uses little endian packing in IM_COL32)
    // Ici on met des couleurs lisibles :
    switch (op) {
        case UC: return IM_COL32( 80, 200, 120, 255); // vert
        case ES: return IM_COL32( 80, 140, 240, 255); // bleu
        case W:  return IM_COL32(220, 170,  70, 255); // orange
        default: return IM_COL32(200, 200, 200, 255);
    }
}

void ExecutionTimelineSequencer::SetTimeline(const ExecutionTimeline* timeline)
{
    mRows.clear();
    mFrameMin = 0;
    mFrameMax = 1;
    mCurrentFrame = 0;
    mSelectedEntry = -1;
    mFirstFrame = 0;
    mExpanded = true;

    if (!timeline || !timeline->processus)
        return;

    // Parcours de timeline->processus : ListeTQ de Processus*
    Liste procCell = teteLTQ(timeline->processus);

    int globalMaxEnd = 1;

    while (procCell != nullptr)
    {
        Processus* p = (Processus*)procCell->data;
        if (!p) {
            procCell = suivantListe(procCell);
            continue;
        }

        Row row;
        row.name = p->name;

        // On reconstruit les segments en cumulant depuis timeArrival
        int t = p->timeArrival;

        // listeTQ du processus : ListeTQ de Quantum*
        Liste qCell = teteLTQ(p->listeTQ);
        while (qCell != nullptr)
        {
            Quantum* q = (Quantum*)qCell->data;
            if (q && q->nbQuantum > 0)
            {
                Segment seg;
                seg.start = t;
                seg.end   = t + q->nbQuantum;
                seg.op    = q->type;

                row.segments.push_back(seg);
                t = seg.end;

                globalMaxEnd = std::max(globalMaxEnd, seg.end);
            }
            qCell = suivantListe(qCell);
        }

        // On met un slot de fond qui couvre toute la timeline,
        // couleur fond = même couleur que background ImSequencer pour "disparaître".
        row.slotStart = mFrameMin;
        row.slotEnd   = globalMaxEnd;

        mRows.push_back(std::move(row));

        procCell = suivantListe(procCell);
    }

    // Frame max globale
    mFrameMax = std::max(1, globalMaxEnd);

    // Mise à jour slotEnd de toutes les rows (pour couvrir la frame max finale)
    for (auto& r : mRows) {
        r.slotStart = mFrameMin;
        r.slotEnd   = mFrameMax;
    }
}

void ExecutionTimelineSequencer::Get(int index, int** start, int** end, int* type, unsigned int* color)
{
    if (index < 0 || index >= (int)mRows.size())
        return;

    Row& row = mRows[index];

    if (start) *start = &row.slotStart;
    if (end)   *end   = &row.slotEnd;

    // type quelconque (pas important ici)
    if (type)  *type = 0;

    // Couleur du slot de fond : proche du background (pour ne pas gêner),
    // on va dessiner nos segments par-dessus dans CustomDrawCompact.
    // (ImSequencer force l'alpha à 0xFF via un OR interne, donc on donne un RGB fond)
    if (color) *color = 0xFF242424;
}

bool ExecutionTimelineSequencer::Draw(int sequenceOptions)
{
    if (sequenceOptions == ImSequencer::SEQUENCER_EDIT_NONE) {
        sequenceOptions = ImSequencer::SEQUENCER_CHANGE_FRAME;
    }
    return ImSequencer::Sequencer(this, &mCurrentFrame, nullptr, &mSelectedEntry, &mFirstFrame, sequenceOptions);
}

void ExecutionTimelineSequencer::CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect)
{
    if (!draw_list) return;
    if (index < 0 || index >= (int)mRows.size()) return;

    const Row& row = mRows[index];

    // IMPORTANT: on clip pour ne pas dessiner hors zone
    draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);

    // Conversion frame -> pixel
    // D'après la construction de rc dans ImSequencer.cpp :
    // rc.Width == (frameMax - frameMin + 2) * framePixelWidth [2](https://stackoverflow.com/questions/2816715/how-to-branch-from-a-previous-commit)
    const float denom = float((mFrameMax - mFrameMin) + 2);
    const float pxPerFrame = (denom > 0.0f) ? (rc.GetWidth() / denom) : 1.0f;

    // Zone verticale de la barre dans la ligne
    const float y1 = rc.Min.y + 2.0f;
    const float y2 = rc.Max.y - 2.0f;

    // Optionnel : dessiner un fond léger (ligne)
    // draw_list->AddRectFilled(rc.Min, rc.Max, IM_COL32(36,36,36,255), 2.0f);

    for (const auto& seg : row.segments)
    {
        // x(frame) = rc.Min.x + (frame - frameMin + 0.5) * pxPerFrame  [2](https://stackoverflow.com/questions/2816715/how-to-branch-from-a-previous-commit)
        const float x1 = rc.Min.x + (float(seg.start - mFrameMin) + 0.5f) * pxPerFrame;
        const float x2 = rc.Min.x + (float(seg.end   - mFrameMin) + 0.5f) * pxPerFrame;

        // Si segment hors écran, on skip vite
        if (x2 < clippingRect.Min.x || x1 > clippingRect.Max.x)
            continue;

        const unsigned int col = ColorFor(seg.op);

        draw_list->AddRectFilled(
            ImVec2(x1, y1),
            ImVec2(x2, y2),
            col,
            2.0f
        );
    }

    draw_list->PopClipRect();
}