/*
 * Created by gbian on 23/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */

// NOLINTBEGIN(*-include-cleaner)
#include "sdl3rayrt_lib/rayTrace/ObjSphere.hpp"

DISABLE_WARNINGS_PUSH(26496)

namespace sdlrt {
    bool ObjSphere::TestIntersection(const Ray &castRay, [[maybe_unused]] glm::dvec3 &intPoint, [[maybe_unused]] glm::dvec3 &localNormal,
                                     [[maybe_unused]] glm::dvec3 &localColor) {
        // Compute the values of a, b and c.
        glm::dvec3 vhat = glm::normalize(castRay.getLab());
        // vhat = glm::normalize(vhat);

        /* Note that a is equal to the squared magnitude of the
            direction of the cast ray. As this will be a unit vector,
            we can conclude that the value of 'a' will always be 1. */
        // a = 1.0;

        // Calculate b.
        double b = 2.0 * glm::dot(castRay.getPoint1(), vhat);

        // Calculate c.
        double c = glm::dot(castRay.getPoint1(), castRay.getPoint1()) - 1.0;

        // Test whether we actually have an intersection.
        double intTest = (b * b) - 4.0 * c;

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
                    intPoint = castRay.getPoint1() + (vhat * t1);
                } else {
                    intPoint = castRay.getPoint1() + (vhat * t2);
                }
            }

            return true;
        } else {
            return false;
        }
    }

}  // namespace sdlrt

DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner)
