#pragma once

#include <string>
#include <vector>
#include <cstddef>
#include <algorithm>
#include <cmath>
#include "ImSequencer.h"

struct ImDrawList;
struct ImRect;

extern "C" {
    #include "data/struct/ExecutionTimeline.h"
    #include "data/struct/liste_tq.h"
    #include "data/struct/liste.h"
    #include "data/struct/processus.h"
}

class ExecutionTimelineSequencer : public ImSequencer::SequenceInterface
{
public:
    ExecutionTimelineSequencer() = default;

    // Recharge les données à partir de la timeline C
    void SetTimeline(const ExecutionTimeline* timeline);

    // Dessine le widget
    bool Draw(int sequenceOptions = ImSequencer::SEQUENCER_EDIT_NONE);

    // n = frame jusqu'où afficher (exclu): tout ce qui commence >= n est caché, tout ce qui dépasse est coupé à n
    void SetRevealFrame(int n) { mRevealFrame = n; }
    void ClearRevealFrame() { mRevealFrame = -1; } // -1 => tout afficher
    bool DrawUntil(int sequenceOptions, int n) { SetRevealFrame(n); return Draw(sequenceOptions); }

    // Contrôle lecture
    void SetCurrentFrame(int frame) { mCurrentFrame = frame; }
    int  GetCurrentFrame() const { return mCurrentFrame; }

    // Séparations par tick
    void SetDrawUnitSeparators(bool v) { mDrawUnitSeparators = v; }
    void SetSeparatorMinPx(float v) { mSeparatorMinPx = v; }

    // ---------- ImSequencer::SequenceInterface ----------
    int GetFrameMin() const override { return mFrameMin; }
    int GetFrameMax() const override { return mFrameMax; }
    int GetItemCount() const override { return (int)mRows.size(); }

    int GetItemTypeCount() const override { return 3; }
    const char* GetItemTypeName(int typeIndex) const override;
    const char* GetItemLabel(int index) const override;

    void Get(int index, int** start, int** end, int* type, unsigned int* color) override;

    void CustomDrawCompact(int index,ImDrawList* draw_list,const ImRect& rc,const ImRect& clippingRect) override;

    // Pas d’édition
    void Add(int) override {}
    void Del(int) override {}
    void Duplicate(int) override {}
    void Copy() override {}
    void Paste() override {}

private:
    struct Segment {
        int start = 0;
        int end   = 0;
        OperationProcessus op = UC;
    };

    struct Row {
        std::string name;
        int slotStart = 0;
        int slotEnd   = 0;
        std::vector<Segment> segments;
        bool isTotalUC = false;
    };

    static unsigned int ColorFor(OperationProcessus op);
    std::vector<Row> mRows;

    int  mFrameMin = 0;
    int  mFrameMax = 1;

    int  mCurrentFrame  = 0;
    int  mSelectedEntry = -1;
    int  mFirstFrame    = 0;

    // Reveal (animation): -1 => tout afficher
    int mRevealFrame = -1;

    // Séparations
    bool  mDrawUnitSeparators = true;
    float mSeparatorMinPx = 4.0f;
};