#ifndef _CLOCK_CONFIG_H_
#define _CLOCK_CONFIG_H_

#include "fsl_common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_XTAL0_CLK_HZ                         24000000U  /*!< Board xtal0 frequency in Hz */

#define BOARD_XTAL32K_CLK_HZ                          32768U  /*!< Board xtal32k frequency in Hz */
/*******************************************************************************
 ************************ BOARD_InitBootClocks function ************************
 ******************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes default configuration of clocks.
 *
 */
void BOARD_InitBootClocks(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

/*******************************************************************************
 ************************** Configuration DCD_HSRUN ****************************
 ******************************************************************************/
/*******************************************************************************
 * Definitions for DCD_HSRUN configuration
 ******************************************************************************/
#define DCD_HSRUN_CORE_CLOCK                      600000000U  /*!< Core clock frequency: 600000000Hz */

/* Clock outputs (values are in Hz): */
#define DCD_HSRUN_AHB_CLK_ROOT                        600000000UL
#define DCD_HSRUN_CAN_CLK_ROOT                        40000000UL
#define DCD_HSRUN_CKIL_SYNC_CLK_ROOT                  0UL
#define DCD_HSRUN_CLKO1_CLK                           0UL
#define DCD_HSRUN_CLKO2_CLK                           0UL
#define DCD_HSRUN_CLK_1M                              1000000UL
#define DCD_HSRUN_CLK_24M                             24000000UL
#define DCD_HSRUN_CSI_CLK_ROOT                        24000000UL
#define DCD_HSRUN_ENET1_TX_CLK                        2400000UL
#define DCD_HSRUN_ENET_125M_CLK                       2400000UL
#define DCD_HSRUN_ENET_25M_REF_CLK                    1200000UL
#define DCD_HSRUN_FLEXIO1_CLK_ROOT                    30000000UL
#define DCD_HSRUN_FLEXIO2_CLK_ROOT                    30000000UL
#define DCD_HSRUN_FLEXSPI_CLK_ROOT                    127058823UL
#define DCD_HSRUN_GPT1_IPG_CLK_HIGHFREQ               50000000UL
#define DCD_HSRUN_GPT2_IPG_CLK_HIGHFREQ               50000000UL
#define DCD_HSRUN_IPG_CLK_ROOT                        150000000UL
#define DCD_HSRUN_LCDIF_CLK_ROOT                      67500000UL
#define DCD_HSRUN_LPI2C_CLK_ROOT                      12000000UL
#define DCD_HSRUN_LPSPI_CLK_ROOT                      105600000UL
#define DCD_HSRUN_LVDS1_CLK                           1200000000UL
#define DCD_HSRUN_MQS_MCLK                            63529411UL
#define DCD_HSRUN_PERCLK_CLK_ROOT                     50000000UL
#define DCD_HSRUN_PLL7_MAIN_CLK                       24000000UL
#define DCD_HSRUN_SAI1_CLK_ROOT                       63529411UL
#define DCD_HSRUN_SAI1_MCLK1                          63529411UL
#define DCD_HSRUN_SAI1_MCLK2                          63529411UL
#define DCD_HSRUN_SAI1_MCLK3                          30000000UL
#define DCD_HSRUN_SAI2_CLK_ROOT                       63529411UL
#define DCD_HSRUN_SAI2_MCLK1                          63529411UL
#define DCD_HSRUN_SAI2_MCLK2                          0UL
#define DCD_HSRUN_SAI2_MCLK3                          30000000UL
#define DCD_HSRUN_SAI3_CLK_ROOT                       63529411UL
#define DCD_HSRUN_SAI3_MCLK1                          63529411UL
#define DCD_HSRUN_SAI3_MCLK2                          0UL
#define DCD_HSRUN_SAI3_MCLK3                          30000000UL
#define DCD_HSRUN_SEMC_CLK_ROOT                       163862068UL
#define DCD_HSRUN_SPDIF0_CLK_ROOT                     30000000UL
#define DCD_HSRUN_SPDIF0_EXTCLK_OUT                   0UL
#define DCD_HSRUN_TRACE_CLK_ROOT                      99000000UL
#define DCD_HSRUN_UART_CLK_ROOT                       80000000UL
#define DCD_HSRUN_USBPHY1_CLK                         0UL
#define DCD_HSRUN_USBPHY2_CLK                         0UL
#define DCD_HSRUN_USDHC1_CLK_ROOT                     163862068UL
#define DCD_HSRUN_USDHC2_CLK_ROOT                     163862068UL

/*! @brief Arm PLL set for DCD_HSRUN configuration.
 */
extern const clock_arm_pll_config_t armPllConfig_DCD_HSRUN;
/*! @brief Usb1 PLL set for DCD_HSRUN configuration.
 */
extern const clock_usb_pll_config_t usb1PllConfig_DCD_HSRUN;
/*! @brief Sys PLL for DCD_HSRUN configuration.
 */
extern const clock_sys_pll_config_t sysPllConfig_DCD_HSRUN;
/*! @brief Enet PLL set for DCD_HSRUN configuration.
 */
extern const clock_enet_pll_config_t enetPllConfig_DCD_HSRUN;

/*******************************************************************************
 * API for DCD_HSRUN configuration
 ******************************************************************************/
#if defined(__cplusplus)
extern "C" {
#endif /* __cplusplus*/

/*!
 * @brief This function executes configuration of clocks.
 *
 */
void DCD_HSRUN(void);

#if defined(__cplusplus)
}
#endif /* __cplusplus*/

#endif /* _CLOCK_CONFIG_H_ */

