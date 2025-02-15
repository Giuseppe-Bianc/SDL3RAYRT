/*
 * Created by gbian on 14/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "sdl3rayrt_core_lib/sdl3rayrt_core.hpp"

static inline constexpr std::string_view wtile = SDL3RAYRT::cmake::project_name;
static inline constexpr std::size_t factor = 80;
static inline constexpr auto wwidth = 16 * factor;
static inline constexpr auto wheight = 9 * factor;
static inline constexpr std::uint8_t MAX_COLOR_VALUE = 255;
static inline constexpr auto ALPHA_VALUE = MAX_COLOR_VALUE;
static inline constexpr auto ALPHA_VALUEI = C_I(MAX_COLOR_VALUE);

// Example usage:
// SDL_Color color = {255, 165, 0, 255};
// std::string s1 = fmt::format("{}", color);                // Default format: (r:255, g:165, b:0, a:255)
// std::string s2 = fmt::format("{:x}", color);              // Uppercase hex format: #FFA500FF
// std::string s3 = fmt::format("{:X}", color);              // Lowercase hex format: #ffa500ff
// std::string s4 = fmt::format("{:R}", color);              // RGB format: FFA500
// std::string s5 = fmt::format("{:a}", color);              // ARGB format: FFA5FF00
// std::string s6 = fmt::format("{:b}", color);              // BGRA format: 00FFA5FF
// std::string s7 = fmt::format("Grayscale: {g}\n", color);  // Grayscale: #805050
// std::string s8 = fmt::format("HSL: {h}\n", color);        // HSL: hsl(30,75.38%,66.67%)
template <> struct fmt::formatter<SDL_Color> : formatter<std::string_view> {
    // Default presentation type is 'f' for full text.
    char presentation = 'f';

    // Parse the format specifier. Accepts one character: 'f', 'h', 't', or 'j'.
    constexpr auto parse(format_parse_context &ctx) -> decltype(ctx.begin()) {
        auto it = ctx.begin(), end = ctx.end();
        if(it != end && *it != '}') {
            // Read the presentation character.
            switch(*it) {
            case 'x':
            case 'X':
            case 'r':
            case 'R':
            case 'a':
            case 'b':
            case 'g':
                presentation = *it++;
                break;
            default:
                break;
            }
        }
        if(it != end && *it != '}') { throw fmt::format_error("invalid format"); }
        return it;
    }

    int grayscaleFromRGBA(const SDL_Color &color) const {
        // Calculate luminance
        const double Y = 0.2126 * color.r + 0.7152 * color.g + 0.0722 * color.b;

        // Convert to integer (assuming integer output range)
        int gray = std::clamp(C_I(Y), 0, ALPHA_VALUEI);

        // Optionally, you can multiply by the alpha value to adjust for transparency
        gray = gray * color.a / ALPHA_VALUEI;

        return gray;
    }

    int grayscaleFromBGRA(const SDL_Color &color) const {
        // Calculate luminance
        const double Y = 0.2126 * color.b + 0.7152 * color.g + 0.0722 * color.r;

        // Convert to integer (assuming integer output range)
        int gray = std::clamp(C_I(Y), 0, ALPHA_VALUEI);

        // Optionally, you can multiply by the alpha value to adjust for transparency
        gray = gray * color.a / ALPHA_VALUEI;

        return gray;
    }

    // Format the SDL_Color according to the specified presentation.
    auto format(const SDL_Color &color, format_context &ctx) const -> format_context::iterator {
        if constexpr(SDL_BYTEORDER == SDL_BIG_ENDIAN) {
            return format_color_big_endian(color, ctx);
        } else {
            return format_color_little_endian(color, ctx);
        }
    }

    auto format_color_big_endian(const SDL_Color &color, format_context &ctx) const -> format_context::iterator {
        switch(presentation) {
        case 'x':
            return fmt::formatter<std::string_view>::format(FORMAT("#{:02X}{:02X}{:02X}{:02X}", color.r, color.g, color.b, color.a), ctx);
        case 'X':
            return fmt::formatter<std::string_view>::format(FORMAT("#{:#02x}{:02x}{:02x}{:02x}", color.r, color.g, color.b, color.a), ctx);
        case 'R':
            return fmt::formatter<std::string_view>::format(FORMAT("{:02X}{:02X}{:02X}", color.r, color.g, color.b), ctx);
        case 'a':
            return fmt::formatter<std::string_view>::format(FORMAT("{:02X}{:02X}{:02X}{:02X}", color.a, color.r, color.g, color.b), ctx);
        case 'b':
            return fmt::formatter<std::string_view>::format(FORMAT("{:02X}{:02X}{:02X}{:02X}", color.b, color.g, color.r, color.a), ctx);
        case 'g':  // Calculate grayscale value (average of RGB components)
            {
                int grayscale = grayscaleFromRGBA(color);
                return fmt::formatter<std::string_view>::format(FORMAT("#{:X}", grayscale), ctx);
            }
        default:
            return fmt::formatter<std::string_view>::format(FORMAT("(r:{}, g:{}, b:{}, a:{})", color.r, color.g, color.b, color.a), ctx);
        }
    }
    auto format_color_little_endian(const SDL_Color &color, format_context &ctx) const -> format_context::iterator {
        switch(presentation) {
        case 'x':
            return fmt::formatter<std::string_view>::format(FORMAT("#{:02X}{:02X}{:02X}{:02X}", color.b, color.g, color.r, color.a), ctx);
        case 'X':
            return fmt::formatter<std::string_view>::format(FORMAT("#{:#02x}{:02x}{:02x}{:02x}", color.b, color.g, color.r, color.a), ctx);
        case 'R':
            return fmt::formatter<std::string_view>::format(FORMAT("{:02X}{:02X}{:02X}", color.b, color.g, color.r), ctx);
        case 'a':
            return fmt::formatter<std::string_view>::format(FORMAT("{:02X}{:02X}{:02X}{:02X}", color.a, color.b, color.g, color.r), ctx);
        case 'b':
            return fmt::formatter<std::string_view>::format(FORMAT("{:02X}{:02X}{:02X}{:02X}", color.r, color.g, color.b, color.a), ctx);
        case 'g':  // Calculate grayscale value (average of RGB components)
            {
                int grayscale = grayscaleFromBGRA(color);
                return fmt::formatter<std::string_view>::format(FORMAT("#{:X}", grayscale), ctx);
            }
        default:
            return fmt::formatter<std::string_view>::format(FORMAT("(r:{}, g:{}, b:{}, a:{})", color.b, color.g, color.r, color.a), ctx);
        }
    }
};

// NOLINTEND(*-include-cleaner)