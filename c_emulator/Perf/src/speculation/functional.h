#pragma once
#include <cstdint>

namespace pipeline_simulator {

using RegNum = unsigned;

class Speculation {
    class RegisterStatus {
    public:
        RegisterStatus() : usage_bitmap(0) {}
        void mark_used(RegNum num) {
            usage_bitmap |= (1U << num);
        }
        bool is_used(RegNum num) const {
            return usage_bitmap & (1U << num);
        }
        void flush() {
            usage_bitmap = 0;
        }
    private:
        uint32_t usage_bitmap;
    };
public:
    bool is_in_speculation_mode() {
        return speculation_active;
    }
    void enter_speculation_mode() {
        speculation_active = true;
    }
    void exit_speculation_mode() {
        reg_stat.flush();
        speculation_active = false;
    }
    uint64_t read(RegNum num) {
        return x[num];
    }
    void write(RegNum num, uint64_t val) {
        reg_stat.mark_used(num);
        x[num] = val;
    }
    bool is_used(RegNum num) {
        return reg_stat.is_used(num);
    }
private:
    uint64_t x[32];
    RegisterStatus reg_stat;
    bool speculation_active;
};

} // namespace pipeline_simulator