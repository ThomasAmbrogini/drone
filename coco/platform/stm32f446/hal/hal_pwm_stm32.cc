#include "coco/hal/pwm.hh"

#include "coco/assert.h"

#include "stm32f4xx_ll_bus.h"
#include "stm32f4xx_ll_tim.h"

namespace coco {

namespace stm32 {
    uint32_t tim_get_channel_compare_value(const TIM_TypeDef* Timer, uint32_t Channel);
    void tim_set_channel_compare_value(TIM_TypeDef* Timer, uint32_t Channel, uint32_t Compare);
} /* namespace stm32 */

pwm_handle* pwm_retrieve_instance(pwm_instance Instance) {
    switch (Instance) {
        case pwm_instance::_1: {
            static pwm_handle PWMHandle1 {
                .PWMInstance {TIM5},
                .PWMChannel {LL_TIM_CHANNEL_CH1},
            };
            return &PWMHandle1;
        }
    }
    return nullptr;
}

int pwm_init(pwm_handle* PWMHandle) {
    COCO_ASSERT(PWMHandle);

    TIM_TypeDef* Timer {static_cast<TIM_TypeDef*>(PWMHandle->PWMInstance)};

    //TODO: this should be done through the hal.
    if (Timer == TIM5) {
        LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM5);
    }

    LL_TIM_InitTypeDef TIMInit {};
    TIMInit.Prescaler         = 0;
    TIMInit.Autoreload        = 0;
    TIMInit.ClockDivision     = LL_TIM_CLOCKDIVISION_DIV1;
    TIMInit.CounterMode       = LL_TIM_COUNTERMODE_UP;
    TIMInit.RepetitionCounter = 0;

    if (LL_TIM_Init(Timer, &TIMInit) != SUCCESS) {
        return -1;
    }

    LL_TIM_OC_InitTypeDef OCInit {};
    OCInit.OCMode       = LL_TIM_OCMODE_PWM1;
    OCInit.OCState      = LL_TIM_OCSTATE_DISABLE;
    OCInit.OCNState     = LL_TIM_OCSTATE_DISABLE;
    OCInit.CompareValue = 0;
    OCInit.OCPolarity   = LL_TIM_OCPOLARITY_HIGH;
    OCInit.OCNPolarity  = LL_TIM_OCPOLARITY_HIGH;
    OCInit.OCIdleState  = LL_TIM_OCIDLESTATE_LOW;
    OCInit.OCNIdleState = LL_TIM_OCIDLESTATE_LOW;

    if (LL_TIM_OC_Init(Timer, PWMHandle->PWMChannel, &OCInit) != SUCCESS) {
        return -1;
    }

    return 0;
}

pwm_configuration pwm_get(pwm_handle* PWMHandle) {
    COCO_ASSERT(PWMHandle);
    COCO_ASSERT(LL_APB1_GRP1_IsEnabledClock(LL_APB1_GRP1_PERIPH_TIM5));

    TIM_TypeDef* Timer {static_cast<TIM_TypeDef*>(PWMHandle->PWMInstance)};
    uint32_t Prescaler {LL_TIM_GetPrescaler(Timer) + 1};

    return pwm_configuration {
        .Enabled {LL_TIM_IsEnabledCounter(Timer) && LL_TIM_CC_IsEnabledChannel(Timer, PWMHandle->PWMChannel)},
        .PeriodUs {static_cast<int>(static_cast<uint64_t>(LL_TIM_GetAutoReload(Timer) + 1) * Prescaler * 1000000UL / SystemCoreClock)},
        .PulseWidthUs {static_cast<int>(static_cast<uint64_t>(stm32::tim_get_channel_compare_value(Timer, PWMHandle->PWMChannel)) * Prescaler * 1000000UL / SystemCoreClock)},
    };
}

void pwm_enable(pwm_handle* PWMHandle) {
    COCO_ASSERT(PWMHandle);
    COCO_ASSERT(LL_APB1_GRP1_IsEnabledClock(LL_APB1_GRP1_PERIPH_TIM5));

    TIM_TypeDef* Timer {static_cast<TIM_TypeDef*>(PWMHandle->PWMInstance)};

    LL_TIM_CC_EnableChannel(Timer, PWMHandle->PWMChannel);
    LL_TIM_EnableCounter(Timer);
}

