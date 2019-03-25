
#include <setjmp.h>
#include "rc-unity_remote.h"
#include <string.h>


#define EXPECT_ABORT_BEGIN_UTIME \
    if (TEST_PROTECT())    \
    {

#define VERIFY_FAILS_END_UTIME                                                      \
    }                                                                          \
    Unity.CurrentTestFailed = (Unity.CurrentTestFailed == 1) ? 0 : 1;          \
    if (Unity.CurrentTestFailed == 1) {                                        \
      SetToOneMeanWeAlreadyCheckedThisGuy_utime = 1;                                 \
      UnityPrintNumberUnsigned(Unity.CurrentTestLineNumber);                   \
      UNITY_OUTPUT_CHAR(':');                                                  \
      UnityPrint(Unity.CurrentTestName);                                       \
      UnityPrint("[[[[ Previous Test Should Have Failed But Did Not ]]]]");    \
      UNITY_OUTPUT_CHAR('\n');                                                 \
    }

static int SetToOneMeanWeAlreadyCheckedThisGuy_utime;

void testRTimeOnlyOneRequest(void)
{
  TEST_ASSERT_TIME_EQ(10)
}


void testRTimeSeveralRequests(void)
{
  TEST_ASSERT_TIME_LT(20);
  TEST_ASSERT_TIME_LE(20);
  TEST_ASSERT_TIME_LE(10);
  TEST_ASSERT_TIME_GT(5);
  TEST_ASSERT_TIME_GE(5);
  TEST_ASSERT_TIME_GE(10);
}


void testRTimeFail(void)
{
    EXPECT_ABORT_BEGIN_UTIME
    TEST_ASSERT_TIME_EQ(1)
    VERIFY_FAILS_END_UTIME
}


void testRConfigure(void)
{
  RCUNITY_CONF_HW_ADDR(0xBEBECAFE)
  RCUNITY_CONF_FLAGS(1, 2)
  TEST_ASSERT_EQUAL(_getRCUnitySkipStartTicks(), 1);
  TEST_ASSERT_EQUAL(_getRCUnitySkipDoneTicks(), 2);
  TEST_ASSERT_EQUAL(_getRCUnityHwAddr(), 0xBEBECAFE);
  TEST_ASSERT_EQUAL(_getRCUnityTestingFUBlock(), 0);
}


void testConfigureWithClkEn(void)
{
  RCUNITY_CONF_HW_ADDR(0xBEBECAFE)
  RCUNITY_CONF_FLAGS(1, 2)
  RCUNITY_START(200)
  TEST_ASSERT_EQUAL(_getRCUnitySkipStartTicks(), 1);
  TEST_ASSERT_EQUAL(_getRCUnitySkipDoneTicks(), 2);
  TEST_ASSERT_EQUAL(_getRCUnityHwAddr(), 0xBEBECAFE);
  TEST_ASSERT_EQUAL(_getRCUnityEnableCycles(), 200);
}


void testConfigureWithClockSel(void)
{
  RCUNITY_CONF_HW_ADDR(0xBEBECAFE)
  RCUNITY_CONF_FLAGS(1, 2)
  RCUNITY_CONF_CLOCK_RATE(400)
  RCUNITY_START(200)
  TEST_ASSERT_EQUAL(_getRCUnitySkipStartTicks(), 1);
  TEST_ASSERT_EQUAL(_getRCUnitySkipDoneTicks(), 2);
  TEST_ASSERT_EQUAL(_getRCUnityHwAddr(), 0xBEBECAFE);
  TEST_ASSERT_EQUAL(_getRCUnityEnableCycles(), 200);
  TEST_ASSERT_EQUAL(_getRCUnityClockRate(), 400);
}


void testConfigureWithAllClockSel(void)
{
  RCUNITY_CONF_CLOCK_RATE(400)
  TEST_ASSERT_EQUAL(_getRCUnityClockRate(), 400);
  
  RCUNITY_CONF_CLOCK_RATE(200)
  TEST_ASSERT_EQUAL(_getRCUnityClockRate(), 200);
  
  RCUNITY_CONF_CLOCK_RATE(100)
  TEST_ASSERT_EQUAL(_getRCUnityClockRate(), 100);
    
  RCUNITY_CONF_CLOCK_RATE(66)
  TEST_ASSERT_EQUAL(_getRCUnityClockRate(), 66);
  
  RCUNITY_CONF_CLOCK_RATE(33)
  TEST_ASSERT_EQUAL(_getRCUnityClockRate(), 33);
  
  RCUNITY_CONF_CLOCK_RATE(500)
  TEST_ASSERT_EQUAL(_getRCUnityClockRate(), 100);
}



void testRESET(void)
{ 
  RCUNITY_CONF_HW_ADDR(0xBEBECAFE)
  RCUNITY_CONF_FLAGS(1, 2)
  RCUNITY_CONF_CLOCK_RATE(400)
  RCUNITY_LOCAL_RESET()
  RCUNITY_RESET()
  TEST_ASSERT_EQUAL(_getRCUnitySkipStartTicks(), 1);
  TEST_ASSERT_EQUAL(_getRCUnitySkipDoneTicks(), 1);
  TEST_ASSERT_EQUAL(_getRCUnityHwAddr(), 0x41000000);
  TEST_ASSERT_EQUAL(_getRCUnityEnableCycles(), 0xFFFFFFFF);
  TEST_ASSERT_EQUAL(_getRCUnityClockRate(), 100);
  TEST_ASSERT_EQUAL(_getRCUnityTestingFUBlock(), 0);
}



void testConfigureFUBlock(void)
{
  RCUNITY_CONF_FUUT(2)
  TEST_ASSERT_EQUAL(_getRCUnitySkipStartTicks(), 2);
  TEST_ASSERT_EQUAL(_getRCUnitySkipDoneTicks(), 2);
  TEST_ASSERT_EQUAL(_getRCUnityTestingFUBlock(), 1);
}


void testConfigureFUBlocks(void)
{
  RCUNITY_CONF_CFUUT(1,2)
  TEST_ASSERT_EQUAL(_getRCUnitySkipStartTicks(), 1);
  TEST_ASSERT_EQUAL(_getRCUnitySkipDoneTicks(), 2);
  TEST_ASSERT_EQUAL(_getRCUnityTestingFUBlock(), 1);
  
  RCUNITY_CONF_CFUUT(2,2)
  TEST_ASSERT_EQUAL(_getRCUnitySkipStartTicks(), 2);
  TEST_ASSERT_EQUAL(_getRCUnitySkipDoneTicks(), 2);
  TEST_ASSERT_EQUAL(_getRCUnityTestingFUBlock(), 1);
}


void testAssertions(void)
{
  TEST_ASSERT_TIME_EQ(10);
  TEST_ASSERT_TIME_LE(10);
  TEST_ASSERT_TIME_GE(10);
  TEST_ASSERT_TIME_LE(15);
  TEST_ASSERT_TIME_GE(5);
  TEST_ASSERT_TIME_LT(11);
  TEST_ASSERT_TIME_GT(9);
}


void testAssertionsBlocks(void)
{
  RCUNITY_CONF_FUUT(2)
  TEST_ASSERT_TIME_EQ(10)
  
  RCUNITY_CONF_CFUUT(1,2)
  TEST_ASSERT_TIME_EQ(10)
}




