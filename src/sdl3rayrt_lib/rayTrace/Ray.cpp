/*
 * Created by gbian on 22/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#include "sdl3rayrt_lib/rayTrace/Ray.hpp"

namespace sdlrt {
    Ray::Ray() noexcept : m_point1(glm::dvec3{0.0, 0.0, 0.0}), m_point2(glm::dvec3{0.0, 0.0, 1.0}) { m_lab = m_point2 - m_point1; }
    Ray::Ray(const glm::dvec3 &point1, const glm::dvec3 &point2) noexcept : m_point1(point1), m_point2(point2) {
        m_lab = m_point2 - m_point1;
    }
    auto Ray::getPoint1() const noexcept -> const glm::dvec3 & { return m_point1; }
    auto Ray::getPoint2() const noexcept -> const glm::dvec3 & { return m_point2; }
    auto Ray::getLab() const noexcept -> const glm::dvec3 & { return m_lab; }

    void Ray::setPoint1(const glm::dvec3 &point1) noexcept { this->m_point1 = point1; }
    void Ray::setPoint2(const glm::dvec3 &point2) noexcept { this->m_point2 = point2; }
    void Ray::setLab(const glm::dvec3 &lab) noexcept { this->m_lab = lab; }

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)
/*
AVX
AVX2
AVX512F
SSE
SSE2
SSE3
SSE4_1
SSE4_2
MMX
 */