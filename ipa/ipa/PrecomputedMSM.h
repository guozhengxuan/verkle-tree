//
// Created by Zhengxuan Guo on 2024/8/30.
//
#pragma once

#include "bandersnatch/Element.h"
#include "bandersnatch/Fr.h"
#include "bandersnatch/PrecomputedElements.h"

using verkle::bandersnatch::PrecomputedElements;
using verkle::bandersnatch::Element;
using verkle::bandersnatch::Fr;

namespace verkle::ipa
{
    // MSMPrecomp is an engine to calculate 256-MSM on a fixed basis using precomputed tables.
    // This precomputed tables design are biased to support an efficient MSM for Verkle Trees.
    //
    // Their design involves 16-bit windows for the first window16vs8IndexLimit points, and 8-bit
    // windows for the rest. The motivation for this is that the first points are used to calculate
    // tree keys, which clients heavily rely on compared to "longer" MSMs. This provides a significant
    // boost to tree-key generation without exploding table sizes.
    class PrecomputedMSM {
    public:
        using Ptr = std::shared_ptr<PrecomputedMSM>;
        explicit PrecomputedMSM(Element::ElementListPtr const& points);
        Element msm(Fr::FrListPtr const& scalars);

    private:
        PrecomputedElements::Ptr m_heavy;
        PrecomputedElements::Ptr m_light;
    };
}
