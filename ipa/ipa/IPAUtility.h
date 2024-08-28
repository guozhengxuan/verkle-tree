#pragma once
#include "bandersnatch/Fr.h"
#include "bandersnatch/Element.h"

using verkle::bandersnatch::Fr;
using verkle::bandersnatch::Element;

namespace verkle::ipa
{
    template <typename T>
    concept Splitable = requires(T a)
    {
        { a->size() } -> std::convertible_to<std::size_t>;
        { a->begin() } -> std::random_access_iterator;
        { a->end() } -> std::random_access_iterator;
    };

    Fr innerProduct(Fr::FrListPtr const& a, Fr::FrListPtr const& b);

    template <Splitable T>
    void split(T a, T& out1, T& out2);

    Element commit(Element::ElementListPtr groupElements, Fr::FrListPtr polynomial);

    Fr::FrListPtr foldScalars(Fr::FrListPtr const& a, Fr::FrListPtr const& b, Fr const& x);

    Element::ElementListPtr foldPoints(
        Element::ElementListPtr const& a,
        Element::ElementListPtr const& b,
        Fr const& x);
}