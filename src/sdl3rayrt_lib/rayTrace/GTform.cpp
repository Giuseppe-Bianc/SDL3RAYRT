/*
 * Created by gbian on 25/02/2025.
 * Copyright (c) 2025 All rights reserved.
 */
// NOLINTBEGIN(*-include-cleaner, *-easily-swappable-parameters)
#include "sdl3rayrt_lib/rayTrace/GTform.hpp"

namespace sdlrt {
    GTform::GTform() noexcept {
        m_fwdtfm = glm::dmat4(1.0);
        m_bcktfm = glm::dmat4(1.0);
    }
    GTform::GTform(const glm::dmat4 &fwd, const glm::dmat4 &bck) noexcept : m_fwdtfm(fwd), m_bcktfm(bck) {}

    void GTform::SetTransform(const glm::dvec3 &translation, const glm::dvec3 &rotation, const glm::dvec3 &scale) {
        const glm::dmat4 translationMatrix = glm::translate(glm::dmat4(1.0f), translation);
        const glm::dmat4 rotationMatrixX = glm::rotate(glm::dmat4(1.0f), rotation.x, glm::dvec3(1.0f, 0.0f, 0.0f));
        const glm::dmat4 rotationMatrixY = glm::rotate(glm::dmat4(1.0f), rotation.y, glm::dvec3(0.0f, 1.0f, 0.0f));
        const glm::dmat4 rotationMatrixZ = glm::rotate(glm::dmat4(1.0f), rotation.z, glm::dvec3(0.0f, 0.0f, 1.0f));
        const glm::dmat4 scaleMatrix = glm::scale(glm::dmat4(1.0f), scale);

        m_fwdtfm = translationMatrix * scaleMatrix * rotationMatrixX * rotationMatrixY * rotationMatrixZ;
        m_bcktfm = glm::inverse(m_fwdtfm);
    }
    Ray GTform::Apply(const Ray &inputRay, bool dirFlag) {
        Ray outputRay;

        if(dirFlag) {
            // Apply the forward transform.
            outputRay.setPoint1(this->Apply(inputRay.getPoint1(), FWDTFORM));
            outputRay.setPoint2(this->Apply(inputRay.getPoint2(), FWDTFORM));
            outputRay.setLab(outputRay.getPoint2() - outputRay.getPoint1());
        } else {
            // Apply the backward transform.
            outputRay.setPoint1(this->Apply(inputRay.getPoint1(), BCKTFORM));
            outputRay.setPoint2(this->Apply(inputRay.getPoint2(), BCKTFORM));
            outputRay.setLab(outputRay.getPoint2() - outputRay.getPoint1());
        }

        return outputRay;
    }
    glm::dvec3 GTform::Apply(const glm::dvec3 &inputVector, bool dirFlag) {
        const glm::dvec4 tempVector(inputVector, 1.0);
        const glm::dvec4 resultVector = dirFlag ? m_fwdtfm * tempVector : m_bcktfm * tempVector;
        return glm::dvec3(resultVector);
    }
    glm::dmat4 GTform::GetForward() const noexcept { return m_fwdtfm; }
    glm::dmat4 GTform::GetBackward() const noexcept { return m_bcktfm; }
    GTform &GTform::operator=(const GTform &rhs) noexcept {
        if(this != &rhs) {
            m_fwdtfm = rhs.m_fwdtfm;
            m_bcktfm = rhs.m_bcktfm;
        }

        return *this;
    }
    GTform operator*(const GTform &lhs, const GTform &rhs) {
        // Compute the forward transformation as matrix multiplication
        const glm::dmat4 fwdResult = lhs.m_fwdtfm * rhs.m_fwdtfm;

        // Compute the backward transformation as the inverse of the forward transform
        const glm::dmat4 bckResult = glm::inverse(fwdResult);

        // Return the final result
        return GTform(fwdResult, bckResult);
    }
}  // namespace sdlrt

// NOLINTEND(*-include-cleaner, *-easily-swappable-parameters)
