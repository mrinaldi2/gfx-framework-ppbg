#pragma once

#include "utils/glm_utils.h"


namespace transform3D
{
    // Translation matrix
    inline glm::mat4 Translate(float translateX, float translateY, float translateZ)
    {
        return glm::transpose(glm::mat4(
            1, 0, 0, translateX,
            0, 1, 0, translateY,
            0, 0, 1, translateZ,
            0, 0, 0, 1
        ));
      
    }

    // Scaling matrix
    inline glm::mat4 Scale(float scaleX, float scaleY, float scaleZ)
    {
        return glm::transpose(glm::mat4(
            scaleX, 0, 0, 0,
            0, scaleY, 0, 0,
            0, 0, scaleZ, 0,
            0, 0, 0, 1
        ));
    }

    // Rotation matrix relative to the OZ axis
    inline glm::mat4 RotateOZ(float radians)
    {
        return glm::transpose(glm::mat4(
            cos(radians), -sin(radians), 0, 0,
            sin(radians), cos(radians), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1
        ));
    }

    // Rotation matrix relative to the OY axis
    inline glm::mat4 RotateOY(float radians)
    {
        return glm::transpose(glm::mat4(
            cos(radians), 0, sin(radians), 0,
            0, 1, 0, 0,
            -sin(radians), 0, cos(radians), 0,
            0, 0, 0, 1
        ));
    }

    // Rotate matrix relative to the OX axis
    inline glm::mat4 RotateOX(float radians)
    {
        return glm::transpose(glm::mat4(
            1, 0, 0, 0,
            0, cos(radians), -sin(radians), 0,
            0, sin(radians), cos(radians), 0,
            0, 0, 0, 1
        ));
    }

    // Perspective projection matrix
    inline glm::mat4 Perspective(
        float fovy, float aspect, float zNear, float zFar)
    {
        float x1 = 1 / (tan(fovy / 2) * aspect);
        float y1 = 1 / tan(fovy / 2);
        float c1 = ( - 1 * (zFar + zNear)) / (zFar - zNear);
        float c2 = ( - 2 * zFar * zNear) / (zFar - zNear);
        return glm::transpose(glm::mat4(
            x1, 0, 0, 0,
            0, y1, 0, 0,
            0, 0, c1, c2,
            0, 0, -1, 1
        ));
    }

    // View matrix
    inline glm::mat4 View(
        const glm::vec3& position,
        const glm::vec3& forward,
        const glm::vec3& right,
        const glm::vec3& up)
    {
        return glm::transpose(glm::mat4(
            right.x, right.y, right.z, 0,
            up.x, up.y, up.z, 0,
            forward.x, forward.y, forward.z, 0,
            0, 0, 0, 1
        )) * glm::transpose(glm::mat4(
            1, 0, 0, -position.x,
            0, 1, 0, -position.y,
            0, 0, 1, -position.z,
            0, 0, 0, 1
        ));
    }

}   // namespace transform3D
