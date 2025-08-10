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

    void dump_latency_split_by_bht() const {
        const unsigned per_flush_cycles = 2;

        // BHT 猜 taken（pred_taken）
        const unsigned bht_taken_flushes =
            info.taken_info.btb_hit_info.incorrect_direction +                 // BTB hit & actual not-taken
            info.taken_info.btb_hit_info.actual_taken_info.incorrect_target +  // BTB hit & wrong target
            info.taken_info.btb_miss_info.actual_taken_info.btb_miss_taken;    // BTB miss & actual taken
        const unsigned long long bht_taken_cycles = 1ULL * bht_taken_flushes * per_flush_cycles;

        // BHT 猜 not-taken（pred_not_taken）
        const unsigned bht_not_taken_flushes = info.not_taken_info.incorrect_direction; // pred_not_taken & actual_taken
        const unsigned long long bht_not_taken_cycles = 1ULL * bht_not_taken_flushes * per_flush_cycles;

        const unsigned total_flushes = bht_taken_flushes + bht_not_taken_flushes;
        const unsigned long long total_cycles = bht_taken_cycles + bht_not_taken_cycles;

        auto pct = [](unsigned long long part, unsigned long long whole){
            return whole ? (100.0 * static_cast<long double>(part) / static_cast<long double>(whole)) : 0.0L;
        };

        std::cout << "\n=== Branch Miss Penalty by BHT Direction ===\n";
        std::cout << "BHT pred_taken : flushes " << bht_taken_flushes
                << " -> cycles " << bht_taken_cycles << "\n";
        std::cout << "BHT pred_not   : flushes " << bht_not_taken_flushes
                << " -> cycles " << bht_not_taken_cycles << "\n";
        std::cout << "TOTAL          : flushes " << total_flushes
                << " -> cycles " << total_cycles << "\n";
        std::cout << std::fixed << std::setprecision(2)
                << "Share          : pred_taken " << pct(bht_taken_cycles, total_cycles)
                << "%, pred_not " << pct(bht_not_taken_cycles, total_cycles) << "%\n";

        // 註記：方向錯但無延遲（pred_taken & BTB miss & actual not-taken）
        std::cout << "Dir-wrong/no-flush: "
                << info.taken_info.btb_miss_info.incorrect_direction_but_correct
                << " (pred_taken & BTB miss & actual not-taken)\n";
    }

    void dump_latency_bht_taken_breakdown_by_btb() const {
        const unsigned per_flush_cycles = 2;

        // 細分BTB Hit的兩種錯誤類型
        const unsigned btb_hit_direction_flushes = 
            info.taken_info.btb_hit_info.incorrect_direction;  // 方向錯誤
        const unsigned btb_hit_target_flushes = 
            info.taken_info.btb_hit_info.actual_taken_info.incorrect_target;  // 目標地址錯誤
        
        const unsigned long long btb_hit_direction_cycles = 
            1ULL * btb_hit_direction_flushes * per_flush_cycles;
        const unsigned long long btb_hit_target_cycles = 
            1ULL * btb_hit_target_flushes * per_flush_cycles;
        
        const unsigned btb_hit_flushes = 
            btb_hit_direction_flushes + btb_hit_target_flushes;
        const unsigned long long btb_hit_cycles = 
            btb_hit_direction_cycles + btb_hit_target_cycles;

        const unsigned btb_miss_flushes =
            info.taken_info.btb_miss_info.actual_taken_info.btb_miss_taken;    // miss & actual taken
        const unsigned long long btb_miss_cycles = 
            1ULL * btb_miss_flushes * per_flush_cycles;

        const unsigned bht_taken_flushes = 
            btb_hit_flushes + btb_miss_flushes;
        const unsigned long long bht_taken_cycles = 
            btb_hit_cycles + btb_miss_cycles;

        auto pct = [](unsigned long long part, unsigned long long whole){
            return whole ? (100.0 * static_cast<long double>(part) / static_cast<long double>(whole)) : 0.0L;
        };

        std::cout << "\n=== Within BHT pred_taken: Detailed BTB Breakdown ===\n";
        // BTB Hit細分
        std::cout << "BTB hit (direction error): flushes " << btb_hit_direction_flushes
                << " -> cycles " << btb_hit_direction_cycles << "\n";
        std::cout << "BTB hit (target error)   : flushes " << btb_hit_target_flushes
                << " -> cycles " << btb_hit_target_cycles << "\n";
        std::cout << "BTB hit (total)          : flushes " << btb_hit_flushes
                << " -> cycles " << btb_hit_cycles << "\n";
        
        // BTB Miss
        std::cout << "BTB miss (actual taken)  : flushes " << btb_miss_flushes
                << " -> cycles " << btb_miss_cycles << "\n";
        
        // 匯總信息
        std::cout << "Subtotal                 : flushes " << bht_taken_flushes
                << " -> cycles " << bht_taken_cycles << "\n";
        std::cout << std::fixed << std::setprecision(2)
                << "BTB hit share: " << pct(btb_hit_cycles, bht_taken_cycles) << "%"
                << " (direction: " << pct(btb_hit_direction_cycles, bht_taken_cycles) << "%, "
                << "target: " << pct(btb_hit_target_cycles, bht_taken_cycles) << "%)\n";
        std::cout << "BTB miss share: " << pct(btb_miss_cycles, bht_taken_cycles) << "%\n";
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