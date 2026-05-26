/*
 * Throttle    Pulse width
 * ─────────── ───────────
 *   0  %       1000 µs
 *  10  %       1100 µs
 *  25  %       1250 µs
 *  50  %       1500 µs
 *  75  %       1750 µs
 * 100  %       2000 µs
 */
#pragma once

#include "stm32f4xx_ll_tim.h"

namespace servo {
    enum class sequence_state {
        WAIT_ARMING,
        ARMING_COMPLETED,
    };

    inline constexpr int FrequencyHZ {50};
    inline constexpr int PeriodMs {1000/FrequencyHZ};
    inline constexpr int MinimumPulseWidthMs {1000};
    inline constexpr int MaximumPulseWidthMs {2000};
    inline constexpr int WaitTimeForArmingS {5};

    struct state {
        sequence_state SequenceState {sequence_state::WAIT_ARMING};
        int CurrentThrottle {};
        TIM_TypeDef* PWMTimer {};
    };

    inline int init(state* State) {
        int ret {};

        State->SequenceState   = sequence_state::WAIT_ARMING;
        State->CurrentThrottle = 0;

        LL_TIM_InitTypeDef TIMInit {
            .Prescaler {},
            .Autoreload {},
            .ClockDivision {},
            .RepetitionCounter {},
        };
        ErrorStatus status {LL_TIM_Init(State->PWMTimer, reinterpret_cast<const LL_TIM_InitTypedef*>(&TIMInit))};

        return ret;
    }

    inline int perform_arming_sequence(state* State);
    inline int change_throttle(state* State, int Throttle);
    inline int throttle_range_calibration(state* State);
} /* namespace servo */

