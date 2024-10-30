#pragma once

#include "BuilderBase.h"

namespace SimpleBLE {

template <typename V1>
struct BuildVecDeduce {
    template <typename T>
    operator T() && {
        T vec;
        vec.reserve(objs.size());
        for (auto& obj : objs) {
            vec.push_back(build(std::move(obj)));
        }
        return vec;
    }

    V1 objs;
};

template <typename V1>
auto build_vec(V1 objs) {
    return BuildVecDeduce<V1>{std::move(objs)};
}

}  // namespace SimpleBLE
