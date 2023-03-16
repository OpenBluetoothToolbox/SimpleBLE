#pragma once

#include <simpleble/export.h>

#include <simpleble_c/types.h>

#ifdef __cplusplus
extern "C" {
#endif

/** Returns the host operating system. */
SIMPLEBLE_EXPORT simpleble_os_t get_operating_system(void);

#ifdef __cplusplus
}
#endif
