/*
 * Created by gbian on 25/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "../headers.hpp"
#include "Ray.hpp"

namespace sdlrt {

    static inline constexpr bool FWDTFORM = true;
    static inline constexpr bool BCKTFORM = false;

    class GTform {
    public:
        GTform() noexcept;
        ~GTform() = default;
        GTform(const glm::dmat4 &fwd, const glm::dmat4 &bck) noexcept;

        GTform(const GTform &other) noexcept : m_fwdtfm{other.m_fwdtfm}, m_bcktfm{other.m_bcktfm} {}
        GTform(GTform &&other) noexcept : m_fwdtfm{std::move(other.m_fwdtfm)}, m_bcktfm{std::move(other.m_bcktfm)} {}
        GTform &operator=(GTform &&other) noexcept = default;

        void SetTransform(const glm::dvec3 &translation, const glm::dvec3 &rotation, const glm::dvec3 &scale);
        Ray Apply(const Ray &inputRay, bool dirFlag);
        glm::dvec3 Apply(const glm::dvec3 &inputVector, bool dirFlag);
        // Functions to return the transform matrices.
        glm::dmat4 GetForward() const noexcept;
        glm::dmat4 GetBackward() const noexcept;

        // Overload operators.
        friend GTform operator*(const GTform &lhs, const GTform &rhs);

        // Overload the assignment operator.
        GTform &operator=(const GTform &rhs) noexcept;

    private:
        glm::dmat4 m_fwdtfm{};
        glm::dmat4 m_bcktfm{};
    };
}  // namespace sdlrt

// NOLINTEND(*-include-cleaner)
