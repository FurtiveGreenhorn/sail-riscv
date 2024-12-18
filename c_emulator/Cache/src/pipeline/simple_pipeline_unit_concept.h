#pragma once

#include "../Instruction/instruction.h"
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <cassert>

template<typename DerivedT, typename DataT = Instruction>
class SimplePipelineStageLogicMixIn {
public:
    SimplePipelineStageLogicMixIn() : data(nullptr) {}
    // receive data from previous pipeline register
    void receive(Instruction *input_data) {
        assert(input_data != nullptr);
        assert(input_data->is_used == true);
        data = input_data;
        if(logged) {
            std::cout << name << " received " 
                      << (data->is_bubble() ? "a bubble" : "data")
                      << " !" << std::endl;
        }
        if(logged) {
            std::cout << name 
                << " is processing !" << std::endl;
        }        
        static_cast<DerivedT&>(*this).process_stage();     
    }   
    // default implementation of process_stage
    void process_stage() {
        // Default implementation does nothing
    }
    // data flow out to next pipeline register
    DataT *flow_out() {
        if (data == nullptr) {
            if(logged) {
                std::cout << "No data flowed out from "
                          << name << " !" << std::endl;
            }
            return nullptr;
        }
        if(logged) {
            std::cout << (data->is_bubble() ? "Bubble" : "data")
                      << " flowed out from "
                      << name << " !" << std::endl;
        }
        DataT *data_out = data;
        data = nullptr;
        return data_out;
    }

    const std::string& get_name() {
        return name;
    }
protected:     
    DataT *data;
    std::string name;
    bool logged = true;
};

class ClockableConcept {
public:
    virtual ~ClockableConcept() = default;
    virtual void clock_start() = 0;
    virtual void clock_end() = 0;
};


class NoStage : public SimplePipelineStageLogicMixIn<NoStage> { };

template<typename DerivedT, typename PreviousStage = NoStage, 
         typename NextStage = NoStage, typename DataT = Instruction>
class ClockableModel : public ClockableConcept {
public:
    ClockableModel(DerivedT &&reg) : reg(reg) {}
    void clock_start() override {
        reg.clock_start();
    }
    void clock_end() override {
        reg.clock_end();
    }
private:
    DerivedT reg;
};

class Clock {
public:
    Clock() : cycle_counter(0) {}
    void tick() { 
        if(logged)
            std::cout << "Clock start!" << std::endl;
        for (auto &clockable : clockableObjects)
            clockable->clock_start();
        if(logged)
            std::cout << "Clock end!" << std::endl;
        for (auto &clockable : clockableObjects)
            clockable->clock_end();
        ++cycle_counter;
    }
    void registerClockable(std::unique_ptr<ClockableConcept>&& clockable) {
        clockableObjects.push_back(std::move(clockable));
    }
    unsigned get_cycle_count() {
        return cycle_counter;
    }
private:
    std::vector<std::unique_ptr<ClockableConcept>> clockableObjects;
    unsigned cycle_counter;
    bool logged = true;
};

template<typename DerivedT, typename PreviousStage = NoStage, 
         typename NextStage = NoStage, typename DataT = Instruction>
class SimplePipelineRegMixin : public ClockableConcept {
public:
    SimplePipelineRegMixin(Clock &clk, 
        PreviousStage *prevStage = nullptr, NextStage *nextStage = nullptr)
        : previous_stage(prevStage), next_stage(nextStage), 
          data(nullptr), stallFlag(false) {
        using ClockableModelT = ClockableModel<DerivedT, PreviousStage, NextStage, DataT>;
        clk.registerClockable( 
            std::unique_ptr<ClockableConcept>(
                new ClockableModelT(std::move(static_cast<DerivedT&>(*this)))));
    }
    void clock_start() override {
        if (stallFlag == true)
            stallFlag = false;
        // transmit data to next stage
        // Supports read and write enables for 
        // controlling data transfer, with customizable behavior
        static_cast<DerivedT&>(*this).transmit();
    }
    void clock_end() override {
        // Supports read and write enables for 
        // controlling data transfer, with customizable behavior
        std::cout << typeid(DerivedT&).name() << "->clock_end !\n";
        static_cast<DerivedT&>(*this).accept();
        static_cast<DerivedT&>(*this).after_accept();
    }
    void receive_stall() {
        if (logged) {
            std::cout << name << " received a stall signal !" << std::endl;
        }
        stallFlag = true;
    }

    // accept(): accept data from previous stage
    void accept() {
        if (stallFlag == true) {
            if (logged) {
                std::cout << name << " is stalled !" << std::endl;
            }
            return;
        }
        if (previous_stage == nullptr)
            return;
        DataT *data_in = previous_stage->flow_out();
        if (data_in == nullptr) {
            if (logged) {
                std::cout << name 
                        << " did not accepted data from " 
                        << previous_stage->get_name() << std::endl;
            }
            return;
        }
        assert(data_in->is_used == true);
        data = data_in;
        if (logged) {
            std::cout << name 
                      << " accepted "
                      << (data->is_bubble() ? "a bubble " : "data ")
                      << "from " 
                      << previous_stage->get_name() << std::endl;
        }        
    }
    void after_accept() {
        // Default implementation does nothing
        std::cout << "wwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwwww\n";
    }
    // transmit(): transmit data to next stage
    void transmit() {
        if (next_stage == nullptr)
            return;
        if (data == nullptr) {
            if(logged) {
                std::cout << name 
                          << " has no data to transmited to " 
                          << next_stage->get_name() << " !" << std::endl;
            }
            return;
        }
        if(logged) {
            // std::cout << name << " transmited data to " 
            //           << next_stage->get_name() << " !" << std::endl;
            // for debug
            std::cout << name << " transmited " << "Inst[" << data->debugNo << "]" <<" to " 
                    << next_stage->get_name() << " !" << std::endl;
        }
        next_stage->receive(data);
    }
    
protected:
    PreviousStage *previous_stage;
    NextStage *next_stage;
    DataT *data = nullptr;
    bool stallFlag = false;
    std::string name;
    bool logged = true;
};