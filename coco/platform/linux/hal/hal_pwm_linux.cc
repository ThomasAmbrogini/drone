#include "coco/hal/pwm.hh"

#include "coco/assert.h"

#include <cstdio>

namespace coco {

struct pwm_mock {
    const char* Name {};
    bool Enabled {};
    int PeriodUs {};
    int PulseWidthUs {};
};

static pwm_mock PWMMocks [] {
    {
        .Name {"PWM Instance 1"},
        .PulseWidthUs {},
    },
};

static pwm_handle PWMHandles [] {
    { .PWMInstance {&PWMMocks[0]}, .PWMChannel {0} },
};

pwm_handle* pwm_retrieve_instance(pwm_instance Instance) {
    switch (Instance) {
        case pwm_instance::_1:
            return &PWMHandles[static_cast<int>(Instance)];
    }
    return nullptr;
}

int pwm_init(pwm_handle* PWMHandle) {
    COCO_ASSERT(PWMHandle);

    pwm_mock* Mock {static_cast<pwm_mock*>(PWMHandle->PWMInstance)};
    printf("PWM Instance name: %s\n", Mock->Name);
    return 0;
}

pwm_configuration pwm_get(pwm_handle* PWMHandle) {
    COCO_ASSERT(PWMHandle);

    pwm_mock* Mock {static_cast<pwm_mock*>(PWMHandle->PWMInstance)};
    return pwm_configuration {
        .Enabled {Mock->Enabled},
        .PeriodUs {Mock->PeriodUs},
        .PulseWidthUs {Mock->PulseWidthUs},
    };
}

void pwm_enable(pwm_handle* PWMHandle) {
    COCO_ASSERT(PWMHandle);

    pwm_mock* Mock {static_cast<pwm_mock*>(PWMHandle->PWMInstance)};
    Mock->Enabled = true;
}

void pwm_disable(pwm_handle* PWMHandle) {
    COCO_ASSERT(PWMHandle);

    pwm_mock* Mock {static_cast<pwm_mock*>(PWMHandle->PWMInstance)};
    Mock->Enabled = false;
}

void pwm_change_period(pwm_handle* PWMHandle, int PeriodUs) {
    COCO_ASSERT(PWMHandle);

    pwm_mock* Mock {static_cast<pwm_mock*>(PWMHandle->PWMInstance)};
    Mock->PeriodUs = PeriodUs;
}

void pwm_change_pulse_width(pwm_handle* PWMHandle, int PulseWidthUs) {
    COCO_ASSERT(PWMHandle);

    pwm_mock* Mock {static_cast<pwm_mock*>(PWMHandle->PWMInstance)};
    Mock->PulseWidthUs = PulseWidthUs;
}

} /* namespace coco */
