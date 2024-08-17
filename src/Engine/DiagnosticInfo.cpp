#include <DiagnosticInfo.hpp>

static ULARGE_INTEGER lastCPU, lastSysCPU, lastUserCPU;
static int numProcessors;
static HANDLE self;

//

DiagnosticInfo GetMemoryUsage( ) {
  MEMORYSTATUSEX memInfo;
  memInfo.dwLength = sizeof(MEMORYSTATUSEX);
  GlobalMemoryStatusEx(&memInfo);
  PROCESS_MEMORY_COUNTERS_EX pmc;
  GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
  DWORDLONG physMemAvail = memInfo.ullAvailPhys;
  SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
  
  return DiagnosticInfo{ physMemAvail /(u_long)(1024.0 * 1024.0), physMemUsedByMe/(u_long)(1024.0 * 1024.0)};
}


void init(){
  SYSTEM_INFO sysInfo;
  FILETIME ftime, fsys, fuser;

  GetSystemInfo(&sysInfo);
  numProcessors = sysInfo.dwNumberOfProcessors;

  GetSystemTimeAsFileTime(&ftime);
  memcpy(&lastCPU, &ftime, sizeof(FILETIME));

  self = GetCurrentProcess();
  GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
  memcpy(&lastSysCPU, &fsys, sizeof(FILETIME));
  memcpy(&lastUserCPU, &fuser, sizeof(FILETIME));
}

double GetCPUUsage( ){
  FILETIME ftime, fsys, fuser;
  ULARGE_INTEGER now, sys, user;
  double percent;

  GetSystemTimeAsFileTime(&ftime);
  memcpy(&now, &ftime, sizeof(FILETIME));

  GetProcessTimes(self, &ftime, &ftime, &fsys, &fuser);
  memcpy(&sys, &fsys, sizeof(FILETIME));
  memcpy(&user, &fuser, sizeof(FILETIME));
  percent = (sys.QuadPart - lastSysCPU.QuadPart) +
      (user.QuadPart - lastUserCPU.QuadPart);
  percent /= (now.QuadPart - lastCPU.QuadPart);
  percent /= numProcessors;
  lastCPU = now;
  lastUserCPU = user;
  lastSysCPU = sys;

  return percent * 100;
} 