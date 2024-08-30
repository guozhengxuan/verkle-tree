//
// Created by Zhengxuan Guo on 2024/8/29.
//
#include "Utility.h"
#include "ipa/IPAUtility.h"
#include <boost/test/unit_test.hpp>

namespace verkle::ipa::test
{
    Fr::FrListPtr testPoly256(std::vector<uint64_t> const& polynomial)
    {
        auto n = polynomial.size();

        if (n > 256)
        {
            BOOST_ERROR("polynomial cannot exceed 256 coefficients");
        }

        auto frs = std::make_shared<std::vector<Fr>>(256);
        for (size_t i = 0; i < n; ++i)
        {
            frs->at(i) = Fr::fromUint64(polynomial[i]);
        }

        for (size_t i = n; i < 256; ++i)
        {
            frs->at(i) = Fr::zero();
        }

        return frs;
    }

    Fr evalOutsideDomain(PrecomputedFrs const& precomputed, Fr::FrListPtr f, Fr const& point)
    {
        auto pointsMinusDomain = std::vector<Fr>(verkle::ipa::domainSize);
        for (size_t i = 0; i < verkle::ipa::domainSize; ++i)
        {
            auto fr_i = Fr::fromUint64(i);
            pointsMinusDomain[i] = (point - fr_i).inv();
        }

        auto summand = Fr::zero();
        for (size_t i = 0; i < pointsMinusDomain.size(); ++i)
        {
            auto weight = precomputed.getInversedBarycentricWeight(i);
            summand += weight * f->at(i) * pointsMinusDomain[i];
        }

        auto a_z = Fr::one();
        for (size_t i = 0; i < verkle::ipa::domainSize; ++i)
        {
            a_z *= point - Fr::fromUint64(i);
        }
        a_z *= summand;

        return a_z;
    }

    Fr evalPoly(const Fr::FrListPtr& poly, const Fr& evalPoint)
    {
        auto powers = verkle::ipa::powers(evalPoint, poly->size());
        auto total = Fr::zero();
        for (size_t i = 0; i < poly->size(); ++i)
        {
            total += powers->at(i) * poly->at(i);
        }
        return total;
    }

    FrPoints::FrPoints(size_t size) : m_inner(std::make_shared<std::vector<FrPoint>>(size)) {}

    Fr::FrListPtr FrPoints::interpolate() const
    {
        auto one = Fr::one();
        auto zero = Fr::zero();

        auto degreePlusOne = m_inner->size();
        if (degreePlusOne < 2)
        {
            BOOST_ERROR("should interpolate for degree >= 1");
        }
        auto coeffs = std::make_shared<std::vector<Fr>>(degreePlusOne);

        for (size_t k = 0; k < m_inner->size(); k++)
        {
            auto [x_k, y_k] = m_inner->at(k);
            auto contribution = std::vector<Fr>(degreePlusOne);
            auto denominator = Fr::one();
            auto contributionDegree = 0;

            for (size_t j = 0; j < m_inner->size(); j++)
            {
                auto x_j = m_inner->at(j).first;

                if (j == k)
                {
                    continue;
                }

                denominator *= x_k - x_j;

                if (contributionDegree == 0)
                {
                    contributionDegree = 1;
                    contribution.at(0) -= x_j;
                    contribution.at(1) += one;
                } else
                {
                    auto mul_by_minus_x_j = std::vector<Fr>(contribution.size());
                    for (size_t i = 0; i < contribution.size(); ++i)
                    {
                        mul_by_minus_x_j[i] = zero - contribution[i] * x_j;
                    }

                    contribution.insert(contribution.begin(), zero);
                    contribution.resize(degreePlusOne);

                    if (degreePlusOne != mul_by_minus_x_j.size())
                    {
                        BOOST_ERROR("malformed mul_by_minus_x_j");
                    }

                    for (size_t i = 0; i < contribution.size(); ++i)
                    {
                        contribution[i] += mul_by_minus_x_j[i];
                    }
                }
            }

            denominator = denominator.inv();
            if (denominator == zero)
            {
                BOOST_ERROR("denominator should not be zero");
            }
            for (size_t i = 0; i < contribution.size(); ++i)
            {
                coeffs->at(i) += contribution[i] * denominator * y_k;
            }
        }

        return coeffs;
    }
}