// Basic example of a DLL/.so cross platform export header (with handy typedefs for the client)
#pragma once

#if defined(_WIN32)
#define DLL_EXPORT_API_AND_TYPEDEF(ret, name, ...) extern "C" __declspec(dllexport) ret __cdecl name(__VA_ARGS__);\
												   typedef ret(__cdecl *name##_t)(__VA_ARGS__);\
												   static const char name##_name[] = #name
#elif defined(__linux__)
#define DLL_EXPORT_API_AND_TYPEDEF(ret, name, ...) extern "C" __attribute__((visibility("default"))) ret name(__VA_ARGS__);\
											     typedef ret(*name##_t)(__VA_ARGS__);\
						         			 static const char name##_name[] = #name
#else
#error Unsupported platform
#endif

// Exports (for the DLL/.so) and corresponding typedefs (for the client)
DLL_EXPORT_API_AND_TYPEDEF(void, mymethod, void);
