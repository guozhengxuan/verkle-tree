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
    explicit Element(const blst_p1& point);
    Element(const byte *in, size_t len);

    Element& add(const Element& other);
    Element& dbl();
    Element& mult(const Fr& fr);
    static Element add(const Element& a, const Element& b);
    static Element dbl(const Element& a);
    static Element mult(const Fr& fr, const Element& a);

    static Element generator();

    void serialize(byte out[96]) const;

private:
    std::shared_ptr<blst_p1> m_point;
};    
}