/*
 * Created by gbian on 24/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#include "sdl3rayrt_lib/rayTrace/Light/LightBase.hpp"

namespace sdlrt {
    DISABLE_WARNINGS_PUSH(26440)
    bool LightBase::ComputeIllumination([[maybe_unused]] const glm::dvec3 &intPoint, [[maybe_unused]] const glm::dvec3 &localNormal,
                                        [[maybe_unused]] const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                        [[maybe_unused]] const std::shared_ptr<ObjectBase> &currentObject,
                                        [[maybe_unused]] glm::dvec3 &color, [[maybe_unused]] double &intensity) {
        return false;
    }
    DISABLE_WARNINGS_POP()
}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)
