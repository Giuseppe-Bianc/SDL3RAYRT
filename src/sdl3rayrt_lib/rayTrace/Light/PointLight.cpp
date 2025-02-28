/*
 * Created by gbian on 24/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#include "../../../../include/sdl3rayrt_lib/rayTrace/Light/PointLight.hpp"

namespace sdlrt {
    PointLight::PointLight() noexcept {
        m_color = glm::dvec3{1.0, 1.0, 1.0};
        m_intensity = 1.0;
    }

    bool PointLight::ComputeIllumination(const glm::dvec3 &intPoint, const glm::dvec3 &localNormal,
                                         const std::vector<std::shared_ptr<ObjectBase>> &objectList,
                                         const std::shared_ptr<ObjectBase> &currentObject, glm::dvec3 &color, double &intensity) {
        // Construct a vector pointing from the intersection point to the light.
        glm::dvec3 lightDir = glm::normalize(m_location - intPoint);
        // Compute a starting point.
        glm::dvec3 startPoint = intPoint;

        // Construct a ray from the point of intersection to the light.
        Ray lightRay(startPoint, startPoint + lightDir);

        /* Check for intersections with all of the objects
            in the scene, except for the current one. */
        glm::dvec3 poi{};
        glm::dvec3 poiNormal{};
        glm::dvec3 poiColor{};
        bool validInt = false;
        for(auto sceneObject : objectList) {
            if(sceneObject != currentObject) { validInt = sceneObject->TestIntersection(lightRay, poi, poiNormal, poiColor); }

            /* If we have an intersection, then there is no point checking further
                so we can break out of the loop. In other words, this object is
                blocking light from this light source. */
            if(validInt) break;
        }

        /* Only continue to compute illumination if the light ray didn't
            intersect with any objects in the scene. Ie. no objects are
            casting a shadow from this light source. */
        if(!validInt) {
            // Compute the angle between the local normal and the light ray.
            // Note that we assume that localNormal is a unit vector.
            double angle = std::acos(glm::dot(localNormal, lightDir));

            // If the normal is pointing away from the light, then we have no illumination.
            if(angle > HALF_PID) {
                // No illumination.
                color = m_color;
                intensity = 0.0;
                return false;
            } else {
                // We do have illumination.
                color = m_color;
                intensity = m_intensity * (1.0 - (angle / HALF_PID));
                // intensity = 1.0;
                return true;
            }
        } else {
            // Shadow, so no illumination.
            color = m_color;
            intensity = 0.0;
            return false;
        }
    }

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)