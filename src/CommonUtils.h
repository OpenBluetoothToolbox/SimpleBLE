#pragma once

#include "LoggingInternal.h"

#define SAFE_CALLBACK_CALL(cb, ...)                                                           \
    do {                                                                                      \
        if (cb) {                                                                             \
            try {                                                                             \
                cb(__VA_ARGS__);                                                              \
            } catch (const std::exception& ex) {                                              \
                SIMPLEBLE_LOG_ERROR(fmt::format("Exception during callback: {}", ex.what())); \
            } catch (...) {                                                                   \
                SIMPLEBLE_LOG_ERROR("Unknown exception during callback");                     \
            }                                                                                 \
        }                                                                                     \
    } while (0)
