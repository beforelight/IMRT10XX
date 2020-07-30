#ifndef SC_ENC_H_
#define SC_ENC_H_
#include "fsl_enc.h"
/**
 * @brief   将enc模块初始化为正交解码模式，此时a,b相分别接编码器的a,b信号，顺序可交换
 * @param  {ENC_Type*} base : ENC peripheral base address.（such as "ENC1","ENC2","ENC3"，"ENC4"）
 */
void ENC_InitDecoder(ENC_Type* base);

/**
 * @brief   将enc模块初始化为脉冲模式，此时a相接编码器的脉冲信号，
 *                                      b相接编码器的方向信号，顺序不可交换
 * @param  {ENC_Type*} base : ENC peripheral base address.（such as "ENC1","ENC2","ENC3"，"ENC4"）
 */
void ENC_InitPhaseCounter(ENC_Type* base);

/**
 * @brief   返回脉冲计数，每次调用这个函数时，会自动记录本次调用和上次调用俩次脉冲计数差值，
 *              这个差值可以通过ENC_Dateget获得。
 * @param  {ENC_Type*} base : ENC peripheral base address.（such as "ENC1","ENC2","ENC3"，"ENC4"）
 * @return {int32_t}        : 脉冲计数
 */
int32_t ENC_PositionGet(ENC_Type* base);

/**
 * @brief   清空脉冲计数器
 * @param  {ENC_Type*} base : ENC peripheral base address.（such as "ENC1","ENC2","ENC3"，"ENC4"）
 */
void ENC_PositionClear(ENC_Type *base);

/**
 * @brief   返回俩次读取的脉冲计数差值，使用本函数可以避免清空脉冲计数器造成的误差
 * @param  {ENC_Type*} base : ENC peripheral base address.（such as "ENC1","ENC2","ENC3"，"ENC4"）
 * @return {int16_t}        : 俩次读取的脉冲计数差值
 */
int16_t ENC_Dateget(ENC_Type* base);

/**
 * @brief   返回圈数计数器的值，当前设置为当脉冲计数器溢出时记一圈
 * @param  {ENC_Type*} base : ENC peripheral base address.（such as "ENC1","ENC2","ENC3"，"ENC4"）
 * @return {int16_t}        : 圈数计数器的值
 */
int16_t ENC_Revolutionget(ENC_Type* base);

/**
 * @brief   取消初始化
 * @param  {ENC_Type*} base : ENC peripheral base address.（such as "ENC1","ENC2","ENC3"，"ENC4"）
 */
extern void ENC_Deinit(ENC_Type* base);
#endif /* SC_ENC_H_ */
