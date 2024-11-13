#pragma once

#if SIMPLEBLE_BACKEND_PLAIN == 1
#include "../plain/AdapterPlain.h"
#elif SIMPLEBLE_BACKEND_LINUX == 1
#include "../linux/AdapterLinux.h"
#elif SIMPLEBLE_BACKEND_WINDOWS == 1
#include "../windows/AdapterWindows.h"
#elif SIMPLEBLE_BACKEND_MACOS == 1
#include "../macos/AdapterMac.h"
#elif SIMPLEBLE_BACKEND_ANDROID == 1
#include "../android/AdapterAndroid.h"
#endif
