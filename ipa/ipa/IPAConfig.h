#pragma once
#include "bandersnatch/Fr.h"
#include "bandersnatch/Element.h"

using verkle::bandersnatch::Fr;
using verkle::bandersnatch::Element;

namespace verkle::ipa
{
struct IPAConfig
{
    using Ptr = std::shared_ptr<IPAConfig>;
    Element::ElementListPtr m_srs;
    Element m_Q;
    uint32_t rounds;

    // TODO: precomputed points for acceleration

    Element commitToPoly(const Fr::FrListPtr& scalars);

    static Fr::FrListPtr computeBVector(Fr const& evalPoint);

    static const Fr maxEvalPoinInsideDomain;
};

inline const Fr IPAConfig::maxEvalPoinInsideDomain = Fr::fromUint64(255);

}