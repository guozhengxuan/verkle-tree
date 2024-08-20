//
// Created by Zhengxuan Guo on 2024/8/13.
//
#pragma once
#include <blst.h>
#include <vector>

namespace verkle::bandersnatch
{
class Element;
class Fr
{
public:
    Fr();

    // deserialize
    Fr(const uint64_t a[4]);
    Fr(const byte* msg, size_t len);

    static Fr zero();
    static Fr random();

    Fr operator+(const Fr& other) const;
    Fr operator-(const Fr& other) const;
    Fr operator*(const Fr& other) const;
    Fr inv() const;
    bool operator==(const Fr& other) const;
    bool operator!=(const Fr& other) const;

    void serialize(uint64_t ret[4]) const;

private:
    std::shared_ptr<blst_fr> m_val;
    friend class Element;
};
}