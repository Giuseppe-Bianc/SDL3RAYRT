/*
 * Created by gbian on 23/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../headers.hpp"
#include "Camera.hpp"
#include "Image.hpp"
#include "Light/PointLight.hpp"
#include "Primatives/ObjPlane.hpp"
#include "Primatives/ObjSphere.hpp"

namespace sdlrt {

    class Scene {
    public:
        Scene() noexcept;
        ~Scene() = default;
        Scene(const Scene &other) : m_camera{other.m_camera}, m_objectList{other.m_objectList}, m_lightList{other.m_lightList} {}
        Scene(Scene &&other) noexcept
          : m_camera{std::move(other.m_camera)}, m_objectList{std::move(other.m_objectList)}, m_lightList{std::move(other.m_lightList)} {}
        Scene &operator=(const Scene &other) {
            if(this == &other) return *this;
            m_camera = other.m_camera;
            m_objectList = other.m_objectList;
            m_lightList = other.m_lightList;
            return *this;
        }
        Scene &operator=(Scene &&other) noexcept {
            if(this == &other) return *this;
            m_camera = std::move(other.m_camera);
            m_objectList = std::move(other.m_objectList);
            m_lightList = std::move(other.m_lightList);
            return *this;
        }

        bool render(Image &image) noexcept;

    private:
        Camera m_camera;
        std::vector<std::shared_ptr<ObjectBase>> m_objectList;
        std::vector<std::shared_ptr<LightBase>> m_lightList;
    };

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)
