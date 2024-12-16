#include "../Instruction/instruction.h"
#include <memory>
#include <vector>


template<typename DerivedT, typename DataT = Instruction>
class SimplePipelineStageLogicMixIn {
public:
    SimplePipelineStageLogicMixIn() : data(nullptr) {}
    // recieve data from previous pipeline register
    void recieve(Instruction *input_data) {
        data = input_data;
        static_cast<DerivedT&>(*this).process_stage();
    }   
    // default implementation of process_stage
    void process_stage() {
        // default does nothing
    }
    // data flow out to next pipeline register
    DataT *flow_out() {
        DataT *data_out = data;
        data = nullptr;
        return data_out;
    }
protected:     
    DataT *data;
};

class NoStage {
    public:
    void flow_out() {}
    void receive(void* data) {}
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

template<typename DerivedT, typename PreviousStage = NoStage, 
         typename NextStage = NoStage, typename DataT = Instruction>
class SimplePipelineRegMixin : Clockable {
public:    
    SimplePipelineRegMixin(Clock &clk, PreviousStage *prevStage, NextStage *nextStage)
        : previous_stage(prevStage), next_stage(nextStage), 
          data(nullptr), stallFlag(false) {
        clk.registerClockable(static_cast<DerivedT&>(*this));
    }
    void clock_start() {
        // transmit data to next stage
        if (stallFlag == true) {
            stallFlag = false;
            return;
        }
        // Supports read and write enables for 
        // controlling data transfer, with customizable behavior
        static_cast<DerivedT&>(*this).transmit();
    }
    void clock_end() {
        // Supports read and write enables for 
        // controlling data transfer, with customizable behavior
        static_cast<DerivedT&>(*this).accept();
    }
    void receive_stall() {
        stallFlag = true;
    }
    // accept data from previous stage
    void accept() {
        if (DataT *data_in = previous_stage->flow_out(); data_in != nullptr)
            data = data_in;
    }
    // transmit data to next stage
    void transmit() {
        next_stage->recieve(data);
    }
    
protected:
    PreviousStage *previous_stage;
    NextStage *next_stage;
    DataT *data = nullptr;
    bool stallFlag = false;
};