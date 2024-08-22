#pragma once
#include "bandersnatch/Fr.h"
#include "bandersnatch/Element.h"

namespace verkle::test
{
static inline const bandersnatch::Fr& FixedFr1()
{
    static uint64_t raw[4] = {
        0xe5d76b4ca918a221,
        0x81ed0b8d092ec3c6,
        0xd124e15f2054fa57,
        0x2ef123703093cbbb,
    };
    static bandersnatch::Fr fr1(raw);
    return fr1;
}

static inline const bandersnatch::Fr& FixedFr2()
{
    static uint64_t raw[4] = {
        0xd894647091277b9c,
        0x5182f0e810967f58,
        0x42958c4a5cf47f12,
        0x61b589a0f601132d,
    };
    static bandersnatch::Fr fr2(raw);
    return fr2;
}

static inline const bandersnatch::Element& FixedGenerator()
{
    static auto g = bandersnatch::Element::generator();
    return g;
}
}