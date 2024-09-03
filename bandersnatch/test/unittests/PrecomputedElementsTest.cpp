//
// Created by Zhengxuan Guo on 2024/8/30.
//
#include <testutils/TestPromptFixture.h>
#include <boost/test/unit_test.hpp>

#include "bandersnatch/Fr.h"
#include "bandersnatch/Element.h"
#include "bandersnatch/PrecomputedElements.h"

using verkle::bandersnatch::Fr;
using verkle::bandersnatch::Element;
using verkle::bandersnatch::PrecomputedElements;

namespace verkle::ipa::test
{
    BOOST_FIXTURE_TEST_SUITE(ElementTest, verkle::test::TestPromptFixture)

    BOOST_AUTO_TEST_CASE(testPrecomputedMSM)
    {
        auto scalars = std::make_shared<std::vector<Fr>>(256);
        auto points = std::make_shared<std::vector<Element>>(256);
        auto exp = Element::zero();
        for (size_t i = 0; i < 256; ++i)
        {
            auto randomFr = Fr::random();
            auto randomPoint = Element::generator().mult(randomFr);

            // naive sum of multiple multiplication
            exp.add(Element::mult(randomFr, randomPoint));

            scalars->at(i) = randomFr;
            points->at(i) = randomPoint;
        }

        auto const precomputed = PrecomputedElements(points, 8);
        auto const res = precomputed.msm(scalars);
        BOOST_ASSERT(res == exp);
    }

    BOOST_AUTO_TEST_SUITE_END()
}