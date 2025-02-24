/*
 * Created by gbian on 23/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */

#include "sdl3rayrt_lib/rayTrace/ObjectBase.hpp"

namespace sdlrt {

    DISABLE_WARNINGS_PUSH(26440)

    bool ObjectBase::TestIntersection([[maybe_unused]] const Ray &castRay, [[maybe_unused]] glm::dvec3 &intPoint,
                                      [[maybe_unused]] glm::dvec3 &localNormal, [[maybe_unused]] glm::dvec3 &localColor) {
        return false;
    }
    DISABLE_WARNINGS_POP()

    bool ObjectBase::CloseEnough(const double f1, const double f2) noexcept {
        return std::fabs(f1 - f2) < std::numeric_limits<double>::epsilon();
    }
}  // namespace sdlrt
