//
// Created by Zhengxuan Guo on 2024/8/30.
//

#include "PrecomputedMSM.h"
#include "IPAUtility.h"

using namespace verkle::ipa;

PrecomputedMSM::PrecomputedMSM(Element::ElementListPtr const& points)
{
    if (points->size() != supportedMSMLength)
    {
        throw std::runtime_error("the number of points must be " + std::to_string(supportedMSMLength));
    }

    auto first = std::make_shared<std::vector<Element>>(points->begin(), points->begin()+window16vs8IndexLimit);
    auto second = std::make_shared<std::vector<Element>>(points->begin()+window16vs8IndexLimit, points->end());
    m_heavy = std::make_shared<PrecomputedElements>(first, 14);
    m_light = std::make_shared<PrecomputedElements>(second, 8);
}

Element PrecomputedMSM::msm(Fr::FrListPtr const& scalars)
{
    if (scalars->size() != supportedMSMLength)
    {
        throw std::runtime_error("the number of points must be " + std::to_string(supportedMSMLength));
    }

    auto firstFrs = std::make_shared<std::vector<Fr>>(scalars->begin(), scalars->begin()+window16vs8IndexLimit);
    auto secondFrs = std::make_shared<std::vector<Fr>>(scalars->begin()+window16vs8IndexLimit, scalars->end());

    auto heavyRes = m_heavy->msm(firstFrs);
    auto lightRes = m_light->msm(secondFrs);

    return Element::add(heavyRes, lightRes);
}