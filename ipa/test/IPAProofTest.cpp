//
// Created by Zhengxuan Guo on 2024/8/30.
//
#include "testutils/TestPromptFixture.h"
#include <boost/test/unit_test.hpp>

#include "Utility.h"
#include "bandersnatch/Fr.h"
#include "ipa/Transcript.h"
#include "ipa/IPAProof.h"
#include "ipa/IPAUtility.h"

namespace verkle::ipa::test
{
    BOOST_FIXTURE_TEST_SUITE(PrecomputedFrsTest, verkle::test::TestPromptFixture)

    BOOST_AUTO_TEST_CASE(TestIPAProofCreateVerify)
    {
        // Shared view
        auto point = bandersnatch::Fr::fromUint64(123456789);

        // Prover view
        auto poly = testPoly256(std::vector<uint64_t>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14});
        auto& config = IPAConfig::getConfig();
        auto proverCommitment = config.commit(poly);
        auto proverTranscript = std::make_shared<Transcript>(SeperateLabel::LABEL_IPA);
        auto proof = IPAProof::create(proverTranscript, config, proverCommitment, poly, point);

        auto lagrangeCoeffs = config.m_precomputed_weights->computeBarycentricCoefficients(point);
        auto innerProd = innerProduct(poly, lagrangeCoeffs);

        // Verifier view
        auto verifierCommitment = proverCommitment;
        auto verifierTranscript = std::make_shared<Transcript>(SeperateLabel::LABEL_IPA);

        BOOST_ASSERT(proof.check(verifierTranscript, config, verifierCommitment, point, innerProd));
    }

    BOOST_AUTO_TEST_CASE(TestIPAProofSerializtion)
    {
        // TODO
    }

    BOOST_AUTO_TEST_SUITE_END()
}
