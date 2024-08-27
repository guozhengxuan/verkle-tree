#pragma once
#include "bandersnatch/Fr.h"
#include "bandersnatch/Element.h"

using verkle::bandersnatch::Fr;
using verkle::bandersnatch::Element;

namespace verkle::ipa
{
    template <typename T>
    concept IsSharedVector = requires(T a)
    {
        { a->size() } -> std::convertible_to<std::size_t>;
        { a->begin() } -> std::random_access_iterator;
        { a->end() } -> std::random_access_iterator;
    };

    Fr innerProduct(Fr::FrListPtr const& a, Fr::FrListPtr const& b);

    template <IsSharedVector T>
    void splitSlice(T a, T& out1, T& out2);

    void commit(Element::ElementListPtr groupElements, Fr::FrListPtr polynomial);
}