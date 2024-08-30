#include "IPAConfig.h"
#include "IPAUtility.h"
#include "PrecomputedMSM.h"

using namespace verkle::ipa;

IPAConfig::IPAConfig():
    m_srs(generateRandomPoints(common::vectorLength)),
    m_Q(Element::generator()),
    m_precomputed_msm(std::make_shared<PrecomputedMSM>(PrecomputedMSM(m_srs))),
    m_precomputed_weights(std::make_shared<PrecomputedWeights>(PrecomputedWeights())),
    m_rounds(computeNumRounds(common::vectorLength)) {}

IPAConfig const& IPAConfig::getConfig()
{
    static auto config = IPAConfig();
    return config;
}

Element IPAConfig::commit(const Fr::FrListPtr& scalars) const
{
    return m_precomputed_msm->msm(scalars);
}

Fr::FrListPtr IPAConfig::computeBVector(Fr const& evalPoint) const
{
    if (evalPoint.cmp(maxEvalPoinInsideDomain) > 0)
    {
        return m_precomputed_weights->computeBarycentricCoefficients(evalPoint);
    }

    // We build b = [0, 0, 0, ... , 1, .., 0] where the 1 element is at the index of the evaluation point.
	// This is correct since innerProductArgument(a, b) will return the evaluation of the polynomial at the
	// evaluation point in the domain.
    auto b = std::make_shared<std::vector<Fr>>(256);
    auto index = evalPoint.toUint64();

    // index is safe because we checked that evalPoint is inside the domain (i.e <256).
    b->at(index) = bandersnatch::Fr::one();

    return b;
}