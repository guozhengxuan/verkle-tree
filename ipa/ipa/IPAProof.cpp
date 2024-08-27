#include "IPAProof.h"
#include "IPAUtility.h"

using namespace verkle::ipa;

IPAProof IPAProof::create(
        Transcript::Ptr transcript,
        IPAConfig::Ptr config, 
        Element const& commitment, 
        Fr::FrListPtr const& a,
        Fr const& evalPoint
    )
{
    transcript->appendLabel(LABEL_IPA);
    
    auto b = config->computeBVector(evalPoint);
    auto innerProd = innerProduct(a, b);

    transcript->appendPoint(commitment, LABEL_COMMITMENT);
    transcript->appendScalar(evalPoint, LABEL_INPUT_POINT);
    transcript->appendScalar(innerProd, LABEL_OUTPUT_POINT);
    auto w = transcript->generateChallenge(LABEL_RESCALING);

    auto q = config->m_Q.mult(w);

    auto rounds = config->rounds;

    auto currentBasis = config->m_srs;

    auto L = std::make_shared<std::vector<Element>>(rounds);
    auto R = std::make_shared<std::vector<Element>>(rounds);

    for (size_t i = 0; i < rounds; i++)
    {
        Fr::FrListPtr a_L, a_R;
        splitSlice(a, a_L, a_R);

        Fr::FrListPtr b_L, b_R;
        splitSlice(b, b_L, b_R);

        Element::ElementListPtr G_L, G_R;
        splitSlice(currentBasis, G_L, G_R);

        auto z_L = innerProduct(a_R, b_L);
        auto z_R = innerProduct(a_L, b_R);

        // auto C_L_1 =

    }
}

bool IPAProof::check(
        Transcript::Ptr transcript, 
        IPAConfig::Ptr config, 
        Element const& commitment, 
        Fr const& evalPoint,
        Fr const& result
    )
{

}