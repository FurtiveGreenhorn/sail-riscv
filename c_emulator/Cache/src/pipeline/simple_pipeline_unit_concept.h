#pragma once

#include "../Instruction/instruction.h"
#include <memory>
#include <vector>
#include <iostream>


/**
 * @brief A mix-in class template that provides basic pipeline stage logic.
 *
 * This template is designed to be inherited by a derived pipeline stage class (DerivedT).
 * The derived class should implement:
 * - A `print_name()` method for logging purposes (if logging is enabled).
 * - Optionally override `process_stage()` to perform stage-specific logic.
 *
 * The pipeline stage represented by this class receives data from a preceding pipeline
 * register, processes it, and holds it until the next pipeline register is ready to accept it.
 * Logging behavior can be toggled by setting the `logged` flag.
 *
 * @tparam DerivedT The derived class type inheriting from this mix-in.
 * @tparam DataT    The type of the data to be processed by this stage. Defaults to `Instruction`.
 */
template<typename DerivedT, typename DataT = Instruction>
class SimplePipelineStageLogicMixIn {
public:
    /**
     * @brief Constructs the pipeline stage logic mix-in.
     *
     * Initializes `data` to `nullptr`. If `logged` is set to `true`, the creation
     * of the stage will be logged.
     */
    SimplePipelineStageLogicMixIn() : data(nullptr) {
        if (logged) {
            std::cout << "stage created!" << std::endl;
        }
    }

    /**
     * @brief Receives input data from the previous pipeline register.
     *
     * This method is called by the previous pipeline register to pass data into this stage.
     * It stores the incoming data in `data` and then calls `process_stage()` on the derived class
     * to perform any stage-specific logic. If `logged` is `true`, a message is printed.
     *
     * @param input_data Pointer to the instruction or data object received from the previous stage.
     */
    void receive(Instruction *input_data) {
        if (logged) {
            static_cast<DerivedT&>(*this).print_name();
            std::cout << " receive !" << std::endl;
        }
        data = input_data;
        static_cast<DerivedT&>(*this).process_stage();
    }

    /**
     * @brief Processes the data currently held in this stage.
     *
     * The default implementation does nothing. Derived classes may override this method
     * to implement stage-specific logic such as instruction decoding, execution, or data transformations.
     */
    void process_stage() {
        // default implementation does nothing
    }

    /**
     * @brief Sends the processed data to the next pipeline register.
     *
     * Once the next stage calls `flow_out()`, the data pointer is returned and cleared from this stage.
     * This ensures that the data is no longer held here and is ready to move forward in the pipeline.
     * If `logged` is `true`, a message is printed.
     *
     * @return A pointer to the processed data (`DataT*`), or `nullptr` if no data is available.
     */
    DataT *flow_out() {
        if (logged) {
            static_cast<DerivedT&>(*this).print_name();
            std::cout << " flow out !" << std::endl;
        }
        DataT *data_out = data;
        data = nullptr;
        return data_out;
    }

protected:
    /**
     * @brief A pointer to the data currently held by this pipeline stage.
     *
     * `data` is set when `receive()` is called and cleared when `flow_out()` is invoked.
     */
    DataT *data;

    /**
     * @brief Flag to control logging output.
     *
     * If set to `true`, the stage will print messages when created, receiving data, and flowing data out.
     */
    bool logged = false;
};

/**
 * @brief An interface for objects that operate under a clock signal.
 *
 * The `Clockable` interface defines two pure virtual methods, `clock_start()` and `clock_end()`,
 * which derived classes must implement. These methods are invoked each clock cycle by the `Clock`
 * class, allowing synchronized actions at the start and end of a cycle.
 */
class Clockable {
public:
    /**
     * @brief Invoked at the start of each clock cycle.
     *
     * Derived classes should implement `clock_start()` to define the actions that occur
     * at the beginning of a cycle, such as data transmission or pipeline register updates.
     */
    virtual void clock_start() = 0;

