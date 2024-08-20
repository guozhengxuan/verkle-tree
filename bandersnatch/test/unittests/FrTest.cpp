//
// Created by Zhengxuan Guo on 2024/8/15.
//
#include "bandersnatch/Fr.h"
#include "testutils/TestPromptFixture.h"
#include <boost/test/unit_test.hpp>

namespace verkle::test
{
static inline bandersnatch::Fr FixedFr1()
{
    static bandersnatch::Fr fr1(
        (uint64_t[4]){
            0xe5d76b4ca918a221,
            0x81ed0b8d092ec3c6,
            0xd124e15f2054fa57,
            0x2ef123703093cbbb,
        }
    );
    return fr1;
}

static inline bandersnatch::Fr FixedFr2()
{
    static bandersnatch::Fr fr2(
        (uint64_t[4]){
            0xd894647091277b9c,
            0x5182f0e810967f58,
            0x42958c4a5cf47f12,
            0x61b589a0f601132d,
        }
    );
    return fr2;
}

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
    bandersnatch::Fr exp(
        (uint64_t[4]){
            0xbe6bcfbe3a401dbc,
            0x7fb2587219c6e720,
            0xe08095a173a7a164,
            0x1cb905bdfcf761a0,
        }
    );
    auto f1 = FixedFr1();
    auto f2 = FixedFr2();

    auto res = f1 + f2;
    uint64_t another[4];
    res.serialize(another);
    bandersnatch::Fr newExp(another);

    auto a = f1 - f2;
    uint64_t aa[4];
    a.serialize(aa);

    auto b = f1 * f2;
    uint64_t bb[4];
    b.serialize(bb);

    auto c = f1.inv();
    uint64_t cc[4];
    c.serialize(cc);

    BOOST_ASSERT(res == exp);
}

BOOST_AUTO_TEST_CASE(testSub)
{
    bandersnatch::Fr exp(
        (uint64_t[4]){
            0xd4306db17f12686,
            0x8427bea7f896a06d,
            0xc1c92d1ccd02534a,
            0x41294122643035d6,
        }
    );
    auto f1 = FixedFr1();
    auto f2 = FixedFr2();
    auto res = f1 - f2;
    BOOST_ASSERT(res == exp);
}

BOOST_AUTO_TEST_CASE(testMult)
{
    bandersnatch::Fr exp(
        (uint64_t[4]){
            0xe9f26c96d15bb807,
            0x6ad84e7137609152,
            0xe193b5e1ce7d01c4,
            0x27625b6a622c0518,
        }
    );
    auto f1 = FixedFr1();
    auto f2 = FixedFr2();
    auto res = f1 * f2;
    BOOST_ASSERT(res == exp);
}

BOOST_AUTO_TEST_CASE(testInv)
{
    bandersnatch::Fr exp(
        (uint64_t[4]){
            0xc2eecde5714050d6,
            0x19f77495338244b4,
            0x65208d07b3ef94b1,
            0x524a236a985c691d,
        }
    );
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