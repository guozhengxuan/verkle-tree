#include "IPAUtility.h"

using namespace verkle::ipa;

Fr innerProduct(Fr::FrListPtr const& a, Fr::FrListPtr const& b)
{
    if (a->size() != b->size()) {
        throw std::runtime_error("a and b are different sizes, " +
            std::to_string(a->size()) + " != " + std::to_string(b->size()));
    }

    auto ret = Fr::zero();
    for (size_t i = 0; i < a->size(); ++i) {
        ret += a->at(i) * b->at(i);
    }

    return ret;
}

template <Splitable T>
void splitSlice(T a, T& out1, T& out2)
{
    if (a->size()%2 != 0)
    {
        throw std::runtime_error("scalars or elements should have even length");
    }

    auto mid = a->size() / 2;

    std::vector<Fr> first(mid);
    std::vector<Fr> second(a->size()-mid);

    std::move(a->begin(), a->begin()+mid, first.begin());
    std::move(a->begin()+mid, a->end(), second.begin());

    out1 = std::make_shared<decltype(first)>(first);
    out2 = std::make_shared<decltype(second)>(second);
}

void commit(Element::ElementListPtr const& groupElements, Fr::FrListPtr const& polynomial)
{
    if (groupElements->size() != polynomial->size()) {
        throw std::runtime_error("a and b are different sizes, " +
            std::to_string(groupElements->size()) + " != " + std::to_string(polynomial->size()));
    }



}
