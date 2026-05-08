#pragma once


#define DEBUG 0
#if DEBUG == 1
  #define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
  #define DEBUG_PRINT(format, ...) // Do nothing //
#endif
