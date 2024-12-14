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
        return data;
    }
    void flush() {
        data = nullptr;
    }
protected:     
    DataT *data;
};

template<typename DerivedT, typename PreviousStage, 
         typename NextStage, typename DataT = Instruction>
class SimplePipelineRegMixin {
public:    
    SimplePipelineRegMixin(PreviousStage* prevStage, NextStage* nextStage)
        : previous_stage(prevStage), next_stage(nextStage), 
          data(nullptr), stallFlag(false) {}

    void clockTicked() {
        // transmit data to next stage
        if (stallFlag == true) {
            stallFlag = false;
            return;
        }
        // Supports read and write enables for 
        // controlling data transfer, with customizable behavior
        static_cast<DerivedT&>(*this).transmit();
        static_cast<DerivedT&>(*this).accept();
    }
    void stall() {
        stallFlag = true;
    }
    // accept data from previous stage
    void accept() {
        if (previous_stage == nullptr)
            return;
        Instruction *data = previous_stage->flow_out();
    }
    // transmit data to next stage
    void transmit() {
        if(next_stage == nullptr)
            return;
        next_stage->recieve(data);
    }
    void flush() {
        data = nullptr;
    }

protected:
    PreviousStage *previous_stage;
    NextStage *next_stage;
    DataT *data = nullptr;
    bool stallFlag = false;
};