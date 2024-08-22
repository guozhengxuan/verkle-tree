//
// Created by Zhengxuan Guo on 2024/8/13.
//

#include <cstdint>
#include <cstring>
#include <random>
#include "Fr.h"

using namespace verkle::bandersnatch;

Fr::Fr() = default;

Fr::Fr(const Fr& other) : m_val(other.m_val) {}

Fr& Fr::operator=(const Fr& other)
{
    if (this == &other)
    {
        return *this;
    }
    this->m_val = other.m_val;
    return *this;
}

Fr::Fr(const uint64_t a[4])
{
    blst_fr_from_uint64(&m_val, a);
}

Fr::Fr(const uint8_t* msg, size_t len)
{
    blst_scalar scalar;
    blst_scalar_from_le_bytes(&scalar, msg, (len+7)/8);
    blst_fr_from_scalar(&m_val, &scalar);
}

Fr Fr::zero()
{
    // zero value of Fr is simply 0x0
    Fr ret;
    memset(&ret.m_val, 0, sizeof(ret.m_val));
    return ret;
}

Fr Fr::random()
{
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(0, UINT64_MAX);

    uint64_t a[4];
    for (int i = 0; i < 4; ++i)
    {
        a[i] = dis(gen);
    }
    Fr ret(a);
    return ret;
}

Fr Fr::operator+(const Fr& other) const
{
    Fr ret;
    blst_fr_add(&ret.m_val, &m_val, &other.m_val);
    return ret;
}

Fr Fr::operator-(const Fr& other) const
{
    Fr ret;
    blst_fr_sub(&ret.m_val, &m_val, &other.m_val);
    return ret;
}

Fr Fr::operator*(const Fr& other) const
{
    Fr ret;
    blst_fr_mul(&ret.m_val, &m_val, &other.m_val);
    return ret;
}

bool Fr::operator==(const Fr& other) const
{
    return std::memcmp(&m_val, &other.m_val, sizeof(blst_fr)) == 0;
}

bool Fr::operator!=(const Fr& other) const
{
    return !(*this == other);
}

Fr Fr::inv() const
{
    Fr ret;
    blst_fr_inverse(&ret.m_val, &m_val);
    return ret;
}

Fr& Fr::operator+=(const Fr& other)
{
    blst_fr_add(&m_val, &m_val, &other.m_val);
    return *this;
}

Fr& Fr::operator-=(const Fr& other)
{
    blst_fr_sub(&m_val, &m_val, &other.m_val);
    return *this;
}

Fr& Fr::operator*=(const Fr& other)
{
    blst_fr_mul(&m_val, &m_val, &other.m_val);
    return *this;
}

void Fr::serialize(uint64_t ret[4]) const
{
    blst_uint64_from_fr(ret, &m_val);
}