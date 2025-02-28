/*
 * Created by gbian on 24/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../../headers.hpp"
#include "../Primatives/ObjectBase.hpp"
#include "../Ray.hpp"

namespace sdlrt {

    class LightBase {
    public:
        LightBase() = default;
        virtual ~LightBase() = default;
        DISABLE_WARNINGS_PUSH(26440)
        virtual bool ComputeIllumination(const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
                                         const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                         const std::shared_ptr<ObjectBase> &currentObject, glm::dvec3 &color, double &intensity);
        DISABLE_WARNINGS_POP()

        glm::dvec3 m_color{};
        glm::dvec3 m_location{};
        double m_intensity{};
    };
}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)
