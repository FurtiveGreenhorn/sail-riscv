#pragma once

#include <memory>
#include <cassert>
#include <iostream>
#include <iomanip> // 添加iomanip用於格式化輸出
#include "btb.h"
#include "bht.h"
#include "../flush_policy.h"
#include "../latency_log.h"

namespace pipeline_simulator {

using DefaultBTBParams = BTBParams<32, 2>;
using DefaultBTB = BTB<DefaultBTBParams>;

template<typename BTBType = DefaultBTB>
class BranchPredictor {
    struct BranchPredInfo {
        unsigned prediction_count = 0;
        
        struct TakenInfo {
            struct BTBHitInfo {
                struct ActualTakenInfo {
                    unsigned correct_target = 0;
                    unsigned incorrect_target = 0;
                } actual_taken_info;
                unsigned incorrect_direction = 0;
            } btb_hit_info;
            
            struct BTBMissInfo {
                struct ActualTakenInfo {
                    unsigned btb_miss_taken = 0;
                } actual_taken_info;
                unsigned incorrect_direction_but_correct = 0; // pc + 4 = target
            } btb_miss_info;
        } taken_info;

        struct NotTakenInfo {
            unsigned correct_direction = 0;
            unsigned incorrect_direction = 0;
        } not_taken_info;

        // 輔助計算函數保持不變
        unsigned total_taken_predictions() const {
            return taken_info.btb_hit_info.actual_taken_info.correct_target +
                   taken_info.btb_hit_info.actual_taken_info.incorrect_target +
                   taken_info.btb_hit_info.incorrect_direction +
                   taken_info.btb_miss_info.actual_taken_info.btb_miss_taken +
                   taken_info.btb_miss_info.incorrect_direction_but_correct;
        }

        unsigned total_not_taken_predictions() const {
            return not_taken_info.correct_direction +
                   not_taken_info.incorrect_direction;
        }

        unsigned total_correct_direction() const {
            return taken_info.btb_hit_info.actual_taken_info.correct_target +
                   taken_info.btb_hit_info.actual_taken_info.incorrect_target +
                   taken_info.btb_miss_info.actual_taken_info.btb_miss_taken +
                   not_taken_info.correct_direction;
        }

        unsigned total_incorrect_direction() const {
            return taken_info.btb_hit_info.incorrect_direction +
                   taken_info.btb_miss_info.incorrect_direction_but_correct +
                   not_taken_info.incorrect_direction;
        }

        unsigned total_btb_hits() const {
            return taken_info.btb_hit_info.actual_taken_info.correct_target +
                   taken_info.btb_hit_info.actual_taken_info.incorrect_target +
                   taken_info.btb_hit_info.incorrect_direction;
        }

        unsigned total_btb_misses() const {
            return taken_info.btb_miss_info.actual_taken_info.btb_miss_taken +
                   taken_info.btb_miss_info.incorrect_direction_but_correct;
        }
    };

public:
    using PCType = typename BTBType::PCType;

    BranchPredictor(std::unique_ptr<SkippedMispredFlushCycle> flush_policy = nullptr)
        : flush_policy(std::move(flush_policy)), 
          btb(), 
          bht(),
          pred_taken(false),
          pred_target(std::nullopt) {}

    void predict(PCType pc) {
        info.prediction_count++;
        pred_taken = bht.predict(pc);
        pred_target = btb.lookup(pc);
    }

    const BranchPredInfo& get_branch_pred_info() const {
        return info;
    }

    void update_and_verify(PCType pc, bool actual_taken, PCType actual_target = 0) {
        bht.update(pc, actual_taken);
        bool incorrect_pred = false;

        if (pred_taken) {
            if (pred_target.has_value()) {
                if (actual_taken) {
                    if (*pred_target == actual_target) {
                        info.taken_info.btb_hit_info.actual_taken_info.correct_target++;
                    } 
                    else {
                        info.taken_info.btb_hit_info.actual_taken_info.incorrect_target++;
                        incorrect_pred = true;
                    }
                } 
                else {
                    info.taken_info.btb_hit_info.incorrect_direction++;
                    incorrect_pred = true;
                }
            }
            else {
                if (actual_taken) {
                    info.taken_info.btb_miss_info.actual_taken_info.btb_miss_taken++;
                    incorrect_pred = true;
                } 
                else {
                    info.taken_info.btb_miss_info.incorrect_direction_but_correct++;
                }
            }
        }
        else {
            if (actual_taken) {
                info.not_taken_info.incorrect_direction++;
                incorrect_pred = true;
            }
            else {
                info.not_taken_info.correct_direction++;
            }
        }        
        
        if (actual_taken && (incorrect_pred || !pred_target.has_value())) {
            btb.update(pc, actual_target);
        }
        
        if (incorrect_pred) {
            if (flush_policy) {
                flush_policy->flush();
            }
            latency_info.update(2);
        }
    }

