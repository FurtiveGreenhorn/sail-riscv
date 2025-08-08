#pragma once

#include <array>
#include <optional>
#include <cstdint>
#include "../cache/replacement_policy/Tree-PLRU.h"

namespace pipeline_simulator {

/// 預設壓縮 tag 函數：BTB-X 壓縮方案
template<typename PCType>
uint16_t default_tag_compression(PCType full_tag) {
    uint8_t low8 = static_cast<uint8_t>(full_tag & 0xFF);
    uint8_t high8 = 0;

    constexpr int tag_bits = sizeof(PCType) * 8;
    for (int i = 8; i < tag_bits; i += 8) {
        high8 ^= static_cast<uint8_t>((full_tag >> i) & 0xFF);
    }

    return static_cast<uint16_t>((high8 << 8) | low8);
}

/// BTB 設定模板
template<
    std::size_t NumSets_,
    std::size_t NumWays_,
    typename PCType_ = uint64_t,
    typename TagType_ = uint16_t,
    template<std::size_t, std::size_t> class ReplacementPolicy_ = TreePlru,
    TagType_ (*CompressFunc_)(PCType_) = default_tag_compression<PCType_>
>
struct BTBParams {
    static constexpr std::size_t NumSets = NumSets_;
    static constexpr std::size_t NumWays = NumWays_;
    using PCType = PCType_;
    using TagType = TagType_;
    using Replacement = ReplacementPolicy_<NumSets, NumWays>;
    static constexpr auto CompressFunc = CompressFunc_;
};

/// 主體 BTB 模板
template<typename Params>
class BTB {
public:
    using PCType = typename Params::PCType;
    using TagType = typename Params::TagType;
    using ReplacementPolicy = typename Params::Replacement;

    BTB() = default;

    std::optional<PCType> lookup(PCType pc);
    void update(PCType pc, PCType target);
    void dump() const;

private:
    struct BTBEntry {
        TagType tag = 0;
        PCType target = 0;
        bool valid = false;
    };

    using SetT = std::array<BTBEntry, Params::NumWays>;
    std::array<SetT, Params::NumSets> sets;
    ReplacementPolicy replacement;

    std::size_t index(PCType pc) const;
    TagType tag(PCType pc) const;
    static constexpr std::size_t log2_constexpr(std::size_t n);
};

// ===== Implementation =====

template<typename Params>
std::optional<typename Params::PCType>
BTB<Params>::lookup(PCType pc) {
    std::size_t set_idx = index(pc);
    TagType pc_tag = tag(pc);

    for (std::size_t way = 0; way < Params::NumWays; ++way) {
        auto& entry = sets[set_idx][way];
        if (entry.valid && entry.tag == pc_tag) {
            replacement.touch(set_idx, way);
            return entry.target;
        }
    }
    return std::nullopt;
}

template<typename Params>
void BTB<Params>::update(PCType pc, PCType target) {
    std::size_t set_idx = index(pc);
    TagType pc_tag = tag(pc);

    for (std::size_t way = 0; way < Params::NumWays; ++way) {
        auto& entry = sets[set_idx][way];
        if (entry.valid && entry.tag == pc_tag) {
            entry.target = target;
            replacement.touch(set_idx, way);
            return;
        }
    }

    std::size_t victim = replacement.get_victim(set_idx);
    auto& entry = sets[set_idx][victim];
    entry.valid = true;
    entry.tag = pc_tag;
    entry.target = target;
    replacement.touch(set_idx, victim);
}

template<typename Params>
std::size_t BTB<Params>::index(PCType pc) const {
    return static_cast<std::size_t>((pc >> 2) & (Params::NumSets - 1));
}

template<typename Params>
typename BTB<Params>::TagType BTB<Params>::tag(PCType pc) const {
    constexpr std::size_t tag_shift = 2 + log2_constexpr(Params::NumSets);
    PCType full_tag = pc >> tag_shift;
    return Params::CompressFunc(full_tag);
}

template<typename Params>
constexpr std::size_t BTB<Params>::log2_constexpr(std::size_t n) {
    return (n < 2) ? 0 : 1 + log2_constexpr(n / 2);
}

} // namespace pipeline_simulator
