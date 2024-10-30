#pragma once

#include <algorithm>
#include <iterator>

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

#define SAFE_RUN(code)                                                                      \
    do {                                                                                    \
        try {                                                                               \
            { code }                                                                        \
        } catch (const std::exception& ex) {                                                \
            SIMPLEBLE_LOG_ERROR(fmt::format("Exception within code block: {}", ex.what())); \
        } catch (...) {                                                                     \
            SIMPLEBLE_LOG_ERROR("Unknown exception within code block");                     \
        }                                                                                   \
    } while (0)

namespace SimpleBLE {

template <typename MAP>
struct CollectValues {
  public:
    template <typename VEC>
    operator VEC() {
        VEC vec;
        vec.reserve(map.size());
        std::transform(map.begin(), map.end(), std::back_inserter(vec), [](const auto& pair) { return pair.second; });
        return vec;
    }

    const MAP& map;
};

/**
 * Collect the values of a mapping like object into a vector-like object.
 */
template <typename MAP>
auto collect_values(const MAP& map) {
    return CollectValues<MAP>{map};
}

}  // namespace SimpleBLE
