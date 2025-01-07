#pragma once

#include <type_traits>
#include <cstdint>
#include <iomanip>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include "replacement_policy/Tree-PLRU.h"
#include "../stall_policy.h"

class CacheConcept {
public:
    virtual unsigned access(uint64_t addr, 
                            bool is_write = false) = 0;
    virtual void set_next_level_cache(CacheConcept *nlc) = 0;
    virtual void show(const std::string& name) = 0;
    virtual ~CacheConcept() = default;
};

// FixMe: 
// Temporary implementation of memory to simulate main memory latency.
// This is a placeholder and should be replaced with a proper implementation.
class Memory : public CacheConcept {
public:
    Memory(unsigned cycles) : cycles(cycles) {}
    unsigned access(uint64_t addr, bool is_write) override {
        return 10;
    }
    void set_next_level_cache(CacheConcept *nlc) override {};
    void show(const std::string& name) override {};
private:
    unsigned cycles;
};

template<
    std::size_t NumSets,
    std::size_t NumWays,
    std::size_t LineSize,
    template<std::size_t, std::size_t> 
    class ReplacementPolicy = TreePlru
>
struct CacheParams {
    static constexpr std::size_t num_sets = NumSets;
    static constexpr std::size_t num_ways = NumWays;
    static constexpr std::size_t line_size = LineSize;

    static constexpr std::size_t offset_bits = 
        __builtin_ctz(LineSize);
    static constexpr std::size_t tag_mask = 
        -1ULL << offset_bits << __builtin_ctz(num_sets);
        
    using replacement_policy = ReplacementPolicy<NumSets, NumWays>;
};

template<typename Params>
class Cache : public CacheConcept {
public:
    struct CacheInfo {
        CacheInfo() : cache_info(0) {}
        uint64_t cache_info;

        static constexpr std::size_t valid_mask = 
            1ULL << Params::offset_bits;
        static constexpr std::size_t dirty_mask = 
            1ULL << (Params::offset_bits + 1);
        
        bool is_valid() const {
            return cache_info & valid_mask;
        }
        bool is_dirty() const {
            return cache_info & dirty_mask;
        }
        void set_valid() {
            cache_info |= valid_mask;
        }
        void set_dirty() {
            cache_info |= dirty_mask;
        }
        void set_tag(uint64_t tag) {
            cache_info = tag;
        }
        bool cache_hit(uint64_t tag) {
            return tag == (cache_info & Params::tag_mask);
        }
        uint64_t get_addr(std::size_t set_index) {
            return cache_info & (set_index << Params::offset_bits);
        }
    };
    using WaysT = std::vector<CacheInfo>;
    using SetsT = std::vector<WaysT>;
    using ReplacementPolicy = typename Params::replacement_policy;
    struct CacheLog {
        CacheLog(): 
            read_access_count(0),
            read_miss_count(0),
            write_access_count(0),
            write_miss_count(0),
            writeback_count(0) {}

        void show(const std::string& name);
        unsigned read_access_count;
        unsigned read_miss_count;
        unsigned write_access_count;
        unsigned write_miss_count;
        unsigned writeback_count;
    };
private:
    SetsT sets;
    CacheLog cache_log;
    unsigned hit_cycles;
    ReplacementPolicy repl_policy;
    CacheConcept *next_level_cache;

    std::size_t get_set_index(uint64_t addr) {
        static auto set_mask = Params::num_sets - 1;
        return (addr >> Params::offset_bits) & set_mask;
    }
    ;
    CacheInfo *find(std::size_t set_index, uint64_t addr) {
        WaysT &ways = sets[set_index]; 
        for (int way_index = 0; way_index != Params::num_ways; ++way_index) {
            CacheInfo &cache_info = ways[way_index];
            if(cache_info.is_valid() && cache_info.cache_hit(addr)) {
                repl_policy.touch(set_index, way_index);
                return &cache_info;
            }
        }
        return nullptr;
    }

public:
    Cache(unsigned hit_cycles = 0) :
          sets(Params::num_sets, WaysT(Params::num_ways)), 
          hit_cycles(hit_cycles) {}

