//
// Created by 17616 on 2020/3/22.
//
#include "FreeRTOS.h"
#include "task.h"
#include "sc.h"
#include "fsl_debug_console.h"

const GPIO_Type *sc::GPIO::gpio_base_ptrs[] = GPIO_BASE_PTRS;

sc::GPIO::GPIO() {
    dir = PinDir::Input;
}

sc::GPIO::GPIO(GPIO_Type *base_, uint32_t pin_) : base(base_), pin(pin_) {
    int gpio = 0;
    for (int i = 1; i < sizeof(gpio_base_ptrs) / sizeof(GPIO_Type *); ++i) {
        if (base == gpio_base_ptrs[i]) {
            gpio = i;
            break;
        }
    }
    assert(gpio);
    assert(pin < 32);
    GPIO();
}

void sc::GPIO::In(void) {
    if (dir == PinDir::Input) {
    } else {
        dir = PinDir::Input;
        base->GDIR &= ~(1UL << pin);
    }
}

void sc::GPIO::Out(void) {
    if (dir == PinDir::Output) {
    } else {
        dir = PinDir::Output;
        base->GDIR |= (1UL << pin);
    }
}

void sc::GPIO::Set(void) {
    GPIO_PortSet(base, 1 << pin);
    Out();
}

void sc::GPIO::Clear(void) {
    GPIO_PortClear(base, 1 << pin);
    Out();
}

void sc::GPIO::Toggle(void) {
    GPIO_PortToggle(base, 1 << pin);
    Out();
}

void sc::GPIO::Wirte(uint8_t output) {
    if (output == 0) {
        Clear();
    } else {
        Set();
    }
}

uint32_t sc::GPIO::Read(void) {
    In();
    return GPIO_PinRead(base, pin);
}


const ADC_Type *sc::ADC::adc_base_ptrs[] = ADC_BASE_PTRS;
uint32_t sc::ADC::adcIsInitMask = 0;
const adc_config_t sc::ADC::ADCDefaultConfig = {
        .enableOverWrite = false,
        .enableContinuousConversion = false,
        .enableHighSpeed = false,
        .enableLowPower = false,
        .enableLongSample = false,
        .enableAsynchronousClockOutput = true,
        .referenceVoltageSource = kADC_ReferenceVoltageSourceAlt0,
        .samplePeriodMode = kADC_SamplePeriodShort8Clocks,
        .clockSource = kADC_ClockSourceIPG,
        .clockDriver = kADC_ClockDriver1,
        .resolution = kADC_Resolution12Bit
};

sc::ADC::ADC(ADC_Type *base_, uint32_t adc_channel_, const adc_config_t *config, adc_hardware_average_mode_t mode)
        : base(base_), adc_channel(adc_channel_) {
    int adc = 0;//当前是第几个adc
    for (int i = 1; i < sizeof(adc_base_ptrs) / sizeof(ADC_Type *); ++i) {
        if (base == adc_base_ptrs[i]) {
            adc = i;
            break;
        }
    }
    assert(adc);
    if (adcIsInitMask & 1 << adc == 0)//该adc还没有初始化
    {
        adcIsInitMask |= 1 << adc;
        ADC_Init(base, config);
        ADC_SetHardwareAverageConfig(base, mode);
        ADC_DoAutoCalibration(base);
        adcChannelConfigStruct.channelNumber = adc_channel;
        adcChannelConfigStruct.enableInterruptOnConversionCompleted = false;//取消采集完成中断
    }
}

uint32_t sc::ADC::Read(void) {
#define DEMO_ADC_CHANNEL_GROUP 0U
    ADC_SetChannelConfig(base, DEMO_ADC_CHANNEL_GROUP, &adcChannelConfigStruct);
    while (0U == ADC_GetChannelStatusFlags(base, DEMO_ADC_CHANNEL_GROUP)) {
    }
    return ADC_GetChannelConversionValue(base, DEMO_ADC_CHANNEL_GROUP);
}



int sc::Keypad::Init(std::vector<GPIO> Rows, std::vector<GPIO> Cols) {
    rows = Rows;
    cols = Cols;

    for (auto i:rows) {
        i.Set();
    }
    for (auto i:cols) {
        i.In();
        colResult.push_back(0);
    }

    return 0;
}

const std::vector<uint8_t> &sc::Keypad::operator[](int row) {
    rows[row].Clear();
    for (int i = 0; i < cols.size(); ++i) {
        if (cols[i].Read() == 0) {
            colResult[i] = 1;
        } else {
            colResult[i] = 0;
        }
    }
    rows[row].Set();
    return colResult;
}

int sc::Keypad::Check(int row, int col) {
    int times = 50;
    while (times--) {
        if ((*this)[row][col] == 0) {
            return 0;
        }
        vTaskDelay(1);
    }
    return 1;
}
