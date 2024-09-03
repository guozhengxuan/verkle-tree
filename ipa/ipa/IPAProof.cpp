#include "IPAProof.h"
#include "IPAUtility.h"

using namespace verkle::ipa;

IPAProof IPAProof::create(
        Transcript::Ptr& transcript,
        const IPAConfig& config,
        Element const& commitment, 
        Fr::FrListPtr a,
        Fr const& evalPoint
    )
{
    transcript->appendLabel(SeperateLabel::LABEL_IPA);
    
    auto b = config.computeBVector(evalPoint);

    // Compute f(z)
    auto innerProd = innerProduct(a, b);

    transcript->appendPoint(commitment, SeperateLabel::LABEL_COMMITMENT);
    transcript->appendScalar(evalPoint, SeperateLabel::LABEL_INPUT_POINT);
    transcript->appendScalar(innerProd, SeperateLabel::LABEL_OUTPUT_POINT);
    auto w = transcript->generateChallenge(SeperateLabel::LABEL_RESCALING);

    auto q = Element::mult(w, config.m_Q);

    auto rounds = config.m_rounds;

    auto currentBasis = config.m_srs;

    auto L = std::make_shared<std::vector<Element>>(rounds);
    auto R = std::make_shared<std::vector<Element>>(rounds);

    // for debug
    auto rawComm = std::vector<Element>(rounds);
    auto localComm = Element::add(Element::mult(innerProd, q), commitment);
    // end for debug

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
            std::make_shared<std::vector<Element>>(std::vector<Element>{C_L_1, q}),
            std::make_shared<std::vector<Fr>>(std::vector<Fr>{Fr::one(), z_L})
            );

        auto C_R_1 = commit(G_R, a_L);
        auto C_R = commit(
            std::make_shared<std::vector<Element>>(std::vector<Element>{C_R_1, q}),
            std::make_shared<std::vector<Fr>>(std::vector<Fr>{Fr::one(), z_R})
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
        Transcript::Ptr& transcript,
        const IPAConfig& config,
        Element const& commitment,
        Fr const& evalPoint,
        Fr const& result
    ) const
{
    if (m_left->size() != m_right->size() || m_left->size() != config.m_rounds)
    {
        return false;
    }

    transcript->appendLabel(SeperateLabel::LABEL_IPA);

    auto b = config.computeBVector(evalPoint);

    transcript->appendPoint(commitment, SeperateLabel::LABEL_COMMITMENT);
    transcript->appendScalar(evalPoint, SeperateLabel::LABEL_INPUT_POINT);
    transcript->appendScalar(result, SeperateLabel::LABEL_OUTPUT_POINT);

    auto w = transcript->generateChallenge(SeperateLabel::LABEL_RESCALING);

    // Rescaling of q.
    auto q = Element::mult(w, config.m_Q);

    auto adapted = Element::add(commitment, Element::mult(result, q));

    auto challenges = generateChallenges(transcript);
    auto invChallenges = std::make_shared<std::vector<Fr>>(challenges->size());
    for (size_t i = 0; i < challenges->size(); ++i)
    {
        invChallenges->at(i) = challenges->at(i).inv();
    }

    // Compute expected commitment
    for (size_t i = 0; i < challenges->size(); ++i)
    {
        auto x = challenges->at(i);
        auto L = m_left->at(i);
        auto R = m_right->at(i);

        adapted = commit(std::make_shared<std::vector<Element>>(std::vector<Element>{adapted, L, R}),
            std::make_shared<std::vector<Fr>>(std::vector<Fr>{Fr::one(), x, invChallenges->at(i)}));
    }

    auto g = config.m_srs;

    // We compute the folding-scalars for g and b.
    auto foldingScalars = std::make_shared<std::vector<Fr>>(g->size());
    for (size_t i = 0; i < foldingScalars->size(); ++i)
    {
        auto scalar = Fr::one();

        for (size_t challengeIdx = 0; challengeIdx < challenges->size(); ++challengeIdx)
        {
            if ((i & (1<<(7-challengeIdx))) > 0)
            {
                scalar *= invChallenges->at(challengeIdx);
            }
        }

        foldingScalars->at(i) = scalar;
    }

    auto g0 = Element::msm(g, foldingScalars);
    auto b0 = innerProduct(b, foldingScalars);

    auto part1 = Element::mult(m_a, g0);
    auto part2 = Element::mult(b0*m_a, q);

    //  g0 * a + (a * b) * Q
    auto expected = Element::add(part1, part2);

    return expected == adapted;
}


Fr::FrListPtr IPAProof::generateChallenges(Transcript::Ptr const& transcript) const
{
    auto out = std::make_shared<std::vector<Fr>>(m_left->size());
    for (size_t i = 0; i < m_left->size(); ++i)
    {
        transcript->appendPoint(m_left->at(i), SeperateLabel::LABEL_LEFT);
        transcript->appendPoint(m_right->at(i), SeperateLabel::LABEL_RIGHT);
        out->at(i) = transcript->generateChallenge(SeperateLabel::LABEL_X);
    }
    return out;
}