//
// Created by Zhengxuan Guo on 2024/8/27.
//
#pragma once
#include "Fr.h"
#include "Element.h"

namespace verkle::bandersnatch
{
    Element multiScalar(Element::ElementListPtr points, Fr::FrListPtr scalars);
}
