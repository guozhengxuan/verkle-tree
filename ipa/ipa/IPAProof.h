#pragma once
#include "bandersnatch/Fr.h"
#include "bandersnatch/Element.h"
#include "Transcript.h"
#include "IPAConfig.h"

using verkle::bandersnatch::Fr;
using verkle::bandersnatch::Element;

namespace verkle::ipa
{
class IPAProof
{
private:
    std::vector<Element> m_left;
    std::vector<Element> m_right;
    bandersnatch::Fr m_a;
public:
    static IPAProof create(
        Transcript::Ptr transcript, 
        IPAConfig::Ptr config, 
        Element const& commitment, 
        Fr::FrListPtr const& a,
        Fr const& evalPoint
    );
    bool check(
        Transcript::Ptr transcript, 
        IPAConfig::Ptr config, 
        Element const& commitment, 
        Fr const& evalPoint,
        Fr const& result
    );
};
}