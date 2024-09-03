#pragma once
#include "bandersnatch/Fr.h"
#include "bandersnatch/Element.h"

using verkle::bandersnatch::Fr;
using verkle::bandersnatch::Element;

namespace verkle::ipa
{
    Fr::FrListPtr powers(Fr const& x, size_t degree);

    Fr innerProduct(Fr::FrListPtr const& a, Fr::FrListPtr const& b);

    Element commit(Element::ElementListPtr const& groupElements, Fr::FrListPtr const& polynomial);

    Fr::FrListPtr foldScalars(Fr::FrListPtr const& a, Fr::FrListPtr const& b, Fr const& x);

    Element::ElementListPtr foldPoints(
        Element::ElementListPtr const& a,
        Element::ElementListPtr const& b,
        Fr const& x);

    Element::ElementListPtr generateRandomPoints(size_t numPoints);

    size_t computeNumRounds(size_t vectorSize);

    size_t constexpr window16vs8IndexLimit = 5;
    size_t constexpr supportedMSMLength = 256;

    template <typename T>
    concept Splitable = requires(T a)
    {
        { a->size() } -> std::convertible_to<std::size_t>;
        { a->begin() } -> std::random_access_iterator;
        { a->end() } -> std::random_access_iterator;
    };

    template <Splitable T>
    void split(T a, T& out1, T& out2)
    {
        using InnerType = typename T::element_type;

        if (a->size()%2 != 0)
        {
            throw std::runtime_error("scalars or elements should have even length");
        }

        auto mid = a->size() / 2;

        InnerType first(mid);
        InnerType second(a->size() - mid);

        std::move(a->begin(), a->begin() + mid, first.begin());
        std::move(a->begin() + mid, a->end(), second.begin());

        out1 = std::make_shared<InnerType>(std::move(first));
        out2 = std::make_shared<InnerType>(std::move(second));
    }

}