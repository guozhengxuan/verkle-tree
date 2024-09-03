//
// Created by Zhengxuan Guo on 2024/8/13.
//
#include "Element.h"
#include "cstring"

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
    {
        throw BLST_BAD_ENCODING;
    }

    blst_p1_affine a;
    BLST_ERROR err = blst_p1_deserialize(&a, in);

    if (err != BLST_SUCCESS)
    {
        throw err;
    }

    blst_p1_from_affine(&m_point, &a);
}

bool Element::isInG1() const
{
    return blst_p1_on_curve(&m_point) && blst_p1_in_g1(&m_point);
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


// This method CAN be called with the point at infinity in `blst_p1` (zero point).

// While this function is significantly faster than g1_lincomb_naive(), we refrain from
// using it in security-critical places (like verification) because the blst Pippenger code has not
// been audited. In those critical places, we prefer using g1_lincomb_naive() which is much simpler.
Element Element::msm(const ElementListPtr& points, const Fr::FrListPtr& scalars)
{
    if (points->size() != scalars->size()) {
        throw std::runtime_error("points and scalars have different sizes, " +
            std::to_string(points->size()) + " != " + std::to_string(scalars->size()));
    }

    auto const n = points->size();

    // TODO: parallel optimization
    // By now we use Pippenger's algorithm to accelerate MSM (Multi-Scalar Multiplication).
    // It is implemented in blst lib.
    auto const sz = blst_p1s_mult_pippenger_scratch_sizeof(n);
    auto scratch = std::make_shared<limb_t[]>(sz/sizeof(limb_t));

    auto affinePoints = std::make_shared<blst_p1_affine[]>(n);
    auto baseScalars = std::make_shared<blst_scalar[]>(n);;

    // Filter out zero points: make a new list p_filtered that contains only non-zero points.
    size_t newLen = 0;
    for (auto i = 0; i < n; ++i)
    {
        if (!blst_p1_is_inf(&points->at(i).m_point))
        {
            blst_p1_to_affine(&affinePoints[newLen], &points->at(i).m_point);
            blst_scalar_from_fr(&baseScalars[newLen], &scalars->at(i).m_val);
            newLen++;
        }
    }

    const blst_p1_affine* pointsArg[2] = {affinePoints.get(), nullptr};
    const byte* scalarsArg[2] = {reinterpret_cast<byte*>(baseScalars.get()), nullptr};

    Element ret;
    blst_p1s_mult_pippenger(&ret.m_point, pointsArg, newLen, scalarsArg, 255, scratch.get());

    // debug
    auto ret1 = Element::zero();
    for (size_t i = 0; i < n; ++i)
    {
        ret1.add(Element::mult(scalars->at(i), points->at(i)));
    }
    auto a = 1;

    return ret;
}

bool Element::operator==(const Element& other) const
{
    return blst_p1_is_equal(&m_point, &other.m_point);
}

bool Element::operator!=(const Element& other) const
{
    return !(*this == other);
}

Element Element::zero()
{
    // Identity/infinity of G1.
    Element ret;
    memset(&ret.m_point, 0, sizeof(ret.m_point));
    return ret;
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