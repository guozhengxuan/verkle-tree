//
// Created by Zhengxuan Guo on 2024/8/15.
//
#include "bandersnatch/Fr.h"
#include "utility/testutils/TestPromptFixture.h"
#include <boost/test/unit_test.hpp>

namespace verkle::test
{
BOOST_FIXTURE_TEST_SUITE(FrTest, TestPromptFixture)
static inline bandersnatch::Fr FixedFr1()
{
    static bandersnatch::Fr fr1(
        (uint64_t[4]){
            0x2ef123703093cbbb,
            0x9876543210fedcba,
            0,
            0,
        }
    );
    return fr1;
}


BOOST_AUTO_TEST_CASE(testAdd)
{

}

BOOST_AUTO_TEST_SUITE_END()
}