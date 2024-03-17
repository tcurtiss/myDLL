// Test harness for cross-platform shared library framework

// Platform-specific includes and defines
#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define MAKEDLL_FILENAME(base) base ".dll"
#define DLLHANDLE HMODULE
#define LOAD_DLL(name) LoadLibraryA(name)
#define GET_DLL_FUNC(h, name) GetProcAddress(h, name)
#define UNLOAD_DLL(h) FreeLibrary(h)
#elif defined(__linux__)
#include <unistd.h>
#include <dlfcn.h>
#define MAKEDLL_FILENAME(base) "lib" base ".so"
#define DLLHANDLE void*
#define LOAD_DLL(name) dlopen(name, RTLD_LAZY)
#define GET_DLL_FUNC(h, name) dlsym(h, name)
#define UNLOAD_DLL(h) dlclose(h)
#else
#error Unsupported platform
#endif

// Common includes
#include <iostream>

// Shared library header
#include "mydll.h"

#define MYSHAREDLIBNAME MAKEDLL_FILENAME("mydll")

// Simple test harness
int main()
{
	// Try loading the shared library via Win32/POSIX API
	DLLHANDLE handle = LOAD_DLL(MYSHAREDLIBNAME);

	// Check if the shared library was loaded
	if (handle) {
		std::cout << "Loaded shared library " << MYSHAREDLIBNAME << std::endl;
	} else {
		std::cerr << "Failed to load shared library " << MYSHAREDLIBNAME << std::endl;
		return 1; // Error
	}

	// Test the shared library method(s) here...
	mymethod_t mymethod = (mymethod_t)GET_DLL_FUNC(handle, mymethod_name);
	if (mymethod) {
		std::cout << "Calling mymethod():" << std::endl;
		mymethod();
	} else {
		std::cerr << "Failed to get " << mymethod_name << "() from shared library " << MYSHAREDLIBNAME << std::endl;
		return 1; // Error
	}

	// Unload the DLL via Win32/POSIX API
	if (handle) {
		int status = UNLOAD_DLL(handle);
		std::cout << "Unloaded shared library " << MYSHAREDLIBNAME << " with status: " << status << std::endl;
	}
	return 0; // Success
}
