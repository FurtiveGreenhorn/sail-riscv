#pragma once

#include <array>
#include <cstdint>

namespace pipeline_simulator {

template<typename PCType, std::size_t Size = 64>
class BHT {
public:
    BHT() {
        predictor_table.fill(1); // initial: weak not taken
    }

    bool predict(PCType pc) const {
        std::size_t index = (pc >> 2) & (Size - 1);
        return predictor_table[index] >= 2;
    }

    void update(PCType pc, bool taken) {
        std::size_t index = (pc >> 2) & (Size - 1);
        auto& counter = predictor_table[index];
        if (taken && counter < 3) ++counter;
        if (!taken && counter > 0) --counter;
    }

private:
    std::array<uint8_t, Size> predictor_table;
};

} // namespace pipeline_simulator
