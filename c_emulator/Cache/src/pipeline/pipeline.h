#include "../Instruction/instruction.h"
#include <memory>
#include <vector>

// PipelineStageLogic 類模板，模擬處理器流水線中的一個階段
// 此類型是流水線的一部分，每個階段負責處理特定的任務
// 可用於實現簡單的順序流水線（In-order pipeline）或複雜的亂序超純量處理器（Out-of-order superscalar processor）
template<typename PipelineBuffer, typename StageInput, typename StageOutput>
class PipelineStageLogicConcept {
public:
    /**
     * 接收資料的方法。此方法用於從流水線前一階段接收資料。
     * 在每個階段，資料可以是指令、數據或其他處理結果。
     * 
     * 在簡單的順序流水線中，這通常是單一值（例如：`PipelineBuffer = 1`），
     * 但在更複雜的亂序處理中，這可能涉及多個資料單元，且資料可能會重排序或延遲處理。
     * 
     * @param input 指向接收到的資料的指標，通常來自上一階段的處理結果。
     */
    virtual void receive(StageInput *input) = 0;

    /**
     * 傳送資料的方法。將當前階段處理完的資料傳遞到流水線的下一個階段。
     * 這個資料將被封裝在 `PipelineBuffer` 中，`PipelineBuffer` 可用來表示任意數量的資料單元，
     * 在普通的順序流水線中，這只是一個簡單的單一值（例如：`PipelineBuffer = 1`），
     * 但在複雜的超純量處理器中，它可以容納多個並行的資料單元。
     * 
     * @param buffer 封裝資料的緩衝區，代表當前階段處理後的結果。這個緩衝區可以包含多個資料單元（例如：多條指令或多個數據），
     *               也可能只是單一的資料值。
     * @param output 指向處理結果資料的指標，準備傳遞給下一階段。
     */
    virtual void transmit(PipelineBuffer& buffer, StageOutput *output) = 0;
};






class PipelineBuffer {

};

// Classic five stage pipeline: inorder & simplescalar
template<typename Derived, typename OutputUnit, typename Buffer = Instruction>
class SimplePipelineUnitMixin : public Tickable {
public:
    SimplePipelineUnitMixin(Clock& clock, OutputUnit& out) : 
        clock(&clock), output_unit(&out), inst(nullptr) {
        clock.registerUnit(derived());
    }
    Derived& derived() { 
        return static_cast<Derived&>( *this );
    }
    Derived const& derived() const { 
        return static_cast<Derived const&>( *this );
    }
    void clockTicked() {
        // if (ControlUnit::stall(OutputUnit::ID()))
        transmit(inst);
        // then receive will be called by pipeline unit whose OutputUnit is this
    }
    void receive(const Buffer *in) {
        inst = in;
    }
    const Buffer * const getInst() const {
        return inst;
    }
protected:
    const Clock *clock;
    OutputUnit *output_unit;
    const Buffer *inst;

    void transmit(const Buffer *out) {
        Derived().recieve(out);
    }
};




// class Tickable {
// public:
//     virtual void clockTicked() = 0;
// };

// class Clock {
// public:
//     void registerUnit(Tickable *unit) {
//         units.push_back(unit);
//     }
//     void tick() {
//         for (auto& unit : units) {
//             unit->clockTicked();
//         }
//     }
// private:
//     std::vector<Tickable *> units;
// };

// class ControlUnit {
// public:
//     void registerUnit();
//     void setStallCycles(Stage_type stage, unsigned num) {
//         stall_stage = stage;
//         stall_cycle_count = num;
//     }
//     bool stall(Stage_type stage) {
//         if ((stage != stall_stage) || (stall_cycle_count == 0))
//             return false;
//         --stall_cycle_count;
//         return true;
//     }
//     unsigned skip_cycle() {
//         unsigned skip = stall_cycle_count;
//         stall_cycle_count = 0;
//         return skip;
//     }
// private:
//     Stage_type stall_stage;
//     unsigned stall_cycle_count;
// };

// // Classic five stage pipeline: inorder & simplescalar
// template<typename Derived, typename OutputUnit, typename Buffer = Instruction>
// class SimplePipelineUnitMixin : public Tickable {
// public:
//     SimplePipelineUnitMixin(Clock& clock, OutputUnit& out) : 
//         clock(&clock), output_unit(&out), inst(nullptr) {
//         clock.registerUnit(derived());
//     }
//     Derived& derived() { 
//         return static_cast<Derived&>( *this );
//     }
//     Derived const& derived() const { 
//         return static_cast<Derived const&>( *this );
//     }
//     void clockTicked() {
//         // if (ControlUnit::stall(OutputUnit::ID()))
//         transmit(inst);
//         // then receive will be called by pipeline unit whose OutputUnit is this
//     }
//     void receive(const Buffer *in) {
//         inst = in;
//     }
//     const Buffer * const getInst() const {
//         return inst;
//     }
// protected:
//     const Clock *clock;
//     OutputUnit *output_unit;
//     const Buffer *inst;

//     void transmit(const Buffer *out) {
//         Derived().recieve(out);
//     }
// };


// Classic five stage pipeline: inorder & simplescalar
// FixMe: Not only one output for each unit.
// template<typename Derived, typename OutputUnit, typename Buffer>
// class SimplePipelineUnit {
// protected:
//    ~SimplePipelineUnit() = default;
// public:
//     Derived& derived() { 
//         return static_cast<Derived&>( *this );
//     }
//     Derived const& derived() const { 
//         return static_cast<Derived const&>( *this );
//     }
//     bool clockTicked() {
//         Derived().clockTicked();
        // transmit(instruction_being_processed);
        // then receive will be called by pipeline unit whose OutputUnit is this
        // could be false in out-of-order pipeline
//     }
//     bool receive(Buffer in) {
//         Derived().receive(in);
//     }
//     bool transmit(Buffer out) {
//         Derived().recieve(out);
//     }
// };