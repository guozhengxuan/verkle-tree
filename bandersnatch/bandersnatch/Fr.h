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
    using FrListPtr = std::shared_ptr<std::vector<Fr>>;

    Fr();
    Fr(const Fr& other);
    Fr& operator=(const Fr& other);

    // deserialize
    Fr(const uint64_t a[4]);
    Fr(const byte* msg, size_t nbits);

    static Fr zero();
    static Fr one();
    static Fr random();

    // comparison in Montgomery form
    int cmp(const Fr& other) const;

    // from/to uint64_t are only used in ipa eval vector generation
    static Fr fromUint64(uint64_t v);
    uint64_t toUint64() const;

    Fr operator+(const Fr& other) const;
    Fr operator-(const Fr& other) const;
    Fr operator*(const Fr& other) const;
    Fr inv() const;
    bool operator==(const Fr& other) const;
    bool operator!=(const Fr& other) const;

    Fr& operator+=(const Fr& other);
    Fr& operator-=(const Fr& other);
    Fr& operator*=(const Fr& other);

    void serialize(uint64_t ret[4]) const;

private:
    blst_fr m_val;
    friend class Element;
};
}