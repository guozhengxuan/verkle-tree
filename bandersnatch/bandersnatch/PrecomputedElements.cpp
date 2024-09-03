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
    auto affinePoints = std::make_shared<blst_p1_affine[]>(m_numPoints);;
    for (size_t i = 0; i < m_numPoints; ++i)
    {
        blst_p1_to_affine(&affinePoints[i], &points->at(i).m_point);
    }

    const blst_p1_affine* pointsArg[2] = {affinePoints.get(), nullptr};
    blst_p1s_mult_wbits_precompute(m_table.get(), m_window, pointsArg, m_numPoints);
}

Element PrecomputedElements::msm(Fr::FrListPtr const& scalars) const
{
    size_t sz = blst_p1s_mult_wbits_scratch_sizeof(m_numPoints);
    auto scratch = std::make_shared<limb_t[]>(sz/sizeof(limb_t));
    auto baseScalars = std::make_shared<blst_scalar[]>(m_numPoints);
    for (size_t i = 0; i < m_numPoints; ++i)
    {
        blst_scalar_from_fr(&baseScalars[i], &scalars->at(i).m_val);
    }

    Element ret;
    const byte* scalarsArg[2] = {reinterpret_cast<byte*>(baseScalars.get()), nullptr};
    blst_p1s_mult_wbits(&ret.m_point, m_table.get(), m_window, m_numPoints, scalarsArg, 255, scratch.get());

    return ret;
}