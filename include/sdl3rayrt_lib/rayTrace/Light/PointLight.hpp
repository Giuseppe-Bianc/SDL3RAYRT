/*
 * Created by gbian on 24/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */

#pragma once

#include "LightBase.hpp"

namespace sdlrt {

    class PointLight : public LightBase {
    public:
        PointLight() noexcept;
        ~PointLight() override = default;
        bool ComputeIllumination(const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
                                 const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                 const std::shared_ptr<ObjectBase> &currentObject, glm::dvec3 &color, double &intensity) override;
    };

}  // namespace sdlrt
