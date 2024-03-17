// Platform specific includes and defines
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#elif defined(__linux__)
#include <dlfcn.h>
#include <unistd.h>
#include <limits.h>
#endif

// Standard includes
#include <iostream>
#include <cstring>

// Module specific includes
#include "mydll.h"


// Helper struct to keep any DLL/.so metadata needed during lifetime
struct DllInfo {
  DllInfo()
  {
#ifdef __linux__ // Linux specific init (.so has no DllMain)
    // Get .so filename
    Dl_info dlinfo;
    if (dladdr((const void *)this, &dlinfo)) {
      this->filename = dlinfo.dli_fname;
    }
    // Get caller filename
    char caller_filename[PATH_MAX];
    if (readlink("/proc/self/exe", caller_filename, sizeof(caller_filename)) != -1) {
      this->caller_filename = caller_filename;
    }
#endif
  }
  ~DllInfo() {} // No cleanup needed for now
#ifdef _WIN32 // Windows specific init, called from DllMain
  void DllInfoUpdate(HINSTANCE hinstDLL)
  {
    TCHAR tempstr[MAX_PATH];
    if (GetModuleFileNameA(hinstDLL, tempstr, sizeof(tempstr) / sizeof(tempstr[0]))) {
      this->filename = tempstr;
    }
    if (GetModuleFileNameA(NULL, tempstr, sizeof(tempstr) / sizeof(tempstr[0]))) {
      this->caller_filename = tempstr;
    }
  }
#endif

  // Exposed members of interest
  std::string filename;
  std::string caller_filename;
};

static DllInfo dllInfo; // Static class, follows lifetime of DLL/.so

#ifdef _WIN32
BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,  // handle to DLL module
  DWORD fdwReason,     // reason for calling function
  LPVOID lpvReserved)  // reserved
{
  lpvReserved; // Unused
  // Perform actions based on the reason for calling.
  switch (fdwReason) {
    case DLL_PROCESS_ATTACH:
      // Register DLL name and caller name with dllInfo at initial attach
      dllInfo.DllInfoUpdate(hinstDLL);
      break;

    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
      break; // No special handling required
  }
  return TRUE;  // Successful DLL_PROCESS_ATTACH
}
#endif


// Sample method, does very little :)
void mymethod(void)
{
  std::cout << "Caller filepath: " << dllInfo.caller_filename << std::endl;
  std::cout << "DLL    filepath: " << dllInfo.filename << std::endl;
}