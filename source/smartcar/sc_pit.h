#ifndef SC_PIT_H_
#define SC_PIT_H_

#include "fsl_pit.h"

/**
 * @brief 初始化pit，但不会立即生效，注意各个通道共用同一个中断服务函数
 * 启用函数：PIT_EnableInterrupts
 * 关闭函数：PIT_DisableInterrupts
 * @param  {pit_chnl_t} channel : 通道
 * @param  {uint64_t} us        : 定时中断时间
 */
void PIT_Init2(pit_chnl_t channel, uint64_t us);

#endif /* SC_PIT_H_ */
