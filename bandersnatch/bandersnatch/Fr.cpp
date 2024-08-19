//
// Created by Zhengxuan Guo on 2024/8/13.
//

#include <cstdint>
#include <random>
#include "Fr.h"

using namespace verkle::bandersnatch;

Fr::Fr() : m_val(std::make_shared<blst_fr>()) {}

Fr::Fr(const uint64_t a[4]) : m_val(std::make_shared<blst_fr>()) 
{
    blst_fr_from_uint64(m_val.get(), a);
}

Fr::Fr(const uint8_t* msg, size_t len) : m_val(std::make_shared<blst_fr>())
{
    blst_scalar scalar;
    blst_scalar_from_le_bytes(&scalar, msg, (len+7)/8);
    blst_fr_from_scalar(m_val.get(), &scalar);
}

Fr Fr::zero()
{
    // zero value of Fr is simply 0x0
    return Fr();
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
    Fr ret;
    blst_fr_from_uint64(ret.m_val.get(), a);
    return ret;
}

Fr Fr::operator+(const Fr& other) const
{
    Fr ret;
    blst_fr_add(ret.m_val.get(), m_val.get(), other.m_val.get());
    return ret;
}

Fr Fr::operator-(const Fr& other) const
{
    Fr ret;
    blst_fr_sub(ret.m_val.get(), m_val.get(), other.m_val.get());
    return ret;
}

Fr Fr::operator*(const Fr& other) const
{
    Fr ret;
    blst_fr_mul(ret.m_val.get(), m_val.get(), other.m_val.get());
    return ret;
}

bool Fr::operator==(const Fr& other) const
{
    for (int i = 0; i < 4; ++i)
    {
        if (m_val.get()->l[i] != other.m_val.get()->l[i])
        {
            return false;
        }
    }
    return true;
}

Fr Fr::inv() const
{
    Fr ret;
    blst_fr_inverse(ret.m_val.get(), m_val.get());
    return ret;
}

std::shared_ptr<const blst_scalar> Fr::toScalar() const
{
    auto ret = std::make_shared<blst_scalar>();
    blst_scalar_from_fr(ret.get(), m_val.get());
    return ret;
}


void Fr::serialize(uint64_t ret[4]) const
{
    blst_uint64_from_fr(ret, m_val.get());
}