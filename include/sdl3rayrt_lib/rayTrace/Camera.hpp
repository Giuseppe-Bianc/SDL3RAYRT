/*
 * Created by gbian on 23/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../headers.hpp"
#include "Ray.hpp"

namespace sdlrt {

    class Camera {
    public:
        Camera() noexcept;
        ~Camera() = default;
        void SetPosition(const glm::dvec3 &newPosition) noexcept;
        void SetLookAt(const glm::dvec3 &newLookAt) noexcept;
        void SetUp(const glm::dvec3 &upVector) noexcept;
        void SetLength(double newLength) noexcept;
        void SetHorzSize(double newSize) noexcept;
        void SetAspect(double newAspectRatio) noexcept;

        glm::dvec3 GetPosition() const noexcept;
        glm::dvec3 GetLookAt() const noexcept;
        glm::dvec3 GetUp() const noexcept;
        glm::dvec3 GetU() const noexcept;
        glm::dvec3 GetV() const noexcept;
        glm::dvec3 GetScreenCentre() const noexcept;
        double GetLength() const noexcept;
        double GetHorzSize() const noexcept;
        double GetAspect() const noexcept;

        bool GenerateRay(double proScreenX, double proScreenY, Ray &ray) noexcept;

        void UpdateCameraGeometry() noexcept;

    private:
        glm::dvec3 m_cameraPosition{};
        glm::dvec3 m_cameraLookAt{};
        glm::dvec3 m_cameraUp{};
        double m_cameraLength;
        double m_cameraHorzSize;
        double m_cameraAspectRatio;

        glm::dvec3 m_alignmentVector{};
        glm::dvec3 m_projectionScreenU{};
        glm::dvec3 m_projectionScreenV{};
        glm::dvec3 m_projectionScreenCentre{};
    };

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)
