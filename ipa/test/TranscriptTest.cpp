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
        0x9c8ca8bdb2785ef8,
        0x7f5c036bd7f1b221,
        0x69d73d9566c4ea5d,
        0x46d9ee65bc2da30d,
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
        0xa1458f845d64bdc,
        0x6240c0aaf1ccf2c1,
        0xaa1ea719e83b622c,
        0x50a286aee96a8e8c,
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
    tr.appendPoint(g, ipa::LABEL_OUTPUT_POINT);

    auto challenge = tr.generateChallenge(ipa::LABEL_X);
    uint64_t out[4];
    challenge.serialize(out);

    uint64_t raw2[4] = {
        0x3d7bcf250fbbd4ca,
        0x2e34916fb1c77331,
        0x342beba88f4caf1b,
        0x165ec3c487655fc3,
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
    tr.appendPoint(g, ipa::LABEL_OUTPUT_POINT);
    tr.appendLabel(ipa::LABEL_RIGHT);

    auto challenge = tr.generateChallenge(ipa::LABEL_X);
    uint64_t out[4];
    challenge.serialize(out);

    uint64_t raw2[4] = {
        0x65d00a1fd12c7f1a,
        0x3fa282eb46713efa,
        0x7103e2c705f23b7c,
        0xe26e3e9526d325d,
    };
    bandersnatch::Fr exp(raw2);
    BOOST_ASSERT(challenge == exp);
}

BOOST_AUTO_TEST_SUITE_END()
}