/*
 * Created by gbian on 14/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "headers.hpp"
#include "FPSCounter.hpp"

namespace sdlrt {

    class CApp {
    public:
        CApp() noexcept;

        int OnExecute();
        bool OnInit();
        void OnEvent(const SDL_Event *event) noexcept;
        void OnLoop() noexcept;
        void OnRender() noexcept;
        void OnExit();

    private:
        std::vector<std::uint32_t> m_pixels{};
        // SDL2 stuff.

        //VSync toggle
        bool vsyncEnabled{ false };
        bool isRunning;
        SDL_Window *pWindow;
        SDL_Renderer *pRenderer;
        SDL_Surface *pSurface;
        SDL_Texture *pTexture;
        std::random_device rd;
        std::mt19937 gen{};  // Mersenne Twister RNG

        // Define a uniform distribution for RGB values (0-255)
        std::uniform_int_distribution<int> dist;
    };

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)