void pwm_disable(pwm_handle* PWMHandle) {
    COCO_ASSERT(PWMHandle);
    COCO_ASSERT(LL_APB1_GRP1_IsEnabledClock(LL_APB1_GRP1_PERIPH_TIM5));

    TIM_TypeDef* Timer {static_cast<TIM_TypeDef*>(PWMHandle->PWMInstance)};
    LL_TIM_CC_DisableChannel(Timer, PWMHandle->PWMChannel);
}

// Assumes the timer input clock equals SystemCoreClock (APB prescaler = 1).
// If the timer is on an APB bus with a prescaler > 1, replace SystemCoreClock
// with the actual timer input clock (2 * APBx clock on STM32F4).
void pwm_change_pulse_width(pwm_handle* PWMHandle, int PulseWidthUs) {
    COCO_ASSERT(PWMHandle);
    COCO_ASSERT(LL_APB1_GRP1_IsEnabledClock(LL_APB1_GRP1_PERIPH_TIM5));

    TIM_TypeDef* Timer {static_cast<TIM_TypeDef*>(PWMHandle->PWMInstance)};

    uint32_t Prescaler {LL_TIM_GetPrescaler(Timer) + 1};
    uint32_t Compare {static_cast<uint32_t>(static_cast<uint64_t>(PulseWidthUs) * SystemCoreClock / Prescaler / 1000000UL)};
    stm32::tim_set_channel_compare_value(Timer, PWMHandle->PWMChannel, Compare);
}

void pwm_change_period(pwm_handle* PWMHandle, int PeriodUs) {
    COCO_ASSERT(PWMHandle);
    COCO_ASSERT(LL_APB1_GRP1_IsEnabledClock(LL_APB1_GRP1_PERIPH_TIM5));

    TIM_TypeDef* Timer {static_cast<TIM_TypeDef*>(PWMHandle->PWMInstance)};

    uint32_t Prescaler {LL_TIM_GetPrescaler(Timer) + 1};
    uint32_t Autoreload {static_cast<uint32_t>(static_cast<uint64_t>(PeriodUs) * SystemCoreClock / Prescaler / 1000000UL)};
    COCO_ASSERT(Autoreload > 0);
    uint32_t AutoreloadRegister {Autoreload - 1};

    LL_TIM_SetAutoReload(Timer, AutoreloadRegister);
}

namespace stm32 {
    uint32_t tim_get_channel_compare_value(const TIM_TypeDef* Timer, uint32_t Channel) {
        switch(Channel) {
            case LL_TIM_CHANNEL_CH1: {
                return LL_TIM_OC_GetCompareCH1(Timer);
            }
            case LL_TIM_CHANNEL_CH2: {
                return LL_TIM_OC_GetCompareCH2(Timer);
            }
            case LL_TIM_CHANNEL_CH3: {
                return LL_TIM_OC_GetCompareCH3(Timer);
            }
            case LL_TIM_CHANNEL_CH4: {
                return LL_TIM_OC_GetCompareCH4(Timer);
            }
        }
        COCO_ASSERT(false);
        return 0;
    }

    void tim_set_channel_compare_value(TIM_TypeDef* Timer, uint32_t Channel, uint32_t Compare) {
        switch(Channel) {
            case LL_TIM_CHANNEL_CH1: {
                return LL_TIM_OC_SetCompareCH1(Timer, Compare);
            }
            case LL_TIM_CHANNEL_CH2: {
                return LL_TIM_OC_SetCompareCH2(Timer, Compare);
            }
            case LL_TIM_CHANNEL_CH3: {
                return LL_TIM_OC_SetCompareCH3(Timer, Compare);
            }
            case LL_TIM_CHANNEL_CH4: {
                return LL_TIM_OC_SetCompareCH4(Timer, Compare);
            }
        }
        COCO_ASSERT(false);
    }
} /* namespace stm32 */


} /* namespace coco */
