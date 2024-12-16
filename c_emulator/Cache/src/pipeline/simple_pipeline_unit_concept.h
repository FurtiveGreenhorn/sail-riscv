#pragma once

#include "../Instruction/instruction.h"
#include <memory>
#include <vector>
#include <iostream>


template<typename DerivedT, typename DataT = Instruction>
class SimplePipelineStageLogicMixIn {
public:
    SimplePipelineStageLogicMixIn() : data(nullptr) {
        if(logged) {
            std::cout << "stage created!" << std::endl;
        }
    }
    // receive data from previous pipeline register
    void receive(Instruction *input_data) {
        if(logged) {
            static_cast<DerivedT&>(*this).print_name();
            std::cout << " receive !" << std::endl;
        }
        data = input_data;
        static_cast<DerivedT&>(*this).process_stage();
    }   
    // default implementation of process_stage
    void process_stage() {
        // default does nothing
    }
    // data flow out to next pipeline register
    DataT *flow_out() {
        if(logged) {
            static_cast<DerivedT&>(*this).print_name();
            std::cout << " flow out !" << std::endl;
        }
        DataT *data_out = data;
        data = nullptr;
        return data_out;
    }
protected:     
    DataT *data;
    bool logged = false;
};

class Clockable {
public:
    virtual void clock_start() = 0;
    virtual void clock_end() = 0;
};

class Clock {
public:
    Clock() : cycle_counter(0) {}
    void tick() {
        for (auto *clockable : clockableObjects)
            clockable->clock_start();
        for (auto *clockable : clockableObjects)
            clockable->clock_end();
        ++cycle_counter;
    }
    void registerClockable(Clockable* clockable) {
        clockableObjects.push_back(clockable);
    }
    unsigned get_cycle_count() {
        return cycle_counter;
    }
private:
    std::vector<Clockable *> clockableObjects;
    unsigned cycle_counter;
};


class NoStage : public SimplePipelineStageLogicMixIn<NoStage> {
public:
    void print_name() {
        std::cout << "NoStage";
    }
};

template<typename DerivedT, typename PreviousStage = NoStage, 
         typename NextStage = NoStage, typename DataT = Instruction>
class SimplePipelineRegMixin : public Clockable {
public:    
    SimplePipelineRegMixin(Clock &clk, PreviousStage *prevStage, NextStage *nextStage)
        : previous_stage(prevStage), next_stage(nextStage), 
          data(nullptr), stallFlag(false) {
        clk.registerClockable(static_cast<DerivedT*>(this));
        if(logged) {
            std::cout << "pipeline register created!" << std::endl;
        }
    }
    void clock_start() override {
        if(logged) {
            std::cout << "clock start!" << std::endl;
        }
        // transmit data to next stage
        if (stallFlag == true) {
            stallFlag = false;
            return;
        }
        // Supports read and write enables for 
        // controlling data transfer, with customizable behavior
        static_cast<DerivedT&>(*this).transmit();
    }
    void clock_end() override {
        if(logged) {
            std::cout << "clock end!" << std::endl;
        }
        // Supports read and write enables for 
        // controlling data transfer, with customizable behavior
        static_cast<DerivedT&>(*this).accept();
    }
    void receive_stall() {
        stallFlag = true;
    }

    // accept(): accept data from previous stage
    void accept() {
        if(logged) {
            static_cast<DerivedT&>(*this).print_name();
            std::cout << " accept !" << std::endl;
        }
        if (DataT *data_in = previous_stage->flow_out(); data_in != nullptr)
            data = data_in;
    }
    // transmit(): transmit data to next stage
    void transmit() {
        if(logged) {
            static_cast<DerivedT&>(*this).print_name();
            std::cout << " transmit !" << std::endl;
        }
        if (data != nullptr)
            next_stage->receive(data);
    }
    
protected:
    PreviousStage *previous_stage;
    NextStage *next_stage;
    DataT *data = nullptr;
    bool stallFlag = false;
    bool logged = false;
};