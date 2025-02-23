/*
 * Created by gbian on 23/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#include "sdl3rayrt_lib/rayTrace/Scene.hpp"

namespace sdlrt {
    bool Scene::render(Image &image) noexcept {
        const int xSize = image.getXSize();
        const int ySize = image.getYSize();
        image.lockImage();
        vnd::Timer imgtime("image fill");
        for(int x = 0; x < xSize; ++x) {
            for(int y = 0; y < ySize; ++y) {
                double red = (C_D(x) / xSize) * 255.0;
                double green = (C_D(y) / ySize) * 255.0;
                image.setPixelColor(x, y, red, green, 0.0);
            }
        }
        LINFO("{}", imgtime);
        image.unlockImage();
        return true;
    }

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)