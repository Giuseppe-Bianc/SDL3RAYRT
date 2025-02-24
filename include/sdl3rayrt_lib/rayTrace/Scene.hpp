/*
 * Created by gbian on 23/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../headers.hpp"
#include "Camera.hpp"
#include "Image.hpp"
#include "ObjSphere.hpp"

namespace sdlrt {

    class Scene {
    public:
        Scene() noexcept;
        ~Scene() = default;
        bool render(Image &image) noexcept;

    private:
        Camera m_camera;
        ObjSphere m_testSphere;
    };

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)