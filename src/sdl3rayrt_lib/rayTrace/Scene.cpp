/*
 * Created by gbian on 23/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner, *-math-missing-parentheses, *-avoid-magic-numbers)
#include "sdl3rayrt_lib/rayTrace/Scene.hpp"

namespace sdlrt {
    Scene::Scene() noexcept {
        DISABLE_WARNINGS_PUSH(26447)
        m_camera.SetPosition(glm::dvec3(0.0, -10.0, -2.0));
        m_camera.SetLookAt(glm::dvec3(0.0, 0.0, 0.0));
        m_camera.SetUp(glm::dvec3(0.0, 0.0, 1.0));
        m_camera.SetHorzSize(0.25);
        m_camera.SetAspect(aspectRatio);
        m_camera.UpdateCameraGeometry();

        // Construct a test sphere.
        m_objectList.push_back(MAKE_SHARED(ObjSphere));
        m_objectList.push_back(MAKE_SHARED(ObjSphere));
        m_objectList.push_back(MAKE_SHARED(ObjSphere));
        m_objectList.push_back(MAKE_SHARED(ObjPlane));
        m_objectList.at(3)->m_baseColor = glm::dvec3{0.5, 0.5, 0.5};

        // Define a transform for the plane.
        GTform planeMatrix;
        planeMatrix.SetTransform(glm::dvec3{0.0, 0.0, 0.75}, glm::dvec3{0.0, 0.0, 0.0}, glm::dvec3{4.0, 4.0, 1.0});
        m_objectList.at(3)->SetTransformMatrix(planeMatrix);

        GTform testMatrix1, testMatrix2, testMatrix3;
        testMatrix1.SetTransform(glm::dvec3{-1.5, 0.0, 0.0}, glm::dvec3{0.0, 0.0, 0.0}, glm::dvec3{0.5, 0.5, 0.75});
        testMatrix2.SetTransform(glm::dvec3{0.0, 0.0, 0.0}, glm::dvec3{0.0, 0.0, 0.0}, glm::dvec3{0.75, 0.5, 0.5});
        testMatrix3.SetTransform(glm::dvec3{1.5, 0.0, 0.0}, glm::dvec3{0.0, 0.0, 0.0}, glm::dvec3{0.75, 0.75, 0.75});
        m_objectList.at(0)->SetTransformMatrix(testMatrix1);
        m_objectList.at(1)->SetTransformMatrix(testMatrix2);
        m_objectList.at(2)->SetTransformMatrix(testMatrix3);

        m_objectList.at(0)->m_baseColor = glm::dvec3{0.25, 0.5, 0.8};
        m_objectList.at(1)->m_baseColor = glm::dvec3{1.0, 0.5, 0.0};
        m_objectList.at(2)->m_baseColor = glm::dvec3{1.0, 0.8, 0.0};

        // Construct a test light.
        m_lightList.push_back(MAKE_SHARED(PointLight));
        m_lightList.at(0)->m_location = glm::dvec3{5.0, -10.0, -5.0};
        m_lightList.at(0)->m_color = glm::dvec3{0.0, 0.0, 1.0};

        m_lightList.push_back(MAKE_SHARED(PointLight));
        m_lightList.at(1)->m_location = glm::dvec3{-5.0, -10.0, -5.0};
        m_lightList.at(1)->m_color = glm::dvec3{1.0, 0.0, 0.0};

        m_lightList.push_back(MAKE_SHARED(PointLight));
        m_lightList.at(2)->m_location = glm::dvec3{0.0, -10.0, -5.0};
        m_lightList.at(2)->m_color = glm::dvec3{0.0, 1.0, 0.0};
        DISABLE_WARNINGS_POP()
    }

    bool Scene::render(Image &image) noexcept {
        /*const int xSize = image.getXSize();
        const int ySize = image.getYSize();*/
        const int xSize = image.GetXSize();
        const int ySize = image.GetYSize();
        Ray cameraRay;
        glm::dvec3 intPoint{};
        glm::dvec3 localNormal{};
        glm::dvec3 localColor{};
        const double xFact = 1.0 / (C_D(xSize) / 2.0);
        const double yFact = 1.0 / (C_D(ySize) / 2.0);
        double minDist = 1e6;
        double maxDist = 0.0;
        // image.lockImage();
        vnd::Timer imgtime("image fill");
        for(int y = 0; y < ySize; ++y) {
            // Normalize the y coordinates.
            double normY = (C_D(y) * yFact) - 1.0;
            for(int x = 0; x < xSize; ++x) {
                // Normalize the x coordinates.
                double normX = (C_D(x) * xFact) - 1.0;

                // Generate the ray for this pixel.
                m_camera.GenerateRay(normX, normY, cameraRay);

                std::shared_ptr<ObjectBase> closestObject;
                glm::dvec3 closestIntPoint{};
                glm::dvec3 closestLocalNormal{};
                glm::dvec3 closestLocalColor{};
                double minDist2 = 1e6;
                bool intersectionFound = false;
                for(auto currentObject : m_objectList) {
                    // Test if we have a valid intersection.
                    DISABLE_WARNINGS_PUSH(26447 26496)
                    bool validInt = currentObject->TestIntersection(cameraRay, intPoint, localNormal, localColor);
                    DISABLE_WARNINGS_POP()

                    // If we have a valid intersection
                    // If we have a valid intersection.
                    if(validInt) {
                        // Set the flag to indicate that we found an intersection.
                        intersectionFound = true;

                        // Compute the distance between the camera and the point of intersection.
                        double dist = glm::distance(intPoint, cameraRay.getPoint1());

                        /* If this object is closer to the camera than any one that we have
                            seen before, then store a reference to it. */
                        if(dist < minDist2) {
                            minDist2 = dist;
                            minDist = minDist2;
                            closestObject = currentObject;
                            closestIntPoint = intPoint;
                            closestLocalNormal = localNormal;
                            closestLocalColor = localColor;
                        }
                        maxDist = std::max(maxDist, dist);
                    }
                }

                /* Compute the illumination for the closest object, assuming that there
                    was a valid intersection. */
                if(intersectionFound) {
                    // Compute the intensity of illumination.
                    double intensity;
                    glm::dvec3 color{};
                    double red = 0.0;
                    double green = 0.0;
                    double blue = 0.0;
                    bool validIllum = false;
                    bool illumFound = false;
                    for(auto currentLight : m_lightList) {
                        validIllum = currentLight->ComputeIllumination(closestIntPoint, closestLocalNormal, m_objectList, closestObject,
                                                                       color, intensity);

                        if(validIllum) {
                            illumFound = true;
                            red += color.r * intensity;
                            green += color.g * intensity;
                            blue += color.b * intensity;
                        }
                    }

                    if(illumFound) {
                        red *= closestLocalColor.r;
                        green *= closestLocalColor.g;
                        blue *= closestLocalColor.b;
                        // image.setPixelColor(x, y, red, green, blue);
                        image.SetPixel(x, y, red, green, blue);
                    }
                }
            }
        }
        DISABLE_WARNINGS_PUSH(26447)
        LINFO("{}", imgtime);
        // image.unlockImage();
        LINFO("minDist: {}", minDist);
        LINFO("maxDist: {}", maxDist);
        DISABLE_WARNINGS_POP()

        return true;
    }

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner, *-math-missing-parentheses, *-avoid-magic-numbers)
