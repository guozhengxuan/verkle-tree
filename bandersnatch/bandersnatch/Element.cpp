//
// Created by Zhengxuan Guo on 2024/8/13.
//
#include "Element.h"

using namespace verkle::bandersnatch;

Element::Element() = default;

Element::Element(const Element& other) : m_point(other.m_point) {}

Element& Element::operator=(const Element& other)
{
    if (this == &other)
    {
        return *this;
    }
    this->m_point = other.m_point;
    return *this;
}

Element::Element(const byte* in, size_t len)
{
    if (len == 0 || len != (in[0]&0x80 ? 48 : 96))
        throw BLST_BAD_ENCODING;
    blst_p1_affine a;
    BLST_ERROR err = blst_p1_deserialize(&a, in);
    if (err != BLST_SUCCESS)
        throw err;
    blst_p1_from_affine(&m_point, &a);
}

Element& Element::add(const Element& other)
{
    blst_p1_add_or_double(&m_point, &m_point, &other.m_point);
    return *this;
}

Element& Element::dbl()
{
    blst_p1_double(&m_point, &m_point);
    return *this;
}

Element& Element::mult(const Fr& fr)
{
    blst_scalar scalar;
    blst_scalar_from_fr(&scalar, &fr.m_val);
    blst_p1_mult(&m_point, &m_point, scalar.b, 255);
    return *this;
}

Element Element::multiscalarMult(Element::ElementListPtr points, Fr::FrListPtr scalars)
{
    if (points->size() != scalars->size()) {
        throw std::runtime_error("points and scalars have different sizes, " +
            std::to_string(points->size()) + " != " + std::to_string(scalars->size()));
    }

    auto n = points->size();

    // TODO: parallel optimization
    // By now we use Pippenger's algorithm to accelerate MSM (Multi-Scalar Multiplication).
    // It is implemented in blst lib.
    auto scratch = blst_p1s_mult_pippenger_scratch_sizeof(points->size());
    // void blst_p1s_mult_pippenger(blst_p1 *ret, const blst_p1_affine *const points[],
    //                          size_t npoints, const byte *const scalars[],
    //                          size_t nbits, limb_t *scratch);
    blst_p1 ret;
    auto* affinePoints = new blst_p1_affine[n];
    for (auto i = 0; i < n; ++i)
    {
        blst_p1_to_affine(&affinePoints[i], &points->at(i).m_point);
    }

    auto* baseScalars = new byte[n];

}

bool Element::operator==(const Element& other) const
{
    return blst_p1_is_equal(&m_point, &other.m_point);
}

bool Element::operator!=(const Element& other) const
{
    return !(*this == other);
}

Element Element::add(const Element& a, const Element& b)
{
    Element ret;
    blst_p1_add_or_double(&ret.m_point, &a.m_point, &b.m_point);
    return ret;
}

Element Element::dbl(const Element& a)
{
    Element ret;
    blst_p1_double(&ret.m_point, &a.m_point);
    return ret;
}

Element Element::mult(const Fr& fr, const Element& a)
{
    Element ret;
    blst_scalar scalar;
    blst_scalar_from_fr(&scalar, &fr.m_val);
    blst_p1_mult(&ret.m_point, &a.m_point, scalar.b, 255);
    return ret;
}

Element Element::generator()
{
    Element ret;
    auto g = blst_p1_generator();
    ret.m_point = *g;
    return ret;
}

void Element::serialize(byte out[96]) const
{
    blst_p1_serialize(out, &m_point);
}

void Element::compress(byte out[48]) const
{
    blst_p1_compress(out, &m_point);
}