//
// Created by Zhengxuan Guo on 2024/8/15.
//
#include "bandersnatch/Fr.h"
#include "testutils/TestPromptFixture.h"
#include "Constants.h"
#include <boost/test/unit_test.hpp>

namespace verkle::test
{
BOOST_FIXTURE_TEST_SUITE(FrTest, TestPromptFixture)

BOOST_AUTO_TEST_CASE(testFromBytes)
{
    byte a[32] = {
        0x6e, 0xf7, 0xfe, 0x28, 0xfa, 0xc6, 0x6c, 0x7d, 
        0xe1, 0x63, 0x15, 0xcd, 0x78, 0x42, 0x8c, 0x61, 
        0x8a, 0xdd, 0x2d, 0x50, 0x08, 0x7b, 0xda, 0x61, 
        0xe2, 0xf7, 0x02, 0xa0, 0x3c, 0xb8, 0xf1, 0x3e
    };
    bandersnatch::Fr t1(a, 32);
    bandersnatch::Fr t2(a, 32);
    BOOST_ASSERT(t1 == t2);
}

BOOST_AUTO_TEST_CASE(testZero)
{
    auto zero = bandersnatch::Fr::zero();
    auto fr1 = FixedFr1();
    auto fr1SumZero = fr1 + zero;
    BOOST_ASSERT(fr1SumZero == fr1);
}

BOOST_AUTO_TEST_CASE(testRandom)
{
    auto r1 = bandersnatch::Fr::random();
    auto r2 = bandersnatch::Fr::random();
    BOOST_ASSERT(r1 != r2);
}

BOOST_AUTO_TEST_CASE(testAdd)
{
    uint64_t raw[4] = {
        0xbe6bcfbe3a401dbc,
        0x7fb2587219c6e720,
        0xe08095a173a7a164,
        0x1cb905bdfcf761a0,
    };
    bandersnatch::Fr exp(raw);
    auto f1 = FixedFr1();
    auto f2 = FixedFr2();

    auto res1 = f1 + f2;
    BOOST_ASSERT(res1 == exp);

    bandersnatch::Fr res2(f1);
    res2 += f2;
    BOOST_ASSERT(res2 == exp);
}

BOOST_AUTO_TEST_CASE(testSub)
{
    uint64_t raw[4] = 
    {
        0xd4306db17f12686,
        0x8427bea7f896a06d,
        0xc1c92d1ccd02534a,
        0x41294122643035d6,
    };
    bandersnatch::Fr exp(raw);
    auto f1 = FixedFr1();
    auto f2 = FixedFr2();
    
    auto res1 = f1 - f2;
    BOOST_ASSERT(res1 == exp);

    bandersnatch::Fr res2(f1);
    res2 -= f2;
    BOOST_ASSERT(res2 == exp);
}

BOOST_AUTO_TEST_CASE(testMult)
{
    uint64_t raw[4] = {
        0xe9f26c96d15bb807,
        0x6ad84e7137609152,
        0xe193b5e1ce7d01c4,
        0x27625b6a622c0518,
    };
    bandersnatch::Fr exp(raw);
    auto f1 = FixedFr1();
    auto f2 = FixedFr2();
    
    auto res1 = f1 * f2;
    BOOST_ASSERT(res1 == exp);

    bandersnatch::Fr res2(f1);
    res2 *= f2;
    BOOST_ASSERT(res2 == exp);
}

BOOST_AUTO_TEST_CASE(testInv)
{
    uint64_t raw[4] = {
        0xc2eecde5714050d6,
        0x19f77495338244b4,
        0x65208d07b3ef94b1,
        0x524a236a985c691d,
    };
    bandersnatch::Fr exp(raw);
    auto f1 = FixedFr1();
    auto res = f1.inv();
    BOOST_ASSERT(res == exp);
}

BOOST_AUTO_TEST_CASE(testEqual)
{
    uint64_t a[4] = {
        0xc2eecde5714050d6,
        0x19f77495338244b4,
        0x65208d07b3ef94b1,
        0x524a236a985c691d,
    };
    bandersnatch::Fr t1(a);
    bandersnatch::Fr t2(a);
    BOOST_ASSERT(t1 == t2);
}

BOOST_AUTO_TEST_CASE(testSerialize)
{
    auto fr1 = FixedFr1();
    uint64_t a[4];
    fr1.serialize(a);
    bandersnatch::Fr fr1Copy(a);
    BOOST_ASSERT(fr1Copy == fr1);
}

BOOST_AUTO_TEST_SUITE_END()
}