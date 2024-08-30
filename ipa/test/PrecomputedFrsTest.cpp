//
// Created by Zhengxuan Guo on 2024/8/29.
//
#include "Utility.h"
#include "testutils/TestPromptFixture.h"
#include <boost/test/unit_test.hpp>

#include "ipa/IPAUtility.h"
#include "ipa/PrecomputedWeights.h"

namespace verkle::ipa::test
{
    BOOST_FIXTURE_TEST_SUITE(PrecomputedFrsTest, verkle::test::TestPromptFixture)

    BOOST_AUTO_TEST_CASE(TestInterpolate)
    {
        auto a = FrPoint(Fr::zero(), Fr::zero());
        auto b = FrPoint(Fr::one(), Fr::one());
        auto points = FrPoints(2);
        points.m_inner->at(0) = a;
        points.m_inner->at(1) = b;

        auto poly = points.interpolate();

        auto randomFr = Fr::random();
        auto res = evalPoly(poly, randomFr);

        BOOST_ASSERT(res == randomFr);
    }

    BOOST_AUTO_TEST_CASE(TestComputeBarycentricCoefficients)
    {
        auto outsidePoint = Fr::fromUint64(3400);
        auto lagrangeFrs = testPoly256(std::vector<uint64_t>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10});

        auto precomputed = PrecomputedWeights();

        auto barCoeffs = precomputed.computeBarycentricCoefficients(outsidePoint);
        auto got = innerProduct(lagrangeFrs, barCoeffs);

        auto exp = evalOutsideDomain(precomputed, lagrangeFrs, outsidePoint);

        auto points = FrPoints(256);
        for (size_t k = 0; k < 256; k++)
        {
            points.m_inner->at(k) = FrPoint(Fr::fromUint64(k), lagrangeFrs->at(k));
        }
        auto polyCoeff = points.interpolate();
        auto exp2 = evalPoly(polyCoeff, outsidePoint);

        if (exp2 != exp)
        {
            BOOST_FAIL("problem with barycentric weights");
        }

        if (exp2 != got) {
            BOOST_FAIL("problem with inner product");
        }
    }

    BOOST_AUTO_TEST_SUITE_END()
}
