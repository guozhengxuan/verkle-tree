#pragma once
#include "bandersnatch/Fr.h"
#include "bandersnatch/Element.h"


namespace verkle::ipa
{
class IPAConfig
{
public:
    bandersnatch::Element commitToPoly(const std::vector<bandersnatch::Fr>& scalars);
private:
    std::vector<bandersnatch::Element> m_srs;
    bandersnatch::Element q;
    uint32_t rounds;
};
}