/*
 * Created by gbian on 23/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */

// NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters)
#include "sdl3rayrt_lib/rayTrace/ObjSphere.hpp"

DISABLE_WARNINGS_PUSH(26496)

namespace sdlrt {
    bool ObjSphere::TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal, glm::dvec3 &localColor) {
        // Copy the ray and apply the backwards transform.
        Ray bckRay = m_transformMatrix.Apply(castRay, BCKTFORM);

        // Compute the values of a, b and c.
        glm::dvec3 vhat = glm::normalize(bckRay.getLab());

        /* Note that a is equal to the squared magnitude of the
            direction of the cast ray. As this will be a unit vector,
            we can conclude that the value of 'a' will always be 1. */
        // a = 1.0;

        // Calculate b.
        double b = 2.0 * glm::dot(bckRay.getPoint1(), vhat);

        // Calculate c.
        double c = glm::dot(bckRay.getPoint1(), bckRay.getPoint1()) - 1.0;

        // Test whether we actually have an intersection.
        double intTest = (b * b) - 4.0 * c;

        glm::dvec3 poi{};
        if(intTest > 0.0) {
            double numSQRT = std::sqrt(intTest);
            double t1 = (-b + numSQRT) / 2.0;
            double t2 = (-b - numSQRT) / 2.0;

            /* If either t1 or t2 are negative, then at least part of the object is
                behind the camera and so we will ignore it. */
            if((t1 < 0.0) || (t2 < 0.0)) {
                return false;
            } else {
                // Determine which point of intersection was closest to the camera.
                if(t1 < t2) {
                    poi = bckRay.getPoint1() + (vhat * t1);
                } else {
                    poi = bckRay.getPoint1() + (vhat * t2);
                }

                // Transform the intersection point back into world coordinates.
                intPoint = m_transformMatrix.Apply(poi, FWDTFORM);

                // Compute the local normal (easy for a sphere at the origin!).
                glm::dvec3 objOrigin = glm::dvec3{0.0, 0.0, 0.0};
                glm::dvec3 newObjOrigin = m_transformMatrix.Apply(objOrigin, FWDTFORM);
                localNormal = glm::normalize(intPoint - newObjOrigin);

                // Return the base color.
                localColor = m_baseColor;
            }

            return true;
        } else {
            return false;
        }
    }

}  // namespace sdlrt

DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner, *-easily-swappable-parameters)
