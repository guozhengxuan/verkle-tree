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
    appendLabel(label);

    uint64_t out[4];
    scalar.serialize(out);
    m_buffer.write(reinterpret_cast<const char*>(out), sizeof(out));
}

void Transcript::appendPoint(const bandersnatch::Element& point, SeperateLabel label)
{
    appendLabel(label);
    
    byte out[96];
    point.serialize(out);
    m_buffer.write(reinterpret_cast<const char*>(out), sizeof(out));
}

verkle::bandersnatch::Fr Transcript::generateChallenge(SeperateLabel label)
{
    appendLabel(label);

    // fetch buffer and reset it
    auto len = m_buffer.rdbuf()->in_avail();
    byte* combined = new byte[len];
    m_buffer.read(reinterpret_cast<char*>(combined), len);
    m_buffer.str("");
    m_buffer.clear();

    // use buffer hash to generate a challenge
    m_state.update(combined, len);
    delete[] combined;
    unsigned char hash[32];
    m_state.finalize(hash);
    bandersnatch::Fr ret(hash, 32*8*sizeof(unsigned char));

    // add the new challenge to the state
    // which "summarises" the previous state before we cleared it
    appendScalar(ret, label);

    return ret;
}