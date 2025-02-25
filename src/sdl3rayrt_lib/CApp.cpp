/*
 * Created by gbian on 14/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#include "sdl3rayrt_lib/CApp.hpp"

DISABLE_WARNINGS_PUSH(26447)

namespace sdlrt {
    CApp::CApp() noexcept : isRunning(true), pWindow(nullptr), pRenderer(nullptr) {}
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
            const auto praova = glm::dvec3(1, 2, 3);
            // LINFO("L {}", glm::length(praova));
            // LINFO("L^2 {}", glm::length2(praova));
            m_image = Image(wwidth, wheight);
            m_scene.render(m_image);
            m_imageTexture = m_image.createTexture(pRenderer);
            if(m_imageTexture == nullptr) [[unlikely]] {
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
                LINFO("esc key pressed");
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
        SDL_RenderTexture(pRenderer, m_imageTexture, nullptr, nullptr);

        SDL_RenderPresent(pRenderer);
        // SDL_Delay(10);
    }

    void CApp::OnExit() {
        vnd::AutoTimer timer{"OnExit"};
        if(m_imageTexture != nullptr) [[likely]] {
            SDL_DestroyTexture(m_imageTexture);
            m_imageTexture = nullptr;
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

DISABLE_WARNINGS_POP()
