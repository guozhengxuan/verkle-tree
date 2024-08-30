//
// Created by Zhengxuan Guo on 2024/8/30.
//
#pragma once
#include "Element.h"
#include "Fr.h"

namespace verkle::bandersnatch
{
    class PrecomputedElements {
    public:
        using Ptr = std::shared_ptr<PrecomputedElements>;
        explicit PrecomputedElements(Element::ElementListPtr const& points, size_t window);
        [[nodiscard]] Element msm(Fr::FrListPtr const& scalars) const;

    private:
        size_t m_window;
        size_t m_numPoints;
        std::shared_ptr<blst_p1_affine[]> m_table;
    };
}
