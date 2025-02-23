/*
 * Created by gbian on 22/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../headers.hpp"

namespace sdlrt {

    class Image {
    public:
        Image() noexcept = default;
        Image(int w, int h);
        ~Image() = default;
        void lockImage() noexcept;
        void unlockImage() noexcept;
        SDL_Texture *createTexture(SDL_Renderer *pRenderer);
        void setPixelColor(int x, int y, const double r, const double g, const double b) noexcept;
        [[nodiscard]] int getXSize() const noexcept;
        [[nodiscard]] int getYSize() const noexcept;

    private:
        int pitch = 0;
        int m_xSize = 0;
        int m_ySize = 0;
        const SDL_PixelFormatDetails *m_pSurfacepxformat = nullptr;
        std::uint32_t *pixels = nullptr;
        SDL_Surface *m_pSurface = nullptr;
    };
}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)
