#pragma once
#include "PrecomputedMSM.h"
#include "PrecomputedWeights.h"
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
    PrecomputedMSM::Ptr m_precomputed_msm;
    PrecomputedWeights::Ptr m_precomputed_weights;
    uint32_t m_rounds{};

    IPAConfig(IPAConfig const&) = delete;
    void operator=(IPAConfig const&) = delete;

    static IPAConfig const& getConfig();

    [[nodiscard]] Element commit(const Fr::FrListPtr& scalars) const;

    [[nodiscard]] Fr::FrListPtr computeBVector(Fr const& evalPoint) const;

    static const Fr maxEvalPoinInsideDomain;

private:
    IPAConfig();
};

inline const Fr IPAConfig::maxEvalPoinInsideDomain = Fr::fromUint64(255);

}