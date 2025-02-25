/*
 * Created by gbian on 23/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#include "sdl3rayrt_lib/rayTrace/Camera.hpp"

namespace sdlrt {
    Camera::Camera() noexcept {
        m_cameraPosition = glm::dvec3{0.0, -10.0, 0.0};
        m_cameraLookAt = glm::dvec3{0.0, 0.0, 0.0};
        m_cameraUp = glm::dvec3{0.0, 0.0, 1.0};
        m_cameraLength = 1.0;
        m_cameraHorzSize = 1.0;
        m_cameraAspectRatio = 1.0;
    }

    void Camera::SetPosition(const glm::dvec3 &newPosition) noexcept { m_cameraPosition = newPosition; }

    void Camera::SetLookAt(const glm::dvec3 &newLookAt) noexcept { m_cameraLookAt = newLookAt; }

    void Camera::SetUp(const glm::dvec3 &upVector) noexcept { m_cameraUp = upVector; }

    void Camera::SetLength(double newLength) noexcept { m_cameraLength = newLength; }

    void Camera::SetHorzSize(double newHorzSize) noexcept { m_cameraHorzSize = newHorzSize; }

    void Camera::SetAspect(double newAspect) noexcept { m_cameraAspectRatio = newAspect; }

    // Method to return the position of the camera.
    glm::dvec3 Camera::GetPosition() const noexcept { return m_cameraPosition; }

    // Method to return the look at of the camera.
    glm::dvec3 Camera::GetLookAt() const noexcept { return m_cameraLookAt; }

    // Method to return the up vector of the camera.
    glm::dvec3 Camera::GetUp() const noexcept { return m_cameraUp; }

    // Method to return the length of the camera.
    double Camera::GetLength() const noexcept { return m_cameraLength; }

    double Camera::GetHorzSize() const noexcept { return m_cameraHorzSize; }

    double Camera::GetAspect() const noexcept { return m_cameraAspectRatio; }

    glm::dvec3 Camera::GetU() const noexcept { return m_projectionScreenU; }

    glm::dvec3 Camera::GetV() const noexcept { return m_projectionScreenV; }

    glm::dvec3 Camera::GetScreenCentre() const noexcept { return m_projectionScreenCentre; }

    DISABLE_WARNINGS_PUSH(26447)

    void Camera::UpdateCameraGeometry() noexcept {
        // First, compute the vector from the camera position to the LookAt position.
        m_alignmentVector = glm::normalize(m_cameraLookAt - m_cameraPosition);

        // Second, compute the alpha and beta unit vectors.
        m_projectionScreenU = glm::normalize(glm::cross(m_alignmentVector, m_cameraUp));
        m_projectionScreenV = glm::normalize(glm::cross(m_projectionScreenU, m_alignmentVector));

        // Thirdly, compute the position of the center point of the screen.
        m_projectionScreenCentre = m_cameraPosition + (m_cameraLength * m_alignmentVector);

        // Modify the U and V vectors to match the size and aspect ratio.
        m_projectionScreenU = m_projectionScreenU * m_cameraHorzSize;
        m_projectionScreenV = m_projectionScreenV * (m_cameraHorzSize / m_cameraAspectRatio);
    }
    DISABLE_WARNINGS_POP()

    DISABLE_WARNINGS_PUSH(26496)

    bool Camera::GenerateRay(double proScreenX, double proScreenY, Ray &ray) noexcept {
        // Compute the location of the screen point in world coordinates.
        glm::dvec3 screenWorldPart1 = m_projectionScreenCentre + (m_projectionScreenU * proScreenX);
        glm::dvec3 screenWorldCoordinate = screenWorldPart1 + (m_projectionScreenV * proScreenY);

        // Use this point along with the camera position to compute the ray.
        ray = Ray(m_cameraPosition, screenWorldCoordinate);
        return true;
    }
    DISABLE_WARNINGS_POP()
}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)
