#pragma once
#include "bandersnatch/Fr.h"
#include "bandersnatch/Element.h"
#include "verkleutils/Hash.h"
#include <sstream>
#include <string>
#include <unordered_map>

namespace verkle::ipa
{
enum SeperateLabel
{
    LABEL_IPA,
    LABEL_COMMITMENT,
    LABEL_INPUT_POINT,
    LABEL_OUTPUT_POINT,
    LABEL_RESCALING,
    LABEL_LEFT,
    LABEL_RIGHT,
    LABEL_X,
};

// The transcript is for challenge scalars generation.
class Transcript
{
private:
    std::stringstream m_buffer;
    utils::Hash m_state;
    static const std::unordered_map<SeperateLabel, const char*> labelValues;
    static const char* getLabelValue(const SeperateLabel& label);

public:
    using Ptr = std::shared_ptr<Transcript>;
    Transcript(SeperateLabel label);
    void appendLabel(SeperateLabel label);
    void appendScalar(const bandersnatch::Fr& scalar, SeperateLabel label);
    void appendPoint(const bandersnatch::Element& point, SeperateLabel label);

    bandersnatch::Fr generateChallenge(SeperateLabel label);
};

inline const std::unordered_map<SeperateLabel, const char*> Transcript::labelValues = {
    {LABEL_IPA, "ipa"},
    {LABEL_COMMITMENT, "commitment"},
    {LABEL_INPUT_POINT, "input point"},
    {LABEL_OUTPUT_POINT, "output point"},
    {LABEL_RESCALING, "rescaling"},
    {LABEL_LEFT, "left"},
    {LABEL_RIGHT, "right"},
    {LABEL_X, "x"}
};

}