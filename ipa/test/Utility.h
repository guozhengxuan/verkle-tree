//
// Created by Zhengxuan Guo on 2024/8/29.
//
#pragma once
#include "ipa/PrecomputedFrs.h"
#include "bandersnatch/Fr.h"

using verkle::bandersnatch::Fr;

namespace verkle::ipa::test
{
    Fr::FrListPtr testPoly256(std::vector<uint64_t> const& polynomial);
    Fr evalOutsideDomain(PrecomputedFrs const& precomputed, Fr::FrListPtr f, Fr const& point);
    Fr evalPoly(Fr::FrListPtr const& poly, Fr const& evalPoint);

    using FrPoint = std::pair<Fr, Fr>;
    struct FrPoints
    {
        explicit FrPoints(size_t size);

        std::shared_ptr<std::vector<FrPoint>> m_inner;
        [[nodiscard]] Fr::FrListPtr interpolate() const;
    };
}