/*
 * Created by gbian on 23/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "ObjectBase.hpp"

namespace sdlrt {

    class ObjSphere : public ObjectBase {
    public:
        ObjSphere() = default;
        ~ObjSphere() override = default;
        bool TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal, glm::dvec3 &localColor) override;
    };

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)