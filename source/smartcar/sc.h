//
// Created by 17616 on 2020/3/22.
//

#ifndef IMXRT_SC_H
#define IMXRT_SC_H

#include "fsl_common.h"
#include "fsl_gpio.h"
#include "fsl_lpi2c.h"
#include "fsl_adc.h"
#include <vector>

namespace sc {
    class GPIO;

    class Led;

    class Bee;

    class ADC;

    class Keypad;


    class GPIO {
    public:
        GPIO(void);

        GPIO(GPIO_Type *base_, uint32_t pin_);

        void In(void);

        void Out(void);

        void Set(void);

        void Clear(void);

        void Toggle(void);

        void Wirte(uint8_t output);

        uint32_t Read(void);

    private:
        GPIO_Type *base;
        uint32_t pin;
        enum class PinDir : uint8_t {
            Input = 0,
            Output,
        } dir;

        static const GPIO_Type *gpio_base_ptrs[];
    };

    class Bee : public GPIO {
    public:
        Bee() {}

        Bee(GPIO_Type *base_, uint32_t pin_) : GPIO(base_, pin_) {}

        void On() { Set(); }

        void Off() { Clear(); }
    };

    class Led : public GPIO {
    public:
        Led() {}

        Led(GPIO_Type *base_, uint32_t pin_) : GPIO(base_, pin_) {}

        void On() { Clear(); }

        void Off() { Set(); }
    };


    class I2C {
        I2C();
//        I2C()

    };

    class SPI {

    };

    class UART {

    };

    class PWMSubModule {

    };

    class ADC {

        ADC(ADC_Type *base_, uint32_t adc_channel_,
            const adc_config_t *config = &ADCDefaultConfig,
            adc_hardware_average_mode_t mode = kADC_HardwareAverageCount4);

        uint32_t Read(void);

        static const adc_config_t ADCDefaultConfig;

    private:
        static const ADC_Type *adc_base_ptrs[];
        static uint32_t adcIsInitMask;
        ADC_Type *base;
        uint32_t adc_channel;
        adc_channel_config_t adcChannelConfigStruct;
    };

    class Keypad {
    public:
        Keypad();

        struct Key {
            uint8_t row;
            uint8_t col;
        };

        int Init(std::vector<GPIO>Rows, std::vector<GPIO>Cols);

        const std::vector<uint8_t>&operator[](int row);

        const uint8_t Read(int row,int col){return (*this)[row][col];}

        int Check(int row,int col);

    private:
        std::vector<GPIO> rows;//行
        std::vector<GPIO> cols;//列
        std::vector<uint8_t> colResult;
    };
};
#endif //IMXRT_SC_H
