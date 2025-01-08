#pragma once

#include <iostream>
#include <vector>

namespace pipeline_simulator {

class Clockable {
public:
    virtual void clock_start() = 0;
    virtual void clock_end() = 0;
};

class Clock {
public:
    using Cycles = unsigned;

    Clock() : cycle_counter(0) {}
    void tick() { 
        if(logged) {
            std::cout << "Clock start!" << std::endl;
        }
        for (auto *clockable : clockableObjects)
            clockable->clock_start();
        if(logged) {
            std::cout << "Clock end!" << std::endl;
        }
        for (auto *clockable : clockableObjects)
            clockable->clock_end();
        ++cycle_counter;
    }
    void registerClockable(Clockable* clockable) {
        clockableObjects.push_back(clockable);
    }
    Cycles get_cycle_count() const {
        //
        return cycle_counter + 4;
    }
    
    void skip_cycle(unsigned skipped_cycle) {
        cycle_counter += skipped_cycle;
    }
private:
    std::vector<Clockable *> clockableObjects;
    unsigned cycle_counter;
    bool logged = false;
};

} // namespace pipeline_simulator