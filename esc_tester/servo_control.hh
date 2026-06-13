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

#include "coco/hal/pwm.hh"

namespace servo {
    enum class instance {
        _1,
    };

    enum class sequence_state {
        WAIT_ARMING,
        ARMING_COMPLETED,
        RANGE_CALIBRATION,
    };

    //TODO: All of these depends on the implementation for the PWM.
    inline constexpr int FrequencyHZ {50};
    inline constexpr int PeriodMs {1000/FrequencyHZ};
    inline constexpr int MinimumPulseWidthUs {1000};
    inline constexpr int MaximumPulseWidthUs {2000};
    inline constexpr int WaitTimeForArmingMs {5000};

    struct state {
        sequence_state SequenceState {sequence_state::WAIT_ARMING};
        int CurrentThrottle {};
        void* PWMInstance {};
    };

    inline int init(state* State, coco::pwm_instance PWMInstance) {
        int ret {};

        State->SequenceState = sequence_state::WAIT_ARMING;
        State->CurrentThrottle = MinimumPulseWidthUs;
        State->PWMInstance = coco::pwm_retrieve_instance(PWMInstance);

        //TODO: add the configuration in here.
        ret = coco::pwm_init(State->PWMInstance);

        return ret;
    }

    inline int change_throttle(state* State, int ThrottlePercentage) {
        if ((State->SequenceState != sequence_state::ARMING_COMPLETED) && (State->SequenceState != sequence_state::RANGE_CALIBRATION)) {
            return -1;
        }

        if ((ThrottlePercentage < 0) || (ThrottlePercentage > 100)) {
            return -1;
        }

        State->CurrentThrottle = ThrottlePercentage;

        int PulseWidthUs {MinimumPulseWidthUs + ThrottlePercentage * (MaximumPulseWidthUs - MinimumPulseWidthUs) / 100};

        int ret {coco::pwm_change_pulse_width(State->PWMInstance, PulseWidthUs)};

        return ret;
    }

    inline int perform_arming_sequence(state* State) {
        if (State->SequenceState != sequence_state::WAIT_ARMING) {
            return -1;
        }

        //Wait for WaitTimeForArmingMs milliseconds with the fixed state of the throttle.
        return 0;
    }

    inline int throttle_range_calibration(state* State) {
        //TODO: Should i have a range calibration state.
        State->SequenceState = sequence_state::RANGE_CALIBRATION;
        int ret {change_throttle(State, 100)};
        if (ret != 0) {
            return ret;
        }

        //TODO: sleep for WaitTimeForArmingMs.

        change_throttle(State, 0);
        //TODO: sleep for WaitTimeForArmingMs.

        return 0;
    }

} /* namespace servo */

