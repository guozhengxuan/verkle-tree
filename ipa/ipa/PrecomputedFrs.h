//
// Created by Zhengxuan Guo on 2024/8/28.
//
#pragma once
#include "bandersnatch/Fr.h"
#include "common/Common.h"

using verkle::bandersnatch::Fr;

namespace verkle::ipa
{
    constexpr auto domainSize = common::vectorLength;

    // PrecomputedWeights contains precomputed coefficients for efficient
    // usage of the Barycentric formula.
    class PrecomputedFrs {
    public:
        PrecomputedFrs();

        [[nodiscard]] Fr::FrListPtr computeBarycentricCoefficients(const Fr& point) const;

        static Fr computeBarycentricWeightForElement(uint64_t element);

    private:
        // This stores A'(x_i) and 1/A'(x_i)
        Fr::FrListPtr m_barycentricWeights;

        // This stores 1/k and -1/k for k \in [0, 255]
        Fr::FrListPtr m_invertedDomain;
    };
}