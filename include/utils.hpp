#pragma once

#define DEBUG 0
#if DEBUG == 1
  #define DEBUG_PRINT(format, ...) printf(format, ##__VA_ARGS__)
#else
  #define DEBUG_PRINT(format, ...) // Do nothing //
#endif

struct WINDOW_SETTINGS {
  float MAX_FRAME_RATE; // max frame rate of this window
  uint16_t DEFAULT_WINDOW_SIZE_X; // 
  uint16_t DEFAULT_WINDOW_SIZE_Y;
  uint16_t WORLD_SIZE; // always square
  std::string WINDOW_NAME;
};

struct UI_SETTINGS {
  uint16_t h1_size; // 50
  uint16_t h2_size; // 35
  uint16_t h3_size; // 25
  uint16_t p1_size; // 20
};