    unsigned access(uint64_t addr, 
                    bool is_write = false) override;
                    
    void set_next_level_cache(CacheConcept *nlc) override {
        next_level_cache = nlc;
    }
    void show(const std::string& name) override {
        cache_log.show(name);
    }
};

template<typename Params>
unsigned Cache<Params>::
access(uint64_t addr, bool is_write) {
    is_write ? ++cache_log.write_access_count :
               ++cache_log.read_access_count;
    
    auto set_index = get_set_index(addr);
    auto tag = addr & Params::tag_mask;

    if(auto *cache_info = find(set_index, tag);
       cache_info != nullptr) {
        if (is_write)
            cache_info->set_dirty();
        return hit_cycles;
    }

    is_write ? ++cache_log.write_miss_count :
               ++cache_log.read_miss_count;

    auto victim_way = repl_policy.get_victim(set_index);
    CacheInfo &victim = sets[set_index][victim_way];

    if (victim.is_valid() && victim.is_dirty()) {
        if (next_level_cache != nullptr)
            next_level_cache->access(victim.get_addr(set_index), true);
        ++cache_log.writeback_count;
    }

    // FixMe: This might be skipped in lazy-write merge policies.
    unsigned cycles = hit_cycles;
    if (next_level_cache != nullptr) {
        cycles += next_level_cache->access(addr);
    }

    victim.set_tag(tag);
    victim.set_valid();
    if (is_write)
        victim.set_dirty();

    return cycles;
}

template<typename Params>
void Cache<Params>::CacheLog::show(const std::string& name) {
    std::cout << std::setprecision(4) << std::fixed;
    auto get_miss_percentage = 
        [](unsigned miss_count, unsigned access_count) {
            return 100.0 * miss_count / access_count;
        };
        unsigned miss_count = read_miss_count + write_miss_count,
                 access_count = read_access_count + write_access_count;
    std::cout << "--------------"<< name <<"--------------" << std::endl
                << miss_count << " misses / "
                << access_count << " accesses" << std::endl
                << "miss rate: "
                << get_miss_percentage(miss_count, access_count) 
                << '%' << std::endl  
                << writeback_count << " writebacks / "
                << access_count << " accesses" << std::endl
                << "writebacks rate: "
                << get_miss_percentage(writeback_count, access_count)
                << '%' <<std::endl << std::endl;
    std::cout << "               Detail             " << std::endl;
    std::cout << ">--------------Read--------------<" << std::endl
                << read_miss_count << " misses / "
                << read_access_count << " accesses" << std::endl
                << "miss rate: "
                << get_miss_percentage(read_miss_count, read_access_count) 
                << '%' << std::endl;
    std::cout << ">--------------Write-------------<" << std::endl
                << write_miss_count << " misses / "
                << write_access_count << " accesses" << std::endl
                << "miss rate: "
                << get_miss_percentage(write_miss_count, write_access_count) 
                << '%' << std::endl;
}

template<typename T>
using has_stall = std::is_invocable_r<void, decltype(&T::stall), T, unsigned>;

template<typename Params, 
         typename StallPolicy = SkippedStallCycle>
class L1Cache : public Cache<Params> {
public:
    static_assert(has_stall<StallPolicy>::value, "StallPolicy must implement void stall(unsigned cycles)");

    L1Cache(unsigned hit_cycles = 0, 
            std::unique_ptr<StallPolicy> st_policy = nullptr) :
        stall_policy(std::move(st_policy)) {}
    unsigned access(uint64_t addr, bool is_write = false) override {
        unsigned cycles = Cache<Params>::access(addr, is_write);
        stall_policy->stall(cycles);
        return cycles;
    }
private:
    std::unique_ptr<StallPolicy> stall_policy;
};