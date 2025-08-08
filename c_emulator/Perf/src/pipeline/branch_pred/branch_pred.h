#pragma once

#include <memory>
#include <cassert> // 添加cassert头文件以使用assert
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
        unsigned prediction_count = 0; // 總預測次數
        unsigned btb_miss = 0;
        unsigned btb_hit = 0;

        unsigned correct_pred = 0;             // 預測方向 & target 都正確
        unsigned incorrect_dir = 0;            // 預測方向錯
        unsigned incorrect_target = 0;      // 預測方向對，但 target 錯

        void dump() const {
            unsigned total = prediction_count;
            unsigned total_mispred = incorrect_dir + incorrect_target;
            double btb_hit_rate = total ? (double)btb_hit / total : 0;
            double direction_accuracy = total ? (double)(correct_pred + incorrect_target) / total : 0;
            double target_accuracy = (correct_pred + incorrect_target) ? 
                (double)correct_pred / (correct_pred + incorrect_target) : 0;
            double misprediction_rate = total ? (double)total_mispred / total : 0;

            std::cout << "====== Branch Predictor Stats ======\n";
            std::cout << "Total Predictions: " << total << "\n";
            std::cout << "BTB Hit: " << btb_hit << ", Miss: " << btb_miss
                    << " (Hit Rate: " << btb_hit_rate * 100 << "%)\n";

            std::cout << "Correct Predictions: " << correct_pred << "\n";
            std::cout << "Incorrect Direction Predictions: " << incorrect_dir << "\n";
            std::cout << "Incorrect Target Predictions: " << incorrect_target << "\n";
            std::cout << "Direction Accuracy: " << direction_accuracy * 100 << "%\n";
            std::cout << "Target Accuracy (on correct direction): " << target_accuracy * 100 << "%\n";
            std::cout << "Overall Misprediction Rate: " << misprediction_rate * 100 << "%\n";
            std::cout << "====================================\n";
        }

    };
public:
    using PCType = typename BTBType::PCType;

    BranchPredictor(std::unique_ptr<SkippedMispredFlushCycle> flush_policy = nullptr)
        : flush_policy(std::move(flush_policy)), btb(), bht() {}

    void predict(PCType pc) {
        info.prediction_count++;
        taken = bht.predict(pc);
        if (taken) {
            target = btb.lookup(pc);
            if (target) {
                info.btb_hit++;
            } else {
                taken = false;
                info.btb_miss++;
            }
        } else {
            target = std::nullopt;
        }
    }

    const LatencyInfo& get_branch_pred_info() const {
        return info;
    }

    void update_and_verify(PCType pc, bool actual_taken, PCType actual_target = 0) {
        // Update BHT
        bht.update(pc, actual_taken);

        // Update BTB only if actually taken
        if (actual_taken) {
            btb.update(pc, actual_target);
        }

        // Evaluate prediction correctness
        bool mispredicted = false;

        if (taken == actual_taken) {
            if (!taken) {
                info.correct_pred++;
                return;
            }
            if (target && *target == actual_target) {
                info.correct_pred++;
            } else {
                info.incorrect_target++;
                mispredicted = true;
            }
        } else {
            info.incorrect_dir++;
            mispredicted = true;
        }

        if (mispredicted && flush_policy) {
            flush_policy->flush();
            latency_info.update(2);
        }
    }

    void dump_info() const {
        std::cout << "Branch Predictor Info:" << std::endl;
        info.dump();
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
    bool taken;
    std::optional<PCType> target;
};

} // namespace pipeline_simulator
