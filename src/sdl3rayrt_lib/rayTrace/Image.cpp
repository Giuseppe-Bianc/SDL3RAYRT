/*
 * Created by gbian on 22/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#include "sdl3rayrt_lib/rayTrace/Image.hpp"

DISABLE_WARNINGS_PUSH(26467 26481)

namespace sdlrt {
    Image::Image(int w, int h) : m_xSize(w), m_ySize(h) {
        vnd::Timer initstimer("init SDL_Surface");
        m_pSurface = SDL_CreateSurface(w, h, SDL_PIXELFORMAT_RGBA32);
        LINFO("{}", initstimer);
        if(m_pSurface == nullptr) {
            LERROR("SDL_CreateSurface Error: {}", SDL_GetError());
            return;
        }
        pixels = C_UI32TP(m_pSurface->pixels);
        pitch = m_pSurface->pitch / TypeSizes::sizeOfUint32T;
        m_pSurfacepxformat = SDL_GetPixelFormatDetails(m_pSurface->format);
        if(m_pSurfacepxformat == nullptr) [[unlikely]] {
            LERROR("SDL_GetPixelFormatDetails Error: {}", SDL_GetError());
            SDL_DestroySurface(m_pSurface);
            return;
        }
    }
    /*Image::~Image() {
        SDL_DestroySurface(m_pSurface);
    }*/
    void Image::lockImage() noexcept { SDL_LockSurface(m_pSurface); }
    void Image::unlockImage() noexcept { SDL_UnlockSurface(m_pSurface); }
    SDL_Texture *Image::createTexture(SDL_Renderer *pRenderer) {
        vnd::Timer initttimer("init SDL_Texture");
        SDL_Texture *pTexture = SDL_CreateTextureFromSurface(pRenderer, m_pSurface);
        LINFO("{}", initttimer);
        if(pTexture == nullptr) [[unlikely]] {
            LERROR("SDL_CreateTextureFromSurface Error: {}", SDL_GetError());
            SDL_DestroySurface(m_pSurface);
            return nullptr;
        }
        SDL_DestroySurface(m_pSurface);
        return pTexture;
    }
    void Image::setPixelColor(int x, int y, const double r, const double g, const double b) noexcept {
        auto pos = y * pitch + x;
        const auto ir = C_UI8T(r);
        const auto ig = C_UI8T(g);
        const auto ib = C_UI8T(b);

        pixels[pos] = SDL_MapRGBA(m_pSurfacepxformat, nullptr, ir, ig, ib, 255);
    }

    int Image::getXSize() const noexcept { return m_xSize; }
    int Image::getYSize() const noexcept { return m_ySize; }

}  // namespace sdlrt

DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner)
