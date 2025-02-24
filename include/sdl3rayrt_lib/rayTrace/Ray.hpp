/*
 * Created by gbian on 22/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../headers.hpp"

namespace sdlrt {

    class Ray {
    public:
        Ray() noexcept;
        Ray(const glm::dvec3 &point1, const glm::dvec3 &point2) noexcept;

        [[nodiscard]] auto getPoint1() const noexcept -> const glm::dvec3 &;
        [[nodiscard]] auto getPoint2() const noexcept -> const glm::dvec3 &;
        [[nodiscard]] auto getLab() const noexcept -> const glm::dvec3 &;

    private:
        glm::dvec3 m_point1{};
        glm::dvec3 m_point2{};
        glm::dvec3 m_lab{};
    };

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)