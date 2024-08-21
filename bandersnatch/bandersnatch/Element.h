//
// Created by Zhengxuan Guo on 2024/8/13.
//
#pragma once
#include <blst.h>
#include "Fr.h"

namespace verkle::bandersnatch
{
class Element
{
public:
    Element();
    Element(const Element& other);
    Element& operator=(const Element& other);

    Element(const byte *in, size_t len);

    Element& add(const Element& other);
    Element& dbl();
    Element& mult(const Fr& fr);
    bool operator==(const Element& other) const;
    bool operator!=(const Element& other) const;

    static Element add(const Element& a, const Element& b);
    static Element dbl(const Element& a);
    static Element mult(const Fr& fr, const Element& a);

    static Element generator();

    void serialize(byte out[96]) const;
    void compress(byte out[48]) const;

private:
    blst_p1 m_point;
};    
}