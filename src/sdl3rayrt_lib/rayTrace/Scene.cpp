/*
 * Created by gbian on 23/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner, *-math-missing-parentheses, *-avoid-magic-numbers)
#include "sdl3rayrt_lib/rayTrace/Scene.hpp"

namespace sdlrt {
    Scene::Scene() noexcept {
        DISABLE_WARNINGS_PUSH(26447)
        m_camera.SetPosition(glm::dvec3(0.0, -10.0, -1.0));
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
        m_objectList.at(3)->m_baseColor = glm::dvec3{128.0, 128.0, 128.0};

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

        m_objectList.at(0)->m_baseColor = glm::dvec3{64.0, 128.0, 200.0};
        m_objectList.at(1)->m_baseColor = glm::dvec3{255.0, 128.0, 0.0};
        m_objectList.at(2)->m_baseColor = glm::dvec3{255.0, 200.0, 0.0};

        // Construct a test light.
        m_lightList.push_back(MAKE_SHARED(PointLight));
        m_lightList.at(0)->m_location = glm::dvec3{5.0, -10.0, -5.0};
        m_lightList.at(0)->m_color = glm::dvec3{255.0, 255.0, 255.0};
        DISABLE_WARNINGS_POP()
    }

    bool Scene::render(Image &image) noexcept {
        const int xSize = image.getXSize();
        const int ySize = image.getYSize();
        Ray cameraRay;
        glm::dvec3 intPoint{};
        glm::dvec3 localNormal{};
        glm::dvec3 localColor{};
        const double xFact = 1.0 / (C_D(xSize) / 2.0);
        const double yFact = 1.0 / (C_D(ySize) / 2.0);
        double minDist = 1e6;
        double maxDist = 0.0;
        image.lockImage();
        vnd::Timer imgtime("image fill");
        for(int x = 0; x < xSize; ++x) {
            // Normalize the x coordinates.
            double normX = (C_D(x) * xFact) - 1.0;
            for(int y = 0; y < ySize; ++y) {
                // Normalize the y coordinates.
                double normY = (C_D(y) * yFact) - 1.0;

                // Generate the ray for this pixel.
                m_camera.GenerateRay(normX, normY, cameraRay);

                std::shared_ptr<ObjectBase> closestObject;
                glm::dvec3 closestIntPoint{};
                glm::dvec3 closestLocalNormal{};
                glm::dvec3 closestLocalColor{};
                [[maybe_unused]] double minDist2 = 1e6;
                [[maybe_unused]] bool intersectionFound = false;
                for(auto currentObject : m_objectList) {
                    // Test if we have a valid intersection.
                    DISABLE_WARNINGS_PUSH(26447 26496)
                    bool validInt = currentObject->TestIntersection(cameraRay, intPoint, localNormal, localColor);
                    DISABLE_WARNINGS_POP()

                    // If we have a valid intersection
                    if(validInt) {
                        // Compute intensity of illumination.
                        double intensity{};
                        glm::dvec3 color{};
                        bool validIllum = false;
                        DISABLE_WARNINGS_PUSH(26447)
                        for(auto currentLight : m_lightList) {
                            validIllum = currentLight->ComputeIllumination(intPoint, localNormal, m_objectList, currentObject, color,
                                                                           intensity);
                        }

                        // Compute the distance between the camera and the point of intersection.
                        double dist = glm::distance(intPoint, cameraRay.getPoint1());
                        minDist = std::min(minDist, dist);
                        maxDist = std::max(maxDist, dist);
                        DISABLE_WARNINGS_POP()

                        // image.setPixelColor(x, y, 255.0 - ((dist - 9.0) / 0.94605) * 255.0, 0.0, 0.0);
                        if(validIllum) {
                            // image.setPixelColor(x, y, 255.0 * intensity, 0.0, 0.0);
                            image.setPixelColor(x, y, localColor.r * intensity, localColor.g * intensity, localColor.b * intensity);
                        } else {
                            // image.setPixelColor(x, y, 0.0, 0.0, 0.0);
                        }
                    } else {
                        // image.setPixelColor(x, y, 0.0, 0.0, 0.0);
                    }
                }
            }
        }
        DISABLE_WARNINGS_PUSH(26447)
        LINFO("{}", imgtime);
        image.unlockImage();
        LINFO("minDist: {}", minDist);
        LINFO("maxDist: {}", maxDist);
        DISABLE_WARNINGS_POP()

        return true;
    }

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner, *-math-missing-parentheses, *-avoid-magic-numbers)
