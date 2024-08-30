//
// Created by Zhengxuan Guo on 2024/8/13.
//
#pragma once
#include <blst.h>
#include "Fr.h"

namespace verkle::bandersnatch
{
class Fr;
class PrecomputedElements;

class Element
{
public:
    using ElementListPtr = std::shared_ptr<std::vector<Element>>;
    
    Element();

    // deserialize
    Element(const Element& other);
    [[nodiscard]] bool isInG1() const;

    Element& operator=(const Element& other);

    Element(const byte *in, size_t len);

    Element& add(const Element& other);
    Element& dbl();
    Element& mult(const Fr& fr);
    bool operator==(const Element& other) const;
    bool operator!=(const Element& other) const;

    static Element zero();
    static Element add(const Element& a, const Element& b);
    static Element dbl(const Element& a);
    static Element mult(const Fr& fr, const Element& a);
    static Element msm(const Element::ElementListPtr& points, const Fr::FrListPtr& scalars);

    static Element generator();

    void serialize(byte out[96]) const;
    void compress(byte out[48]) const;

private:
    blst_p1 m_point;
    friend class PrecomputedElements;
};    
}