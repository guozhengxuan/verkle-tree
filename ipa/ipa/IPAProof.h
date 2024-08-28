#pragma once
#include "bandersnatch/Fr.h"
#include "bandersnatch/Element.h"
#include "Transcript.h"
#include "IPAConfig.h"

using verkle::bandersnatch::Fr;
using verkle::bandersnatch::Element;

namespace verkle::ipa
{
struct  IPAProof
{
    Element::ElementListPtr m_left;
    Element::ElementListPtr m_right;
    Fr m_a;

    static IPAProof create(
        Transcript::Ptr transcript, 
        IPAConfig::Ptr config, 
        Element const& commitment, 
        Fr::FrListPtr& a,
        Fr const& evalPoint
    );
    bool check(
        Transcript::Ptr transcript, 
        IPAConfig::Ptr config, 
        Element& commitment,
        Fr const& evalPoint,
        Fr const& result
    ) const;
};
}