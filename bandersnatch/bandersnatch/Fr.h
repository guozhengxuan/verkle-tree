//
// Created by Zhengxuan Guo on 2024/8/13.
//
#pragma once
#include <blst.h>
#include <vector>

namespace verkle::bandersnatch
{
class Fr
{
public:
    Fr();
    Fr(const uint64_t a[4]);
    Fr(const byte* msg, size_t len);

    static Fr zero();
    static Fr random();

    Fr operator+(const Fr& other) const;
    Fr operator-(const Fr& other) const;
    Fr operator*(const Fr& other) const;
    Fr inv() const;
    bool operator==(const Fr& other) const;

    std::shared_ptr<const blst_scalar> toScalar() const;

    void serialize(uint64_t ret[4]) const;

private:
    std::shared_ptr<blst_fr> m_val;
};
}