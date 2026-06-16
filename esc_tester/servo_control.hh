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
#include "coco/os/sleep.hh"

namespace servo {

enum class sequence_state {
    WAIT_ARMING,
    ARMING_COMPLETED,
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

namespace details {

inline void _change_throttle(state* State, int ThrottlePercentage) {
    State->CurrentThrottle = ThrottlePercentage;
    int PulseWidthUs {MinimumPulseWidthUs + ThrottlePercentage * (MaximumPulseWidthUs - MinimumPulseWidthUs) / 100};
    coco::pwm_change_pulse_width(State->PWMInstance, PulseWidthUs);
}

} /* namespace details */;

inline int init(state* State, coco::pwm_instance PWMInstance) {
    int ret {};

    State->SequenceState = sequence_state::WAIT_ARMING;
    State->CurrentThrottle = 0;
    State->PWMInstance = coco::pwm_retrieve_instance(PWMInstance);

    ret = {coco::pwm_init(State->PWMInstance)};
    if (ret != 0) {
        return -1;
    }

    coco::pwm_change_period(State->PWMInstance, (1000000 / FrequencyHZ));
    details::_change_throttle(State, 0);

    //TODO: should I enable the PWM?
    coco::pwm_enable(State->PWMInstance);

    //TODO: maybe the arming sequence can be done in here since I have to do it at the start anyway.

    return ret;
}

inline int change_throttle(state* State, int ThrottlePercentage) {
    if (State->SequenceState != sequence_state::ARMING_COMPLETED) {
        return -1;
    }

    if ((ThrottlePercentage < 0) || (ThrottlePercentage > 100)) {
        return -1;
    }

    details::_change_throttle(State, ThrottlePercentage);

    return 0;
}

inline int perform_arming_sequence(state* State) {
    if (State->SequenceState != sequence_state::WAIT_ARMING) {
        return -1;
    }

    details::_change_throttle(State, 0);
    os::sleep_busy_wait_ms(WaitTimeForArmingMs);

    State->SequenceState = sequence_state::ARMING_COMPLETED;

    return 0;
}

} /* namespace servo */

