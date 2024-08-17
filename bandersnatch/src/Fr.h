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
    Fr(const byte* msg, size_t len);

    Fr operator+(const Fr& other) const;
    Fr operator-(const Fr& other) const;
    Fr operator*(const Fr& other) const;
    Fr inv() const;

    std::shared_ptr<const blst_scalar> toScalar() const;

    void serialize(uint64_t ret[4]) const;

private:
    std::unique_ptr<blst_fr> m_val;
};
}