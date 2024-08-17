#pragma once
#include <windows.h>
#include <stdio.h>
#include <psapi.h>

struct DiagnosticInfo {
  DWORDLONG available;
  SIZE_T used;
};

DiagnosticInfo GetMemoryUsage();
void init();
double GetCPUUsage();

 

