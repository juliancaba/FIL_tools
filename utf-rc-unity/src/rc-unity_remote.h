#ifndef RCUNITY_REMOTE_H
#define RCUNITY_REMOTE_H

#include "unity.h"

#define HELD_HIGH 0xFFFFFFFF


unsigned int _getRCUnitySkipStartTicks(void);
unsigned int _getRCUnitySkipDoneTicks(void);
unsigned int _getRCUnityEnableCycles(void);
void _setRCUnityClockRate(unsigned int clk);
unsigned int _getRCUnityClockRate(void);
void _setRCUnityHwAddr(unsigned int addr);
unsigned int _getRCUnityHwAddr(void);
unsigned char _getRCUnityTestingFUBlock(void);
void _RCUnityLocalReset(void);
void _RCUnityReset(void);
void _RCUnityClockEn(unsigned int clocks);
void _RCUnityConfigureFlags(unsigned int nflagStart, unsigned int nflagDone, unsigned char testFUBlocks);
long long _RCUnityGetTime(void);



void RCUnityTestFail(const _U_SINT expected,
		       const _U_SINT actual,
		       const UNITY_LINE_TYPE lineNumber,
		       const UNITY_DISPLAY_STYLE_T style);


#ifndef UNITY_SUPPORT_64
#define RCTYPE _U_SINT
#define RCDISPLAY UNITY_DISPLAY_STYLE_INT32
#else
#define RCTYPE _UU64
#define RCDISPLAY UNITY_DISPLAY_STYLE_INT64
#endif

#define TEST_ASSERT_TIME_EQ(expected)    {long long _unitytime = _RCUnityGetTime(); if ((expected - _unitytime) == 0) {} else {RCUnityTestFail((RCTYPE)(expected), (RCTYPE)_unitytime, __LINE__, RCDISPLAY);}}

#define TEST_ASSERT_TIME_GT(expected)    {long long _unitytime = _RCUnityGetTime(); if ((expected - _unitytime) < 0) {} else {RCUnityTestFail((RCTYPE)(expected), (RCTYPE)_unitytime, __LINE__, RCDISPLAY);}}

#define TEST_ASSERT_TIME_GE(expected)    {long long _unitytime = _RCUnityGetTime(); if ((expected - _unitytime) <= 0) {} else {RCUnityTestFail((RCTYPE)(expected), (RCTYPE)_unitytime, __LINE__, RCDISPLAY);}}

#define TEST_ASSERT_TIME_LT(expected)    {long long _unitytime = _RCUnityGetTime(); if ((expected - _unitytime) > 0) {} else {RCUnityTestFail((RCTYPE)(expected), (RCTYPE)_unitytime, __LINE__, RCDISPLAY);}}

#define TEST_ASSERT_TIME_LE(expected)    {long long _unitytime = _RCUnityGetTime(); if ((expected - _unitytime) >= 0) {} else {RCUnityTestFail((RCTYPE)(expected), (RCTYPE)_unitytime, __LINE__, RCDISPLAY);}}


#define RCUNITY_RESET() _RCUnityReset(); 
#define RCUNITY_LOCAL_RESET() _RCUnityLocalReset();

#define RCUNITY_START(clocks) _RCUnityClockEn(clocks);
#define RCUNITY_STOP() _RCUnityClockEn(0x0);

#define RCUNITY_CONF_HW_ADDR(addr) _setRCUnityHwAddr(addr);
#define RCUNITY_CONF_CLOCK_RATE(clk) _setRCUnityClockRate(clk);
#define RCUNITY_CONF_FLAGS(nflagStart, nflagDone) _RCUnityConfigureFlags(nflagStart, nflagDone, 0);
#define RCUNITY_CONF_CFUUT(initialFUBlock, endFUBlock) _RCUnityConfigureFlags(initialFUBlock, endFUBlock, 1);
#define RCUNITY_CONF_FUUT(fu_block) _RCUnityConfigureFlags(fu_block, fu_block, 1);


#endif

