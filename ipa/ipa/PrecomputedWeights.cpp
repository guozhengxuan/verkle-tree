//
// Created by Zhengxuan Guo on 2024/8/28.
//

#include "PrecomputedWeights.h"

using namespace verkle::ipa;

// Imagine we have two arrays of the same length and we concatenate them together
// This is how we will store the A'(x_i) and 1/A'(x_i)
// This midpoint variable is used to compute the offset that we need
// to place 1/A'(x_i)
PrecomputedWeights::PrecomputedWeights()
{
    auto mid = domainSize;

    // Note there are DOMAIN_SIZE number of weights, but we are also storing their inverses
    // so we need double the amount of space
    auto barycentricWeights = std::make_shared<std::vector<Fr>>(mid*2);
    for (auto i = 0; i < mid; ++i)
    {
        auto weight = computeBarycentricWeightForElement(i);
        auto invWeight = weight.inv();

        barycentricWeights->at(i) = weight;
        barycentricWeights->at(i+mid) = invWeight;
    }

    // Computing 1/k and -1/k for k \in [0, 255]
    // Note that since we cannot do 1/0, we have one less element
    mid = domainSize - 1;
    auto invertedDomain = std::make_shared<std::vector<Fr>>(mid*2);
    for (auto i = 1; i < domainSize; ++i)
    {
        auto k = Fr::fromUint64(i).inv();
        auto kNeg = Fr::zero() - k;

        invertedDomain->at(i-1) = k;
        invertedDomain->at(i-1+mid) = kNeg;
    }

    m_barycentricWeights = barycentricWeights;
    m_invertedDomain = invertedDomain;
}

// computes A'(x_j) where x_j must be an element in the domain
// This is computed as the product of x_j - x_i where x_i is an element in the domain
// and x_i is not equal to x_j
Fr PrecomputedWeights::computeBarycentricWeightForElement(uint64_t element)
{
    if (element > domainSize)
    {
        throw std::runtime_error("the domain size is [0, 255], " +
            std::to_string(element) + " is not in the domain");
    }

    auto domainEle = Fr::fromUint64(element);
    auto total = Fr::one();

    for (auto i = 0; i < domainSize; ++i)
    {
        if (i == element)
        {
            continue;
        }

        total *= domainEle - Fr::fromUint64(i);
    }

    return total;
}

Fr PrecomputedWeights::getInversedBarycentricWeight(size_t index) const
{
    auto const mid = m_barycentricWeights->size() / 2;
    return m_barycentricWeights->at(index+mid);
}

// ComputeBarycentricCoefficients, computes the coefficients `bary_coeffs`
// for a point `z` such that when we have a polynomial `p` in lagrange
// basis, the inner product of `p` and `bary_coeffs` is equal to p(z)
// Note that `z` should not be in the domain.
// This can also be seen as the lagrange coefficients L_i(point)
Fr::FrListPtr PrecomputedWeights::computeBarycentricCoefficients(const Fr& point) const
{
    auto lagrangeEvals =  std::make_shared<std::vector<Fr>>(domainSize);
    for (auto i = 0; i < domainSize; ++i)
    {
        auto weight = m_barycentricWeights->at(i);
        auto fr_i = Fr::fromUint64(i);

        lagrangeEvals->at(i) = (point - fr_i) * weight;
    }

    auto totalProd = Fr::one();
    for (auto i = 0; i < domainSize; ++i)
    {
        auto fr_i = Fr::fromUint64(i);
        totalProd *= point - fr_i;
    }

    for (auto& scalar: *lagrangeEvals)
    {
        scalar = totalProd * scalar.inv();
    }

    return lagrangeEvals;
}
