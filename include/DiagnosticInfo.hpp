#pragma once
#pragma push_macro("max")
#pragma push_macro("min")
#include <windows.h>
#pragma pop_macro("max")
#pragma pop_macro("min")
#include <stdio.h>
#include <psapi.h>


struct DiagnosticInfo {
  DWORDLONG available;
  SIZE_T used;
};

DiagnosticInfo GetMemoryUsage();
void init();
double GetCPUUsage();
