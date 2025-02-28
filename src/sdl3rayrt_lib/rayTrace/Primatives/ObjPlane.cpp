/*
 * Created by gbian on 25/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#include "sdl3rayrt_lib/rayTrace/Primatives/ObjPlane.hpp"

namespace sdlrt {
    bool ObjPlane::TestIntersection(const Ray &castRay, glm::dvec3 &intPoint, glm::dvec3 &localNormal, glm::dvec3 &localColor) {
        Ray bckRay = m_transformMatrix.Apply(castRay, BCKTFORM);

        // Copy the m_lab vector from bckRay and normalize it.
        glm::dvec3 k = glm::normalize(bckRay.getLab());

        /* Check if there is an intersection, ie. if the castRay is not parallel
            to the plane. */
        if(!CloseEnough(k[2], 0.0)) {
            // There is an intersection.
            double t = bckRay.getPoint1()[2] / -k[2];

            /* If t is negative, then the intersection point must be behind
                the camera and we can ignore it. */
            if(t > 0.0) {
                // Compute the values for u and v.
                double u = bckRay.getPoint1()[0] + (k[0] * t);
                double v = bckRay.getPoint1()[1] + (k[1] * t);

                /* If the magnitude of both u and v is less than or equal to one
                    then we must be in the plane. */
                if((abs(u) < 1.0) && (abs(v) < 1.0)) {
                    // Compute the point of intersection.
                    glm::dvec3 poi = bckRay.getPoint1() + t * k;

                    // Transform the intersection point back into world coordinates.
                    intPoint = m_transformMatrix.Apply(poi, FWDTFORM);

                    // Compute the local normal.
                    glm::dvec3 localOrigin{0.0, 0.0, 0.0};
                    glm::dvec3 normalVector{0.0, 0.0, -1.0};
                    glm::dvec3 globalOrigin = m_transformMatrix.Apply(localOrigin, FWDTFORM);
                    localNormal = glm::normalize(m_transformMatrix.Apply(normalVector, FWDTFORM) - globalOrigin);

                    // Return the base color.
                    localColor = m_baseColor;

                    return true;
                } else {
                    return false;
                }
            } else {
                return false;
            }
        }

        return false;
    }

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)
