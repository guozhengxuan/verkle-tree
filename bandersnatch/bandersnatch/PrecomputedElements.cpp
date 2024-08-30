//
// Created by Zhengxuan Guo on 2024/8/30.
//
#include "PrecomputedElements.h"

using namespace verkle::bandersnatch;

PrecomputedElements::PrecomputedElements(Element::ElementListPtr const& points, size_t window)
    : m_window(window),
    m_numPoints(points->size()),
    m_table(std::make_shared<blst_p1_affine[]>([window, this]() {
            auto const bits = blst_p1s_mult_wbits_precompute_sizeof(window, m_numPoints);
            return bits / sizeof(blst_p1_affine);
    }()))
{
    blst_p1_affine affinePoints[m_numPoints];
    for (size_t i = 0; i < m_numPoints; ++i)
    {
        blst_p1_to_affine(&affinePoints[i], &points->at(i).m_point);
    }

    const blst_p1_affine* pointsArg[2] = {affinePoints, nullptr};
    blst_p1s_mult_wbits_precompute(m_table.get(), m_window, pointsArg, m_numPoints);
}

Element PrecomputedElements::msm(Fr::FrListPtr const& scalars) const
{
    size_t sz = blst_p1s_mult_wbits_scratch_sizeof(m_numPoints);
    limb_t scratch[sz/sizeof(limb_t)];

    blst_scalar baseScalars[m_numPoints];
    for (size_t i = 0; i < m_numPoints; ++i)
    {
        blst_scalar_from_fr(&baseScalars[i], &scalars->at(i).m_val);
    }

    Element ret;
    const byte* scalarsArg[2] = {reinterpret_cast<byte*>(baseScalars), nullptr};
    blst_p1s_mult_wbits(&ret.m_point, m_table.get(), m_window, m_numPoints, scalarsArg, 255, scratch);

    return ret;
}