#include "IPAConfig.h"

using namespace verkle::ipa;

Fr::FrListPtr IPAConfig::computeBVector(Fr const& evalPoint) const
{
    if (evalPoint.cmp(maxEvalPoinInsideDomain) > 0)
    {
        return m_precomputed_weights.computeBarycentricCoefficients(evalPoint);
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