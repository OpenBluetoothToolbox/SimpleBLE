#pragma once

#include <utility>

namespace SimpleBLE {

/**
 * Generic builder class for classes that take a single argument in their constructor.
 *
 * Normally, the base class's non-default constructor will not exist or will be
 * protected, so that the user cannot directly instantiate the base class.
 *
 * It is possible to specialize this class for classes that are constructed
 * differently.
 */
template <typename T>
struct Builder : public T {
    template <typename ARG>
    Builder(ARG&& internal) { this->internal_ = std::move(internal); }
};

/**
 * Helper class to deduce the return type of the build function.
 */
template <typename ARG>
struct BuildDeduce {
    template <typename T>
    operator T() && {
        return Builder<T>(std::move(arg));
    }
    ARG arg;
};

/**
 * Build a new object which is not directly constructible.
 */
template <typename ARG>
auto build(ARG x) {
    return BuildDeduce<ARG>{std::move(x)};
}

}  // namespace SimpleBLE
