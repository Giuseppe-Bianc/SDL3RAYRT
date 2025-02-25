/*
 * Created by gbian on 24/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */

#include "sdl3rayrt_lib/rayTrace/PointLight.hpp"

namespace sdlrt {
    PointLight::PointLight() noexcept {
        m_color = glm::dvec3{1.0, 1.0, 1.0};
        m_intensity = 1.0;
    }

    bool PointLight::ComputeIllumination(const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
                                         [[maybe_unused]] const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                         [[maybe_unused]] const std::shared_ptr<ObjectBase> &currentObject, glm::dvec3 &color,
                                         double &intensity) {
        // Construct a vector pointing from the intersection point to the light.
        const glm::dvec3 lightDir = glm::normalize(m_location - intPoint);

        // Compute a starting point.
        const glm::dvec3 startPoint = intPoint;

        // Compute the angle between the local normal and the light ray.
        // Note that we assume that localNormal is a unit vector.
        double angle = std::acos(glm::dot(localNormal, lightDir));

        // If the normal is pointing away from the light, then we have no illumination.
        if(angle > 1.5708) {
            // No illumination.
            color = m_color;
            intensity = 0.0;
            return false;
        } else {
            // We do have illumination.
            color = m_color;
            intensity = m_intensity * (1.0 - (angle / 1.5708));
            // intensity = 1.0;
            return true;
        }

        return true;
    }

}  // namespace sdlrt
