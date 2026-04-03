#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

#include "ImSequencer.h"

extern "C" {
#include "data/struct/ExecutionTimeline.h"
#include "data/struct/processus.h"
}

class ExecutionTimelineSequencer : public ImSequencer::SequenceInterface
{
public:
    ExecutionTimelineSequencer() = default;

    // Reconstruit les données internes depuis ta timeline C (copie des infos nécessaires)
    void SetTimeline(const ExecutionTimeline* timeline);

    // Appel pratique : dessine le sequencer
    // À appeler à l’intérieur d’un ImGui::Begin(...) / End(...)
    bool Draw(int sequenceOptions = ImSequencer::SEQUENCER_EDIT_NONE);

    // Contrôles lecture (optionnel)
    void SetCurrentFrame(int frame) { mCurrentFrame = frame; }
    int  GetCurrentFrame() const { return mCurrentFrame; }

    // -------- ImSequencer::SequenceInterface --------
    int GetFrameMin() const override { return mFrameMin; }
    int GetFrameMax() const override { return mFrameMax; }
    int GetItemCount() const override { return (int)mRows.size(); }

    int GetItemTypeCount() const override { return 3; }
    const char* GetItemTypeName(int typeIndex) const override;
    const char* GetItemLabel(int index) const override;

    void Get(int index, int** start, int** end, int* type, unsigned int* color) override;

    // On désactive l’édition : pas de Add/Del/Duplicate/Copy/Paste
    void Add(int) override {}
    void Del(int) override {}
    void Duplicate(int) override {}
    void Copy() override {}
    void Paste() override {}

    // Dessin multi-segments sur une seule ligne
    void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect) override;

private:
    struct Segment {
        int start = 0; // frame de début
        int end   = 0; // frame de fin (exclusive)
        OperationProcessus op = UC;
    };

    struct Row {
        std::string name;
        int slotStart = 0; // utilisé par ImSequencer pour le "slot" de fond
        int slotEnd   = 0;
        std::vector<Segment> segments;
    };

    static unsigned int ColorFor(OperationProcessus op);

private:
    std::vector<Row> mRows;

    int  mFrameMin = 0;
    int  mFrameMax = 1; // doit être >= 1
    int  mCurrentFrame = 0;
    int  mSelectedEntry = -1;
    int  mFirstFrame = 0;  // scroll horizontal dans ImSequencer
    bool mExpanded = true;
};