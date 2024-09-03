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
public:
    static IPAProof create(
        Transcript::Ptr& transcript,
        const IPAConfig& config,
        Element const& commitment, 
        Fr::FrListPtr a,
        Fr const& evalPoint
    );
    bool check(
        Transcript::Ptr& transcript,
        const IPAConfig& config,
        Element const& commitment,
        Fr const& evalPoint,
        Fr const& result
    ) const;

private:
    Element::ElementListPtr m_left;
    Element::ElementListPtr m_right;
    Fr m_a;

    [[nodiscard]] Fr::FrListPtr generateChallenges(Transcript::Ptr const& transcript) const;
};
}