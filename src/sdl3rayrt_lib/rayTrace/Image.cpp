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

    // The default constructor.
    qbImage::qbImage() {
        m_xSize = 0;
        m_ySize = 0;
        m_pTexture = NULL;
        m_pSurfacepxformat = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32);
    }

    // The destructor.
    qbImage::~qbImage() {
        if(m_pTexture != NULL) SDL_DestroyTexture(m_pTexture);
    }

    // Function to inialize.
    void qbImage::Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer) {
        // Resize the image arrays.
        m_rChannel.resize(xSize, std::vector<double>(ySize, 0.0));
        m_gChannel.resize(xSize, std::vector<double>(ySize, 0.0));
        m_bChannel.resize(xSize, std::vector<double>(ySize, 0.0));

        // Store the dimensions.
        m_xSize = xSize;
        m_ySize = ySize;

        // Store the pointer to the renderer.
        m_pRenderer = pRenderer;

        // Initialise the texture.
        InitTexture();
    }

    // Function to set pixels.
    void qbImage::SetPixel(const int x, const int y, const double red, const double green, const double blue) {
        m_rChannel.at(x).at(y) = red;
        m_gChannel.at(x).at(y) = green;
        m_bChannel.at(x).at(y) = blue;
    }

    // Function to return the dimensions of the image.
    int qbImage::GetXSize() { return m_xSize; }
    int qbImage::GetYSize() { return m_ySize; }

    // Function to generate the display.
    void qbImage::Display() {
        // Compute maximum values.
        ComputeMaxValues();

        // Allocate memory for a pixel buffer.
        Uint32 *tempPixels = new Uint32[m_xSize * m_ySize];

        // Clear the pixel buffer.
        memset(tempPixels, 0, m_xSize * m_ySize * sizeof(Uint32));

        for(int x = 0; x < m_xSize; ++x) {
            for(int y = 0; y < m_ySize; ++y) {
                tempPixels[(y * m_xSize) + x] = ConvertColor(m_rChannel[x][y], m_gChannel[x][y], m_bChannel[x][y]);
            }
        }

        // Update the texture with the pixel buffer.
        SDL_UpdateTexture(m_pTexture, NULL, tempPixels, m_xSize * sizeof(Uint32));

        // Delete the pixel buffer.
        delete[] tempPixels;

        // Copy the texture to the renderer.
        SDL_Rect srcRect, bounds;
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = m_xSize;
        srcRect.h = m_ySize;
        bounds = srcRect;
        SDL_RenderTexture(m_pRenderer, m_pTexture, nullptr, nullptr);
    }

    // Function to initialize the texture.
    void qbImage::InitTexture() {
        // Delete any previously created texture.
        if(m_pTexture != nullptr) SDL_DestroyTexture(m_pTexture);

        // Create the texture that will store the image.
        SDL_Surface *tempSurface = SDL_CreateSurface(m_xSize, m_ySize, SDL_PIXELFORMAT_RGBA32);
        m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
        SDL_DestroySurface(tempSurface);
    }

    // Function to convert colours to Uint32
    Uint32 qbImage::ConvertColor(const double red, const double green, const double blue) {
        // Convert the colours to unsigned integers.
        unsigned char r = static_cast<unsigned char>((red / m_overallMax) * 255.0);
        unsigned char g = static_cast<unsigned char>((green / m_overallMax) * 255.0);
        unsigned char b = static_cast<unsigned char>((blue / m_overallMax) * 255.0);

        Uint32 pixelColor = SDL_MapRGBA(m_pSurfacepxformat, nullptr, r, g, b, 255);
        return pixelColor;
    }

    // Function to compute maximum values.
    void qbImage::ComputeMaxValues() {
        m_maxRed = 0.0;
        m_maxGreen = 0.0;
        m_maxBlue = 0.0;
        m_overallMax = 0.0;
        for(int x = 0; x < m_xSize; ++x) {
            for(int y = 0; y < m_ySize; ++y) {
                double redValue = m_rChannel[x][y];
                double greenValue = m_gChannel[x][y];
                double blueValue = m_bChannel[x][y];

                /*if(redValue > m_maxRed) m_maxRed = redValue;

                if(greenValue > m_maxGreen) m_maxGreen = greenValue;

                if(blueValue > m_maxBlue) m_maxBlue = blueValue;*/
                m_maxRed = std::max(m_maxRed, redValue);
                m_maxGreen = std::max(m_maxGreen, greenValue);
                m_maxBlue = std::max(m_maxBlue, blueValue);

                /*if(m_maxRed > m_overallMax) m_overallMax = m_maxRed;

                if(m_maxGreen > m_overallMax) m_overallMax = m_maxGreen;

                if(m_maxBlue > m_overallMax) m_overallMax = m_maxBlue;*/
                m_overallMax = std::max({m_maxRed, m_maxGreen, m_maxBlue});
            }
        }
    }
}  // namespace sdlrt

DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner)
