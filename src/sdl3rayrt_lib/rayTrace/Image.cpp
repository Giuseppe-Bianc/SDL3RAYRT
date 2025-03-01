/*
 * Created by gbian on 22/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters, *-identifier-length)
#include "sdl3rayrt_lib/rayTrace/Image.hpp"

DISABLE_WARNINGS_PUSH(26467 26481)

namespace sdlrt {
    inline static constexpr double maxrgb = 255.0;
    inline static constexpr std::uint8_t maxrgbi = C_UI8T(maxrgb);
    // The default constructor.
    Image::Image()
      : m_xSize(0), m_ySize(0), m_pRenderer(nullptr), m_pTexture(nullptr),
        m_pSurfacepxformat(SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_RGBA32)) {}

    // The destructor.
    Image::~Image() {
        if(m_pTexture != nullptr) { SDL_DestroyTexture(m_pTexture); }
    }

    // Function to initialize.
    void Image::Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer) {
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
    void Image::SetPixel(const int x, const int y, const double red, const double green, const double blue) {
        m_rChannel[x][y] = red;
        m_gChannel[x][y] = green;
        m_bChannel[x][y] = blue;
    }
    // clang-format on

    // Function to return the dimensions of the image.
    int Image::GetXSize() const noexcept { return m_xSize; }
    int Image::GetYSize() const noexcept { return m_ySize; }

    // Function to generate the display.
    void Image::Display() {
        // If ComputeMaxValues() isn't needed for color conversion, remove it
        ComputeMaxValues();  // Only keep if absolutely necessary

        // Use RAII container for automatic memory management
        std::vector<std::uint32_t> tempPixels(m_xSize * m_ySize);

        // Optimized loop ordering for cache efficiency
        std::uint32_t *pixelBuffer = tempPixels.data();

        // Flip loop order to y-first for better memory access patterns
        for(int y = 0; y < m_ySize; ++y) {
            for(int x = 0; x < m_xSize; ++x) {
                // Calculate index once per iteration
                const size_t index = (y * m_xSize) + x;
                pixelBuffer[index] = ConvertColor(m_rChannel[x][y], m_gChannel[x][y], m_bChannel[x][y]);
            }
        }

        // Update texture with proper pitch calculation
        SDL_UpdateTexture(m_pTexture, nullptr, pixelBuffer, C_ST(m_xSize) * TypeSizes::sizeOfUint32T);

        // Render directly without creating unnecessary rects
        SDL_RenderTexture(m_pRenderer, m_pTexture, nullptr, nullptr);
    }

    // Function to initialize the texture.
    void Image::InitTexture() {
        // Delete any previously created texture.
        if(m_pTexture != nullptr) { SDL_DestroyTexture(m_pTexture); }

        // Create the texture that will store the image.
        SDL_Surface *tempSurface = SDL_CreateSurface(m_xSize, m_ySize, SDL_PIXELFORMAT_RGBA32);
        m_pTexture = SDL_CreateTextureFromSurface(m_pRenderer, tempSurface);
        SDL_DestroySurface(tempSurface);
    }

    // Function to convert colours to Uint32
    Uint32 Image::ConvertColor(const double red, const double green, const double blue) {
        // Convert the colours to unsigned integers.
        auto r = C_UC((red / m_overallMax) * maxrgb);
        auto g = C_UC((green / m_overallMax) * maxrgb);
        auto b = C_UC((blue / m_overallMax) * maxrgb);

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        Uint32 pixelColor = SDL_MapRGBA(m_pSurfacepxformat, nullptr, r, g, b, maxrrgbi);
#else
        Uint32 pixelColor = SDL_MapRGBA(m_pSurfacepxformat, nullptr, b, g, r, maxrgbi);
#endif
        return pixelColor;
    }

    // Function to compute maximum values.
    void Image::ComputeMaxValues() {
        m_maxRed = 0.0;
        m_maxGreen = 0.0;
        m_maxBlue = 0.0;
        m_overallMax = 0.0;
        for(int y = 0; y < m_ySize; ++y) {
            for(int x = 0; x < m_xSize; ++x) {
                double redValue = m_rChannel[x][y];
                double greenValue = m_gChannel[x][y];
                double blueValue = m_bChannel[x][y];
                m_maxRed = std::max(m_maxRed, redValue);
                m_maxGreen = std::max(m_maxGreen, greenValue);
                m_maxBlue = std::max(m_maxBlue, blueValue);
                m_overallMax = std::max({m_maxRed, m_maxGreen, m_maxBlue});
            }
        }
    }
}  // namespace sdlrt

DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner, *-easily-swappable-parameters, *-identifier-length)