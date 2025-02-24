/*
 * Created by gbian on 23/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../headers.hpp"
#include "Ray.hpp"

namespace sdlrt {

    class ObjectBase {
    public:
        ObjectBase() noexcept = default;
        virtual ~ObjectBase() = default;
        // Function to test for intersections.
        virtual bool TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal, glm::dvec3 &localColor);

        // Function to test whether two floating-point numbers are close to being equal.
        bool CloseEnough(const double f1, const double f2) noexcept;
        glm::dvec3 m_baseColor{};
    };

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)
