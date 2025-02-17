/*
 * Created by gbian on 14/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#include "sdl3rayrt_lib/CApp.hpp"

namespace sdlrt {
    CApp::CApp() noexcept
      : isRunning(true), pWindow(nullptr), pRenderer(nullptr), gen(rd()), dist(std::uniform_int_distribution<int>(0, 255)) {}
    bool CApp::OnInit() {
        vnd::Timer inittimer("init SDL");
        const int result = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);  // NOLINT(readability-implicit-bool-conversion)
        if(result < 0) {                                                // NOLINT(knownConditionTrueFalse)
            LERROR("SDL could not initialize! SDL_Error: {}", SDL_GetError());
            return false;
        }
        LINFO("{}", inittimer);
        vnd::Timer initwtimer("init SDL_window");
        pWindow = SDL_CreateWindow(wtile.data(), wwidth, wheight, SDL_WINDOW_RESIZABLE);
        LINFO("{}", initwtimer);

        if(pWindow != nullptr) {
            LINFO("creazione SDL window di dimensioni(w: {}, h: {})", wwidth, wheight);
            // Initialise the renderer.
            vnd::Timer initrtimer("init SDL_Renderer");
            pRenderer = SDL_CreateRenderer(pWindow, nullptr);
            LINFO("{}", initrtimer);
            if(pRenderer == nullptr) [[unlikely]] {
                LERROR("SDL_CreateRenderer Error: {}", SDL_GetError());
                SDL_DestroyWindow(pWindow);
                SDL_Quit();
                return false;
            }
            vnd::Timer initstimer("init SDL_Surface");
            pSurface = SDL_CreateSurface(wwidth ,wheight, SDL_PIXELFORMAT_RGBA32);
            LINFO("{}", initstimer);
            if(pSurface == nullptr) [[unlikely]] {
                LERROR("SDL_CreateSurface Error: {}", SDL_GetError());
                SDL_DestroyRenderer(pRenderer);
                SDL_DestroyWindow(pWindow);
                SDL_Quit();
                return false;
            }
        } else [[unlikely]] {
            LERROR("SDL_CreateWindow Error: {}", SDL_GetError());
            return false;
        }

        return true;
    }

    int CApp::OnExecute() {
        SDL_Event event{};

        if(OnInit() == false) { return -1; }
        SDL_LockSurface(pSurface);
        Uint32 *pixels = (Uint32 *)pSurface->pixels;
        int pitch =  pSurface->pitch / TypeSizes::sizeOfUint32T;
        const auto *pSurfacepxformat = SDL_GetPixelFormatDetails(pSurface->format);
        if(pSurfacepxformat == nullptr) [[unlikely]] {
            LERROR("SDL_GetPixelFormatDetails Error: {}", SDL_GetError());
            SDL_DestroySurface(pSurface);
            SDL_DestroyRenderer(pRenderer);
            SDL_DestroyWindow(pWindow);
            SDL_Quit();
            return -1;
        }
        for (int j = 0; j < wheight; j++) {
            for (int i = 0; i < wwidth; i++) {
                auto r = C_LD(i) / (wwidth-1);
                auto g = C_LD(j) / (wheight-1);
                auto b = 0;

                auto ir = C_UI8T(255.999L * r);
                auto ig = C_UI8T(255.999L * g);
                auto ib = C_UI8T(255.999L * b);

                pixels[j * pitch + i] = SDL_MapRGBA(pSurfacepxformat,nullptr, ir, ig, ib, 255);
            }
        }
        SDL_UnlockSurface(pSurface);
        vnd::Timer initttimer("init SDL_Texture");
        pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
        LINFO("{}", initttimer);
        if(pTexture == nullptr)  [[unlikely]] {
            LERROR("SDL_CreateTextureFromSurface Error: {}", SDL_GetError());
            SDL_DestroySurface(pSurface);
            SDL_DestroyRenderer(pRenderer);
            SDL_DestroyWindow(pWindow);
            SDL_Quit();
            return -1;
        }
        SDL_DestroySurface(pSurface);
        FPSCounter counter(pWindow, wtile.data());
        while(isRunning) {
            while(SDL_PollEvent(&event) != 0) { OnEvent(&event); }
            counter.frameInTitle(vsyncEnabled, false);
            OnLoop();
            OnRender();
        }

        OnExit();
        return 0;
    }

    void CApp::OnEvent(const SDL_Event *event) noexcept {
        switch(event->type) {
        case SDL_EVENT_QUIT:
            isRunning = false;
            break;
        case SDL_EVENT_KEY_DOWN:
            switch(event->key.key) {
            case SDLK_ESCAPE:
                isRunning = false;
                break;
            case SDLK_SPACE:
                vsyncEnabled = !vsyncEnabled;
                SDL_SetRenderVSync(pRenderer, (vsyncEnabled) ? 1 : SDL_RENDERER_VSYNC_DISABLED);
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
    }

    void CApp::OnLoop() noexcept {}

    void CApp::OnRender() noexcept {
        // Set the background colour to white.
        SDL_RenderClear(pRenderer);
        SDL_RenderTexture(pRenderer, pTexture, nullptr, nullptr);
        SDL_RenderPresent(pRenderer);
        //SDL_Delay(10);
    }

    void CApp::OnExit() {
        vnd::AutoTimer timer{"OnExit"};
        if(pTexture != nullptr) [[likely]] {
            SDL_DestroyTexture(pTexture);
            pTexture = nullptr;
        }
        // Tidy up SDL2 stuff.
        if(pRenderer != nullptr) [[likely]] {
            SDL_DestroyRenderer(pRenderer);
            pRenderer = nullptr;
        }
        if(pWindow != nullptr) [[likely]] {
            SDL_DestroyWindow(pWindow);
            pWindow = nullptr;
        }
        SDL_Quit();
    }

}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)