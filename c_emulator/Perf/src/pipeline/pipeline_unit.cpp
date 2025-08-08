#include "pipeline_unit.h"
#include "latency_log.h"

namespace pipeline_simulator {

void Execute::process_stage() {
    if (data->is_load()) {
        hazard_detection_unit->
            receive_IdEx_rd(data->rd);
    }
    // a simple implementation of execution unit
    switch (data->get_execution_unit_type()) {
        case ExecutionUnitType::INT_ALU:
            int_alu.execute(data);
        break;
        case ExecutionUnitType::MUL_UNIT:
            mul_unit.execute(data);
        break;
        case ExecutionUnitType::DIV_UNIT:
            div_unit.execute(data);
        break;
        case ExecutionUnitType::NONE:
            // Handle NONE case if necessary
        break;
    }
    if (data->is_branch()) {
        // Branch prediction
        branch_predictor->predict(data->addr);
        if (data->branch_taken()) {
            branch_predictor->update_and_verify(
                data->addr, true, data->get_target_addr());
        } else {
            branch_predictor->update_and_verify(
                data->addr, false);
        }
    }
}

const LatencyInfo *
Execute::get_latency_info(ExecutionUnitType unit) const {
    switch (unit) {
        case ExecutionUnitType::INT_ALU:
            return &int_alu.get_latency_info();
        break;
        case ExecutionUnitType::MUL_UNIT:
            return &mul_unit.get_latency_info();
        break;
        case ExecutionUnitType::DIV_UNIT:
            return &div_unit.get_latency_info();
        break;
        case ExecutionUnitType::NONE:
            return nullptr;
        break;
    }
    return nullptr;
}

// HazardDetectionUnit function implementation
void HazardDetectionUnit::handle_load_use_hazard() {
    if (debug_logged)
        std::cout << "detect hazard !" << std::endl;
    // send stall signal to IF/ID & PC
    // send flush signal to ID/EX
    pc_reg->receive_stall();
    if_id_reg->receive_stall();
    id_ex_reg->flush();
    if (debug_logged) {
        std::cout << "Send flush to ID/EX !" << std::endl;
    }
    latency_info.update(1);
}

void HazardDetectionUnit::receive_IdEx_rd(RegNum rd) {
    IdExRegRd = rd;
    check_hazard();
}

void HazardDetectionUnit::receive_IfId_rs(RegNum rs1, RegNum rs2) {
    IfIdRegRs1 = rs1;
    IfIdRegRs2 = rs2;
    check_hazard();
}

void HazardDetectionUnit::check_hazard() {
    static bool load_use_detection_ready = false;

    if (load_use_detection_ready == false) {
        load_use_detection_ready = true;
        return;
    }

    load_use_detection_ready = false;
    if ((IdExRegRd != IfIdRegRs1) && (IdExRegRd != IfIdRegRs2))
        return;
    
    handle_load_use_hazard();
}

} // namespace pipeline_simulator