#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include "imgui_internal.h"
#include "ExecutionTimelineSequencer.h"

const char* ExecutionTimelineSequencer::GetItemTypeName(int typeIndex) const{
    switch (typeIndex) {
        case 0: return "UC";
        case 1: return "ES";
        case 2: return "W";
        default: return "";
    }
}

const char* ExecutionTimelineSequencer::GetItemLabel(int index) const{
    if (index < 0 || index >= (int)mRows.size()) return "";
    return mRows[index].name.c_str();
}

unsigned int ExecutionTimelineSequencer::ColorFor(OperationProcessus op){
    switch (op) {
        case UC: return IM_COL32( 80, 200, 120, 255); // vert
        case ES: return IM_COL32( 80, 140, 240, 255); // bleu
        case W:  return IM_COL32(220, 170,  70, 255); // orange
        default: return IM_COL32(200, 200, 200, 255);
    }
}

void ExecutionTimelineSequencer::SetTimeline(const ExecutionTimeline* timeline){
    mRows.clear();
    mFrameMin = 0;
    mFrameMax = 1;

    mCurrentFrame = 0;
    mSelectedEntry = -1;
    mFirstFrame = 0;

    // par défaut: pas de reveal (tout afficher)
    mRevealFrame = -1;

    if (!timeline || !timeline->processus)return;

    int globalMaxEnd = 1;

    // Lignes par processus
    for (Liste procCell = teteLTQ(timeline->processus);procCell != nullptr;procCell = suivantListe(procCell)){
        Processus* p = (Processus*)procCell->data;
        if (!p) continue;

        Row row;
        row.name = p->name;
        int t = p->timeArrival;

        for (Liste qCell = teteLTQ(p->listeTQ);qCell != nullptr;qCell = suivantListe(qCell)){
            Quantum* q = (Quantum*)qCell->data;
            if (!q || q->nbQuantum <= 0) continue;

            // Quantum: type + nbQuantum (durée)
            Segment seg;
            seg.start = t;
            seg.end   = t + q->nbQuantum;
            seg.op    = q->type;

            row.segments.push_back(seg);

            t = seg.end;
            globalMaxEnd = std::max(globalMaxEnd, seg.end);
        }

        mRows.push_back(std::move(row));
    }

    // Frame max globale
    mFrameMax = std::max(1, globalMaxEnd);

    // Ligne TOTAL UC : union des intervalles UC
    std::vector<std::pair<int,int>> intervals;
    intervals.reserve(256);

    for (const auto& r : mRows) {
        for (const auto& s : r.segments) {
            if (s.op == UC && s.end > s.start) {
                intervals.emplace_back(s.start, s.end);
            }
        }
    }

    std::sort(intervals.begin(), intervals.end(),[](const auto& a, const auto& b){ return a.first < b.first; });

    std::vector<std::pair<int,int>> merged;
    merged.reserve(intervals.size());

    for (const auto& it : intervals){
        if (merged.empty() || it.first > merged.back().second)merged.push_back(it);
        else merged.back().second = std::max(merged.back().second, it.second);
    }

    int busyUC = 0;
    for (const auto& m : merged)busyUC += (m.second - m.first);

    const int totalTime = std::max(1, (mFrameMax - mFrameMin));
    const float percent = (100.0f * (float)busyUC) / (float)totalTime;

    Row total;
    total.isTotalUC = true;
    total.name = "TOTAL UC (CPU) : " + std::to_string(std::round(percent)) + "%";

    for (const auto& m : merged) {
        Segment seg;
        seg.start = m.first;
        seg.end = m.second;
        seg.op = UC;
        total.segments.push_back(seg);
    }

    mRows.push_back(std::move(total));

    for (auto& r : mRows) {
        r.slotStart = mFrameMin;
        r.slotEnd   = mFrameMax;
    }

}

void ExecutionTimelineSequencer::Get(int index, int** start, int** end, int* type, unsigned int* color){
    if (index < 0 || index >= (int)mRows.size())return;

    Row& row = mRows[index];

    if (start) *start = &row.slotStart;
    if (end) *end = &row.slotEnd;
    if (type) *type = 0;
    if (color) *color = 0xFF242424;
}

bool ExecutionTimelineSequencer::Draw(int sequenceOptions){
    if (sequenceOptions == ImSequencer::SEQUENCER_EDIT_NONE)sequenceOptions = ImSequencer::SEQUENCER_CHANGE_FRAME;

    return ImSequencer::Sequencer(
        this,
        &mCurrentFrame,
        nullptr,
        &mSelectedEntry,
        &mFirstFrame,
        sequenceOptions
    );
}

void ExecutionTimelineSequencer::CustomDrawCompact(int index,ImDrawList* draw_list,const ImRect& rc,const ImRect& clippingRect){
    if (!draw_list) return;
    if (index < 0 || index >= (int)mRows.size()) return;

    const Row& row = mRows[index];

    draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);

    const float denom = float((mFrameMax - mFrameMin) + 2);
    const float pxPerFrame = (denom > 0.0f) ? (rc.GetWidth() / denom) : 1.0f;

    // Épaisseur un peu augmentée
    const float padY = 0.8f;
    const float y1 = rc.Min.y + padY;
    const float y2 = rc.Max.y - padY;

    for (const auto& seg : row.segments){
        int drawStart = seg.start;
        int drawEnd   = seg.end;

        // --- Reveal (afficher jusqu'à mRevealFrame) ---
        if (mRevealFrame >= 0) {
            if (drawStart >= mRevealFrame)continue;
            if (drawEnd > mRevealFrame)drawEnd = mRevealFrame;
        }

        if (drawEnd <= drawStart)continue;

        const float x1 = rc.Min.x + (float(drawStart - mFrameMin) + 0.5f) * pxPerFrame;
        const float x2 = rc.Min.x + (float(drawEnd   - mFrameMin) + 0.5f) * pxPerFrame;

        if (x2 < clippingRect.Min.x || x1 > clippingRect.Max.x)continue;

        const ImU32 col = ColorFor(seg.op);

        // Segment
        draw_list->AddRectFilled(ImVec2(x1, y1), ImVec2(x2, y2), col, 2.0f);
        draw_list->AddRect(ImVec2(x1, y1), ImVec2(x2, y2), IM_COL32(0, 0, 0, 120), 2.0f, 0, 1.0f);

        // Séparations "tick" (uniquement sur la partie révélée)
        if (mDrawUnitSeparators && drawEnd - drawStart > 1){
            int step = 1;
            if (pxPerFrame < mSeparatorMinPx) {
                step = (int)std::ceil(mSeparatorMinPx / pxPerFrame);
                if (step < 1) step = 1;
            }

            for (int f = drawStart + step; f < drawEnd; f += step){
                const float x = rc.Min.x + (float(f - mFrameMin) + 0.5f) * pxPerFrame;
                if (x < clippingRect.Min.x || x > clippingRect.Max.x)continue;
                draw_list->AddLine(ImVec2(x, y1), ImVec2(x, y2),IM_COL32_BLACK, 1.0f);
            }
        }
    }
    draw_list->PopClipRect();
}