    /**
     * @brief Invoked at the end of each clock cycle.
     *
     * Derived classes should implement `clock_end()` to define actions performed at the
     * end of a cycle, such as accepting new data or finalizing computations before the next cycle.
     */
    virtual void clock_end() = 0;
};

/**
 * @brief A class that simulates a system clock for synchronous components.
 *
 * The `Clock` class maintains a list of `Clockable` objects and advances them through discrete
 * clock cycles. Calling `tick()` triggers `clock_start()` on all registered objects,
 * then `clock_end()` on all registered objects, simulating the behavior of a synchronous pipeline.
 */
class Clock {
public:
    /**
     * @brief Constructs a `Clock` instance with a cycle counter initialized to zero.
     */
    Clock() : cycle_counter(0) {}

    /**
     * @brief Advances the clock by one cycle.
     *
     * When `tick()` is called, it first calls `clock_start()` on all registered `Clockable` objects,
     * then calls `clock_end()` on all of them. After completing these calls, the cycle counter
     * is incremented by one.
     */
    void tick() {
        for (auto *clockable : clockableObjects)
            clockable->clock_start();
        for (auto *clockable : clockableObjects)
            clockable->clock_end();
        ++cycle_counter;
    }

    /**
     * @brief Registers a `Clockable` object with the clock.
     *
     * Once registered, the object’s `clock_start()` and `clock_end()` methods will be called
     * every time `tick()` is invoked, ensuring synchronous progression through clock cycles.
     *
     * @param clockable A pointer to the `Clockable` object to be registered.
     */
    void registerClockable(Clockable* clockable) {
        clockableObjects.push_back(clockable);
    }

    /**
     * @brief Returns the number of clock cycles elapsed.
     *
     * @return An unsigned integer representing the total number of completed cycles.
     */
    unsigned get_cycle_count() {
        return cycle_counter;
    }

private:
    /**
     * @brief A list of pointers to all registered `Clockable` objects.
     *
     * The objects in this vector are all ticked together to maintain synchronous operation.
     */
    std::vector<Clockable *> clockableObjects;

    /**
     * @brief A counter that tracks the number of completed clock cycles.
     */
    unsigned cycle_counter;
};

/**
 * @brief A "no-operation" pipeline stage that implements the `SimplePipelineStageLogicMixIn` interface.
 *
 * `NoStage` acts as a placeholder or terminal stage that can be used when no actual pipeline stage logic is required.
 * It inherits from `SimplePipelineStageLogicMixIn` with itself as the derived type, and does not modify
 * the default behavior.
 */
class NoStage : public SimplePipelineStageLogicMixIn<NoStage> {
public:
    /**
     * @brief Prints the name of this stage.
     *
     * Used for logging purposes. Prints "NoStage" to identify this as a no-operation pipeline stage.
     */
    void print_name() {
        std::cout << "NoStage";
    }
};

/**
 * @brief A template mix-in class that represents a pipeline register connecting two pipeline stages.
 *
 * `SimplePipelineRegMixin` is designed to be inherited by a derived pipeline register class (`DerivedT`).
 * It is clocked by a `Clock` object and manages data transfer between a `PreviousStage` and a `NextStage`.
 * At `clock_start()`, it may transmit data to the next stage (unless stalled), and at `clock_end()`,
 * it accepts data from the previous stage.
 *
 * @tparam DerivedT       The derived class type that inherits from this mix-in.
 * @tparam PreviousStage  The type of the previous pipeline stage. Defaults to `NoStage`.
 * @tparam NextStage      The type of the next pipeline stage. Defaults to `NoStage`.
 * @tparam DataT          The type of the data held by this pipeline register. Defaults to `Instruction`.
 */
template<typename DerivedT, typename PreviousStage = NoStage, 
         typename NextStage = NoStage, typename DataT = Instruction>