    void dump_info() const {
        const unsigned total_predictions = info.prediction_count;
        const unsigned taken_predictions = info.total_taken_predictions();
        const unsigned not_taken_predictions = info.total_not_taken_predictions();
        const unsigned correct_direction = info.total_correct_direction();
        const unsigned incorrect_direction = info.total_incorrect_direction();
        const unsigned btb_hits = info.total_btb_hits();
        const unsigned btb_misses = info.total_btb_misses();
        
        // 計算各種比率
        const double direction_accuracy = total_predictions > 0 ? 
            (static_cast<double>(correct_direction) / total_predictions * 100.0) : 0.0;
        
        // 修正整體準確率計算
        const unsigned overall_correct = 
            info.taken_info.btb_hit_info.actual_taken_info.correct_target + 
            info.not_taken_info.correct_direction;
        const double overall_accuracy = total_predictions > 0 ?
            (static_cast<double>(overall_correct) / total_predictions * 100.0) : 0.0;
        
        const double btb_hit_rate = taken_predictions > 0 ? 
            (static_cast<double>(btb_hits) / taken_predictions * 100.0) : 0.0;
        
        const double target_accuracy = (info.taken_info.btb_hit_info.actual_taken_info.correct_target + 
                                    info.taken_info.btb_hit_info.actual_taken_info.incorrect_target) > 0 ?
            (static_cast<double>(info.taken_info.btb_hit_info.actual_taken_info.correct_target) / 
            (info.taken_info.btb_hit_info.actual_taken_info.correct_target + 
            info.taken_info.btb_hit_info.actual_taken_info.incorrect_target) * 100.0) : 0.0;
        
        // 格式化輸出
        std::cout << "\n========== Branch Predictor Performance Report ==========\n";
        std::cout << "Total Predictions:      " << total_predictions << "\n";
        std::cout << "Direction Accuracy:     " << std::fixed << std::setprecision(2) 
                << direction_accuracy << "%\n";
        std::cout << "Overall Accuracy:       " << std::fixed << std::setprecision(2) 
                << overall_accuracy << "% (direction + target)\n";
        std::cout << "Misprediction Rate:     " << std::fixed << std::setprecision(2) 
                << (100.0 - direction_accuracy) << "%\n\n";
        
        std::cout << "--- Prediction Type Distribution ---\n";
        // 修正表達式閉合問題
        std::cout << "Taken Predictions:      " << taken_predictions << " (" 
                << std::fixed << std::setprecision(2) 
                << (static_cast<double>(taken_predictions) / total_predictions * 100.0)
                << "%)\n";
        std::cout << "Not-Taken Predictions:  " << not_taken_predictions << " (" 
                << std::fixed << std::setprecision(2) 
                << (static_cast<double>(not_taken_predictions) / total_predictions * 100.0)
                << "%)\n\n";
        
        std::cout << "--- BTB Performance ---\n";
        std::cout << "BTB Hit Rate:           " << std::fixed << std::setprecision(2) 
                << btb_hit_rate << "%\n";
        std::cout << "BTB Target Accuracy:    " << std::fixed << std::setprecision(2) 
                << target_accuracy << "% (when hit and taken)\n";
        std::cout << "BTB Misses:             " << btb_misses << "\n";
        std::cout << "  - Caused Mispredict: " << info.taken_info.btb_miss_info.actual_taken_info.btb_miss_taken 
                << "\n\n";
        
        std::cout << "--- Misprediction Analysis ---\n";
        std::cout << "Direction Errors:       " << incorrect_direction << "\n";
        std::cout << "  - Taken but not:     " << info.taken_info.btb_hit_info.incorrect_direction 
                << " (BTB hit)\n";
        std::cout << "  - Taken but not:     " << info.taken_info.btb_miss_info.incorrect_direction_but_correct 
                << " (BTB miss)\n";
        std::cout << "  - Not taken but did: " << info.not_taken_info.incorrect_direction << "\n";
        std::cout << "Target Errors:          " << info.taken_info.btb_hit_info.actual_taken_info.incorrect_target 
                << " (correct direction, wrong target)\n";
        std::cout << "==========================================================\n";
    }

    const LatencyInfo& get_latency_info() const {
        return latency_info;
    }

private:
    std::unique_ptr<SkippedMispredFlushCycle> flush_policy;
    LatencyInfo latency_info;
    BranchPredInfo info;
    BTBType btb;
    BHT<PCType> bht;
    bool pred_taken;
    std::optional<PCType> pred_target;
};

} // namespace pipeline_simulator