#include "IPAUtility.h"

namespace verkle::ipa
{
    Fr::FrListPtr powers(Fr const& x, size_t degree)
    {
        auto res = std::make_shared<std::vector<Fr>>(degree);
        res->at(0) = Fr::one();

        for (size_t i = 1; i < degree; ++i)
        {
            res->at(i) = res->at(i-1) * x;
        }
        return res;
    }

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

    Element commit(Element::ElementListPtr const& groupElements, Fr::FrListPtr const& polynomial)
    {
        if (groupElements->size() != polynomial->size()) {
            throw std::runtime_error("group elements and polynomial are different sizes, " +
                std::to_string(groupElements->size()) + " != " + std::to_string(polynomial->size()));
        }

        return Element::msm(groupElements, polynomial);
    }

    Fr::FrListPtr foldScalars(Fr::FrListPtr const& a, Fr::FrListPtr const& b, Fr const& x)
    {
        if (a->size() != b->size()) {
            throw std::runtime_error("a and b are different sizes, " +
                std::to_string(a->size()) + " != " + std::to_string(b->size()));
        }
        auto ret = std::make_shared<std::vector<Fr>>(a->size());
        for (size_t i = 0; i < a->size(); ++i)
        {
            ret->at(i) = a->at(i) + b->at(i) * x;
        }
        return ret;
    }

    Element::ElementListPtr foldPoints(
        Element::ElementListPtr const& a,
        Element::ElementListPtr const& b,
        Fr const& x)
    {
        if (a->size() != b->size()) {
            throw std::runtime_error("a and b are different sizes, " +
                std::to_string(a->size()) + " != " + std::to_string(b->size()));
        }
        auto ret = std::make_shared<std::vector<Element>>(a->size());
        for (size_t i = 0; i < a->size(); ++i)
        {
            ret->at(i) = Element::add(a->at(i), Element::mult(x, b->at(i)));
        }
        return ret;
    }

    Element::ElementListPtr generateRandomPoints(size_t numPoints)
    {
        auto const g = Element::generator();
        auto ret = std::make_shared<std::vector<Element>>(numPoints);
        for (size_t i = 0; i < numPoints; ++i)
        {
            auto randomFr = Fr::random();
            ret->at(i) = Element::mult(randomFr, g);
        }
        return ret;
    }

    size_t computeNumRounds(size_t vectorSize)
    {
        if (vectorSize == 0)
        {
            throw std::runtime_error("zero is not a valid input");
        }

        if ((vectorSize & (vectorSize - 1)) != 0)
        {
            throw std::runtime_error("non power of 2 numbers are not valid inputs");
        }

        size_t ret = std::floor(std::log2(static_cast<double>(vectorSize)));
        return ret;
    }
}
