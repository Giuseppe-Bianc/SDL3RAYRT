/*
* Created by gbian on 22/02/2025.
* Copyright (c) 2025 All rights reserved.
*/
// NOLINTBEGIN(*-include-cleaner)
#include "sdl3rayrt_lib/rayTrace/Ray.hpp"

namespace sdlrt {
    Ray::Ray(const glm::dvec3 &point1, const glm::dvec3 &point2) noexcept : m_point1(point1), m_point2(point2) {
        m_lab = m_point2 - m_point1;
    }
    auto Ray::getPoint1() const noexcept -> const glm::dvec3 & { return m_point1; }
    auto Ray::getPoint2() const noexcept -> const glm::dvec3 & { return m_point2; }
    auto Ray::getLab() const noexcept -> const glm::dvec3 & { return m_lab; }

} // sdlrt

// NOLINTEND(*-include-cleaner)