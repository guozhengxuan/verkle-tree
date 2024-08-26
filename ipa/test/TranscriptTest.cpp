//
// Created by Zhengxuan Guo on 2024/8/15.
//
#include "ipa/Transcript.h"
#include "testutils/TestPromptFixture.h"
#include <boost/test/unit_test.hpp>

namespace verkle::test
{
BOOST_FIXTURE_TEST_SUITE(TransCriptTest, TestPromptFixture)

BOOST_AUTO_TEST_CASE(testCallChallengeScalarTwice)
{
    ipa::Transcript tr(ipa::LABEL_IPA);
    auto challenge_1 = tr.generateChallenge(ipa::LABEL_X);
    auto challenge_2 = tr.generateChallenge(ipa::LABEL_X);
    BOOST_ASSERT(challenge_1 != challenge_2);
}

BOOST_AUTO_TEST_CASE(testChallengeScalar)
{
    ipa::Transcript tr(ipa::LABEL_IPA);
    auto challenge = tr.generateChallenge(ipa::LABEL_X);
    uint64_t out[4];
    challenge.serialize(out);

    uint64_t raw[4] = {
        0xe9f26c96d15bb807,
        0x6ad84e7137609152,
        0xe193b5e1ce7d01c4,
        0x27625b6a622c0518,
    };
    bandersnatch::Fr exp(raw);
    BOOST_ASSERT(challenge == exp);
}

BOOST_AUTO_TEST_CASE(testAppendScalar)
{
    uint64_t raw1[4] = {
        0xe9f26c96d15bb807,
        0x6ad84e7137609152,
        0xe193b5e1ce7d01c4,
        0x27625b6a622c0518,
    };
    bandersnatch::Fr base(raw1);
    ipa::Transcript tr(ipa::LABEL_LEFT);

    // append scalar twice
    tr.appendScalar(base, ipa::LABEL_RESCALING);
    tr.appendScalar(base, ipa::LABEL_RESCALING);

    auto challenge = tr.generateChallenge(ipa::LABEL_X);
    uint64_t out[4];
    challenge.serialize(out);

    uint64_t raw2[4] = {
        0xe9f26c96d15bb807,
        0x6ad84e7137609152,
        0xe193b5e1ce7d01c4,
        0x27625b6a622c0518,
    };
    bandersnatch::Fr exp(raw2);
    BOOST_ASSERT(challenge == exp);
}

BOOST_AUTO_TEST_CASE(testAppendPoint)
{
    auto g = bandersnatch::Element::generator();
    ipa::Transcript tr(ipa::LABEL_LEFT);

    // append point twice
    tr.appendPoint(g, ipa::LABEL_INPUT_POINT);
    tr.appendPoint(g, ipa::LABEL_OUTPPUT_POINT);

    auto challenge = tr.generateChallenge(ipa::LABEL_X);
    uint64_t out[4];
    challenge.serialize(out);

    uint64_t raw2[4] = {
        0xe9f26c96d15bb807,
        0x6ad84e7137609152,
        0xe193b5e1ce7d01c4,
        0x27625b6a622c0518,
    };
    bandersnatch::Fr exp(raw2);
    BOOST_ASSERT(challenge == exp);
}

BOOST_AUTO_TEST_CASE(testAppendLabel)
{
    auto g = bandersnatch::Element::generator();
    ipa::Transcript tr(ipa::LABEL_LEFT);

    // append point and some labels
    tr.appendPoint(g, ipa::LABEL_INPUT_POINT);
    tr.appendLabel(ipa::LABEL_COMMITMENT);
    tr.appendPoint(g, ipa::LABEL_OUTPPUT_POINT);
    tr.appendLabel(ipa::LABEL_RIGHT);

    auto challenge = tr.generateChallenge(ipa::LABEL_X);
    uint64_t out[4];
    challenge.serialize(out);

    uint64_t raw2[4] = {
        0xe9f26c96d15bb807,
        0x6ad84e7137609152,
        0xe193b5e1ce7d01c4,
        0x27625b6a622c0518,
    };
    bandersnatch::Fr exp(raw2);
    BOOST_ASSERT(challenge == exp);
}

BOOST_AUTO_TEST_SUITE_END()
}