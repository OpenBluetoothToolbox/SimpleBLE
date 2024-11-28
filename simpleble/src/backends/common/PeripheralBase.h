#pragma once

#if SIMPLEBLE_BACKEND_PLAIN == 1
#include "../plain/PeripheralPlain.h"
#elif SIMPLEBLE_BACKEND_LINUX == 1
#include "../linux/PeripheralLinux.h"
#elif SIMPLEBLE_BACKEND_WINDOWS == 1
#include "../windows/PeripheralWindows.h"
#elif SIMPLEBLE_BACKEND_MACOS == 1
#include "../macos/PeripheralMac.h"
#elif SIMPLEBLE_BACKEND_ANDROID == 1
#include "../android/PeripheralAndroid.h"
#endif
