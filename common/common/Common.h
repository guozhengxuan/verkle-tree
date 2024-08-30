//
// Created by Zhengxuan Guo on 2024/8/28.
//
#pragma once
#include <cstddef>
#include <cstdint>
#include <memory>

namespace verkle::common
{
    constexpr size_t vectorLength = 256;

    using byte = uint8_t;
    using bytes = std::vector<byte>;
    using bytesPtr = std::shared_ptr<std::vector<byte>>;

}