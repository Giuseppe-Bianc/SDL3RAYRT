/*
 * Created by gbian on 14/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "FPSCounter.hpp"
#include "headers.hpp"
#include "rayTrace/Image.hpp"

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

        // VSync toggle
        bool vsyncEnabled{false};
        bool isRunning;
        SDL_Window *pWindow;
        SDL_Renderer *pRenderer;
        SDL_Texture *m_imageTexture = nullptr;
        Image m_image;
    };

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)
