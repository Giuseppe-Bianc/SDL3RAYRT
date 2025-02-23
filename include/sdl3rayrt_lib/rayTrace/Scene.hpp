/*
 * Created by gbian on 23/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../headers.hpp"
#include "Image.hpp"

namespace sdlrt {

    class Scene {
    public:
        Scene() noexcept = default;
        ~Scene() = default;
        bool render(Image &image) noexcept;

    private:
    };

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)