#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>


#include "rc-unity.h"



const char RCUnityStrFail[]      = "TIME FAIL";
const char RCUnityStrExpected[]  = " Time Expected ";
const char RCUnityStrWas[]       = " Was ";

unsigned int clockRates[5] = {100,400,200,66,33};

unsigned int _hw_addr = 0x41000000;
unsigned int _nflagStart = 0x0001;
unsigned int _nflagDone = 0x0001;
unsigned int _clocks = 0xFFFFFFFF;
unsigned char _clockRate = 0;
char _time_valid = 0;
long long _time_value = 0;
unsigned char _testFUBlock = 0;


unsigned int
_getRCUnitySkipStartTicks(void)
{
  return _nflagStart;
}


unsigned int
_getRCUnitySkipDoneTicks(void)
{
  return _nflagDone;
}



unsigned int
_getRCUnityEnableCycles(void)
{
  return _clocks;
}

void
_setRCUnityClockRate(unsigned int clk)
{
  unsigned char i;
  
  printf("[INFO] Modified locally\n");
  for(i=0; i!=5; i++)
    if (clockRates[i] == clk){
      _clockRate = i;
      break;
    }
  
  if (i==5){
    printf("[INFO] No clock rate found!! (Default clock rate is set)\n");
    _clockRate = 0; 
  }
}


unsigned int
_getRCUnityClockRate(void)
{
  return clockRates[_clockRate];
}


void
_setRCUnityHwAddr(unsigned int addr)
{
  _hw_addr = addr;
}

unsigned int
_getRCUnityHwAddr(void)
{
  return _hw_addr;
}


unsigned char
_getRCUnityTestingFUBlock(void)
{
  return _testFUBlock;
}



void
_RCUnityLocalReset(void)
{
  _nflagStart = 0x0001;
  _nflagDone = 0x0001;
  _clocks = 0xFFFFFFFF;
  _clockRate = 0;
  _hw_addr = 0x41000000;
  _testFUBlock = 0;
}


void
_RCUnityReset(void)
{  
#ifndef RCUNITY_TEST
  int fd;
  void *ptr;
  unsigned page_size=sysconf(_SC_PAGESIZE);

  int _head;

  fd = open ("/dev/mem", O_RDWR);
  if(fd < 1) {
    printf("Cannot open /dev/mem for writing\n");
    return;
  }

  ptr = mmap(NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, _hw_addr);

  *((unsigned *)ptr) = 0x00010100;
  *((unsigned *)ptr) = 0x00110000;
  
  _head=*((unsigned *)ptr); // head
  
  munmap(ptr, page_size);
  close(fd);
  
  if(_head == 0)
    printf("DummyCheck\n");
#endif
}


void
_RCUnityClockEn(unsigned int clocks)
{  
  _clocks = clocks;
#ifndef RCUNITY_TEST
  int fd;
  void *ptr;
  unsigned page_size=sysconf(_SC_PAGESIZE);

  int _head;

  fd = open ("/dev/mem", O_RDWR);
  if(fd < 1) {
    printf("Cannot open /dev/mem for writing\n");
    return;
  }

  ptr = mmap(NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, _hw_addr);

  *((unsigned *)ptr) = 0x00010204;
  *((unsigned *)ptr) = 0x00110001;
  *((unsigned *)ptr) = _clocks;
  
  _head=*((unsigned *)ptr); // head
  
  munmap(ptr, page_size);
  close(fd);
  
  if(_head == 0)
    printf("DummyCheck\n");
#endif
}


void
_RCUnityConfigureFlags(unsigned int nflagStart, unsigned int nflagDone, unsigned char testFUBlocks)
{
  _time_value = 0;
  _time_valid = 0;
  _nflagStart = nflagStart;
  _nflagDone = nflagDone;
  _testFUBlock = testFUBlocks;

#ifndef RCUNITY_TEST
  int fd;
  void *ptr;
  unsigned page_size=sysconf(_SC_PAGESIZE);

  int _head;


  fd = open ("/dev/mem", O_RDWR);
  if(fd < 1) {
    printf("Cannot open /dev/mem for writing\n");
    return;
  }

  ptr = mmap(NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, _hw_addr);

  *((unsigned *)ptr) = 0x00010304;
  *((unsigned *)ptr) = 0x00110002;
  *((unsigned *)ptr) = _nflagStart;
  *((unsigned *)ptr) = _nflagDone;
  
  _head=*((unsigned *)ptr); // head
  
  munmap(ptr, page_size);
  close(fd);
  
  if(_head == 0)
    printf("DummyCheck\n");
#endif

  return;
}


long long
_RCUnityGetTime(void)
{
  if (_time_valid)
    return _time_value;

#ifndef RCUNITY_TEST
  int fd;
  void *ptr;
  long long _time_value_i;
  unsigned page_size=sysconf(_SC_PAGESIZE);

  int _head, _head2;
  
  fd = open ("/dev/mem", O_RDWR);
  if(fd < 1) {
    printf("Cannot open /dev/mem for writing\n");
    return -1;
  }

  ptr = mmap(NULL, page_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, _hw_addr);

  *((unsigned *)ptr) = 0x00010400;
  *((unsigned *)ptr) = 0x00110000;

  _head=*(unsigned *)ptr; // head
  _head2=*(unsigned *)ptr; // head 2
  unsigned int a = *((unsigned *)ptr);
  unsigned int b = *((unsigned *)ptr);
  long long all = a;
  _time_value_i = (all<<32) | b; 
  
  if (_testFUBlock == 0)
    _time_value = _time_value_i;
  else
    if (_nflagDone == _nflagStart)
      _time_value = _time_value_i;
    else
      _time_value = _time_value_i - (_nflagDone-_nflagStart-1);
       
  munmap(ptr, page_size);
  close(fd);

  if(_head == 0)
    printf("DummyCheck\n");
#else
  if (_testFUBlock == 0)
    _time_value = 10;
  else
    if (_nflagDone == _nflagStart)
      _time_value = 10;
    else
      _time_value = 10 - (_nflagDone-_nflagStart-1);
#endif
  
  _time_valid = 1;
  
  return _time_value; 
}



void RCUnityTestFail( const _U_SINT expected,
			const _U_SINT actual,
			const UNITY_LINE_TYPE lineNumber,
			const UNITY_DISPLAY_STYLE_T style)
{
    UNITY_SKIP_EXECUTION;

    UnityTestResultsFailBegin(lineNumber);
    UnityPrint(RCUnityStrExpected);
    UnityPrintNumberByStyle(expected, style);
    UnityPrint(RCUnityStrWas);
    UnityPrintNumberByStyle(actual, style);
    //UnityAddMsgIfSpecified(msg);
    UNITY_OUTPUT_CHAR('\n');
    UNITY_FAIL_AND_BAIL;
}
