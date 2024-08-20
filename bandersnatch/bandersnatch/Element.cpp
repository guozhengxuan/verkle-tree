//
// Created by Zhengxuan Guo on 2024/8/13.
//
#include "Element.h"

using namespace verkle::bandersnatch;

Element::Element() : m_point(std::make_shared<blst_p1>()) {}

Element::Element(const blst_p1& point) : m_point(std::make_shared<blst_p1>(point)) {}

Element::Element(const byte* in, size_t len) : m_point(std::make_shared<blst_p1>())
{
    if (len == 0 || len != (in[0]&0x80 ? 48 : 96))
        throw BLST_BAD_ENCODING;
    blst_p1_affine a;
    BLST_ERROR err = blst_p1_deserialize(&a, in);
    if (err != BLST_SUCCESS)
        throw err;
    blst_p1_from_affine(m_point.get(), &a);
}

Element& Element::add(const Element& other)
{
    blst_p1_add(m_point.get(), m_point.get(), other.m_point.get());
    return *this;
}

Element& Element::dbl()
{
    blst_p1_double(m_point.get(), m_point.get());
    return *this;
}

Element& Element::mult(const Fr& fr)
{
    blst_scalar scalar;
    blst_scalar_from_fr(&scalar, fr.m_val.get());
    blst_p1_mult(m_point.get(), m_point.get(), scalar.b, 255);
    return *this;
}

Element Element::add(const Element& a, const Element& b)
{
    Element ret;
    blst_p1_add(ret.m_point.get(), a.m_point.get(), b.m_point.get());
    return ret;
}

Element Element::dbl(const Element& a)
{
    Element ret;
    blst_p1_double(ret.m_point.get(), a.m_point.get());
    return ret;
}

Element Element::mult(const Fr& fr, const Element& a)
{
    Element ret;
    blst_scalar scalar;
    blst_scalar_from_fr(&scalar, fr.m_val.get());
    blst_p1_mult(ret.m_point.get(), a.m_point.get(), scalar.b, 255);
    return ret;
}

Element Element::generator()
{
    auto g = blst_p1_generator();
    return Element(*g);
}

void Element::serialize(byte out[96]) const
{
    blst_p1_serialize(out, m_point.get());
}
