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
        void computeMaxValues() noexcept;
        int pitch = 0;
        int m_xSize = 0;
        int m_ySize = 0;
        double m_maxRed;
        double m_maxGreen;
        double m_maxBlue;
        double m_overallMax;
        const SDL_PixelFormatDetails *m_pSurfacepxformat = nullptr;
        std::uint32_t *pixels = nullptr;
        SDL_Surface *m_pSurface = nullptr;
    };
    class qbImage {
    public:
        // The constructor.
        qbImage();

        // The destructor.
        ~qbImage();

        // Function to initialise.
        void Initialize(const int xSize, const int ySize, SDL_Renderer *pRenderer);

        // Function to set the colour of a pixel.
        void SetPixel(const int x, const int y, const double red, const double green, const double blue);

        // Function to return the image for display.
        void Display();

        // Functions to return the dimensions of the image.
        int GetXSize();
        int GetYSize();

    private:
        Uint32 ConvertColor(const double red, const double green, const double blue);
        void InitTexture();
        void ComputeMaxValues();

    private:
        // Arrays to store image data.
        std::vector<std::vector<double>> m_rChannel;
        std::vector<std::vector<double>> m_gChannel;
        std::vector<std::vector<double>> m_bChannel;

        // Store the dimensions of the image.
        int m_xSize, m_ySize;

        // Store the maximum values.
        double m_maxRed, m_maxGreen, m_maxBlue, m_overallMax;

        // SDL2 stuff.
        SDL_Renderer *m_pRenderer;
        SDL_Texture *m_pTexture;
        const SDL_PixelFormatDetails *m_pSurfacepxformat = nullptr;
    };
}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)
