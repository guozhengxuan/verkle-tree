#include "IPAProof.h"
#include "IPAUtility.h"

using namespace verkle::ipa;

IPAProof IPAProof::create(
        Transcript::Ptr transcript,
        IPAConfig::Ptr config, 
        Element const& commitment, 
        Fr::FrListPtr& a,
        Fr const& evalPoint
    )
{
    transcript->appendLabel(SeperateLabel::LABEL_IPA);
    
    auto b = config->computeBVector(evalPoint);
    auto innerProd = innerProduct(a, b);

    transcript->appendPoint(commitment, SeperateLabel::LABEL_COMMITMENT);
    transcript->appendScalar(evalPoint, SeperateLabel::LABEL_INPUT_POINT);
    transcript->appendScalar(innerProd, SeperateLabel::LABEL_OUTPUT_POINT);
    auto w = transcript->generateChallenge(SeperateLabel::LABEL_RESCALING);

    auto q = config->m_Q.mult(w);

    auto rounds = config->rounds;

    auto currentBasis = config->m_srs;

    auto L = std::make_shared<std::vector<Element>>(rounds);
    auto R = std::make_shared<std::vector<Element>>(rounds);

    for (size_t i = 0; i < rounds; i++)
    {
        Fr::FrListPtr a_L, a_R;
        split(a, a_L, a_R);

        Fr::FrListPtr b_L, b_R;
        split(b, b_L, b_R);

        Element::ElementListPtr G_L, G_R;
        split(currentBasis, G_L, G_R);

        auto z_L = innerProduct(a_R, b_L);
        auto z_R = innerProduct(a_L, b_R);

        auto C_L_1 = commit(G_L, a_R);
        auto C_L = commit(
            std::make_shared<std::vector<Element>>(std::initializer_list<auto>{C_L_1, q}),
            std::make_shared<std::vector<Fr>>(std::initializer_list<auto>{Fr::one(), z_L})
            );

        auto C_R_1 = commit(G_R, a_L);
        auto C_R = commit(
            std::make_shared<std::vector<Element>>(std::initializer_list<auto>{C_R_1, q}),
            std::make_shared<std::vector<Fr>>(std::initializer_list<auto>{Fr::one(), z_R})
            );

        L->at(i) = C_L;
        R->at(i) = C_R;

        transcript->appendPoint(C_L, SeperateLabel::LABEL_LEFT);
        transcript->appendPoint(C_R, SeperateLabel::LABEL_RIGHT);
        auto x = transcript->generateChallenge(SeperateLabel::LABEL_X);
        auto xInv = x.inv();

        a = foldScalars(a_L, a_R, x);
        b = foldScalars(b_L, b_R, xInv);
        currentBasis = foldPoints(G_L, G_R, xInv);
    }

    if (a->size() != 1)
    {
        throw std::runtime_error("length of `a` should be 1 at the end of the reduction");
    }

    IPAProof ret;
    ret.m_left = L;
    ret.m_right = R;
    ret.m_a = a->at(0);
    return ret;
}

bool IPAProof::check (
        Transcript::Ptr transcript, 
        IPAConfig::Ptr config, 
        Element& commitment,
        Fr const& evalPoint,
        Fr const& result
    ) const
{
    if (m_left->size() != m_right->size() || m_left->size() != config->rounds)
    {
        return false;
    }

    transcript->appendLabel(SeperateLabel::LABEL_IPA);

    auto b = config->computeBVector(evalPoint);

    transcript->appendPoint(commitment, SeperateLabel::LABEL_COMMITMENT);
    transcript->appendScalar(evalPoint, SeperateLabel::LABEL_INPUT_POINT);
    transcript->appendScalar(result, SeperateLabel::LABEL_OUTPUT_POINT);

    auto w = transcript->generateChallenge(SeperateLabel::LABEL_RESCALING);

    auto q = config->m_Q.mult(w);

    commitment.add(q.mult(result));

    auto 

}