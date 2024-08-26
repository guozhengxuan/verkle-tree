#include "Transcript.h"

using namespace verkle::ipa;

Transcript::Transcript(SeperateLabel label)
{
    auto lv = getLabelValue(label);
    m_state.update(lv, strlen(lv));
}

const char* Transcript::getLabelValue(const SeperateLabel& label)
{
    return labelValues.at(label);
}

void Transcript::appendLabel(SeperateLabel label)
{
    m_buffer << getLabelValue(label);
}

void Transcript::appendScalar(const bandersnatch::Fr& scalar, SeperateLabel label)
{
    uint64_t out[4];
    scalar.serialize(out);
    appendLabel(label);
    for (size_t i = 0; i < 4; ++i)
    {
        m_buffer << out[i];
    }
}

void Transcript::appendPoint(const bandersnatch::Element& point, SeperateLabel label)
{
    byte out[96];
    point.serialize(out);
    appendLabel(label);
    for (size_t i = 0; i < 96; ++i)
    {
        m_buffer << out[i];
    }
}

verkle::bandersnatch::Fr Transcript::generateChallenge(SeperateLabel label)
{
    // fetch buffer and reset it
    appendLabel(label);
    auto str = m_buffer.str();
    auto combined = str.c_str();
    m_buffer.str("");
    m_buffer.clear();

    // use buffer hash to generate a challenge
    unsigned char hash[32];
    m_state.update(combined, strlen(combined));
    m_state.finalize(hash);
    bandersnatch::Fr ret(hash, sizeof(hash));

    // add the new challenge to the state
    // which "summarises" the previous state before we cleared it
    appendScalar(ret, label);

    return ret;
}