class SimplePipelineRegMixin : public Clockable {
public:
    /**
     * @brief Constructs a pipeline register and registers it with the given clock.
     *
     * @param clk        Reference to the global/system clock.
     * @param prevStage  Pointer to the previous pipeline stage.
     * @param nextStage  Pointer to the next pipeline stage.
     *
     * Upon construction, the pipeline register is registered as a `Clockable` with the clock.
     * If `logged` is `true`, a creation message is printed.
     */
    SimplePipelineRegMixin(Clock &clk, PreviousStage *prevStage, NextStage *nextStage)
        : previous_stage(prevStage), next_stage(nextStage), 
          data(nullptr), stallFlag(false) {
        clk.registerClockable(static_cast<DerivedT*>(this));
        if (logged) {
            std::cout << "pipeline register created!" << std::endl;
        }
    }

    /**
     * @brief Invoked at the start of each clock cycle.
     *
     * If `stallFlag` is set, the register will not transmit data this cycle and will clear the stall flag.
     * Otherwise, it calls `transmit()` (implemented by the derived class) to send data to the next stage.
     * If `logged` is `true`, a message is printed indicating the start of the cycle.
     */
    void clock_start() override {
        if (logged) {
            std::cout << "clock start!" << std::endl;
        }
        if (stallFlag == true) {
            stallFlag = false;
            return;
        }
        static_cast<DerivedT&>(*this).transmit();
    }

    /**
     * @brief Invoked at the end of each clock cycle.
     *
     * At this point, the register attempts to accept data from the previous stage by calling `accept()`
     * (implemented by the derived class). If `logged` is `true`, it prints a message indicating the end of the cycle.
     */
    void clock_end() override {
        if (logged) {
            std::cout << "clock end!" << std::endl;
        }
        static_cast<DerivedT&>(*this).accept();
    }

    /**
     * @brief Requests that the register stall on the next cycle.
     *
     * Setting `stallFlag` to `true` prevents the register from transmitting data at the start
     * of the next clock cycle. This can be used to handle pipeline hazards or timing issues.
     */
    void receive_stall() {
        stallFlag = true;
    }

    /**
     * @brief Accepts data from the previous stage at the end of the cycle.
     *
     * This method is called by `clock_end()` (through the derived class). It attempts to retrieve data
     * from `previous_stage` by calling `flow_out()`. If non-null data is received, it is stored in `data`.
     * If `logged` is `true`, a message is printed.
     */
    void accept() {
        if (logged) {
            static_cast<DerivedT&>(*this).print_name();
            std::cout << " accept !" << std::endl;
        }
        if (DataT *data_in = previous_stage->flow_out(); data_in != nullptr)
            data = data_in;
    }

    /**
     * @brief Transmits data to the next stage at the start of the cycle.
     *
     * This method is called by `clock_start()` (through the derived class).
     * If `data` is non-null, it is passed to `next_stage->receive()`.
     * If `logged` is `true`, a message is printed.
     */
    void transmit() {
        if (logged) {
            static_cast<DerivedT&>(*this).print_name();
            std::cout << " transmit !" << std::endl;
        }
        if (data != nullptr)
            next_stage->receive(data);
    }

protected:
    /**
     * @brief A pointer to the previous pipeline stage.
     *
     * Used by `accept()` to retrieve data at the end of each cycle.
     */
    PreviousStage *previous_stage;

    /**
     * @brief A pointer to the next pipeline stage.
     *
     * Used by `transmit()` to pass data forward at the start of each cycle.
     */
    NextStage *next_stage;

    /**
     * @brief A pointer to the data currently held in the pipeline register.
     *
     * `data` is updated at `accept()` time and transmitted at `transmit()` time.
     */
    DataT *data = nullptr;

    /**
     * @brief A flag indicating whether the pipeline register should stall.
     *
     * If `true`, the register will skip transmitting data on the next `clock_start()`.
     * The stall is cleared after one cycle.
     */
    bool stallFlag = false;

    /**
     * @brief A flag that controls logging output.
     *
     * If `true`, the register prints diagnostic messages during creation, clock starts/ends,
     * and data acceptance/transmission.
     */
    bool logged = false;
};
