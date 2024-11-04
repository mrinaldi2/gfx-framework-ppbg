#pragma once

#include <vector>

#include "components/simple_scene.h"

#include "lab/lab03/transform2D.h"

namespace lab
{
    class Lab05 : public gfxc::SimpleScene
    {
     public:
        Lab05();
        ~Lab05();

        void Init() override;

     private:

        enum SCENE {
             DRAW_FOUR_VIEWS = 0,
             DRAW_SOLAR_SYSTEM = 1,
             COUNT
        };

        SCENE sceneToDraw = DRAW_SOLAR_SYSTEM;

        void CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);
        void DrawObjects(gfxc::Camera *camera, const transform2D::ViewportSpace &viewport_space);

        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void RenderMesh(Mesh * mesh, Shader * shader, const glm::mat4 & modelMatrix,
            const glm::mat4 &viewMatrix, const glm::mat4 &projectionMatrix);

        void TranslateYCubeAnimator(float deltaTime);
        void TranslateYCubeAnimatorState();

        void TranslateXCubeAnimator(float deltaTime);
        void TranslateXCubeAnimatorState();

        void ScaleCubeAnimator(float deltaTime);
        void ScaleCubeAnimatorState();

        void RotateCubeAnimator(float deltaTime);

        void Draw4Views(float deltaTimeSeconds);
        void DrawSolarSystem(float deltaTimeSeconds);
        void DrawSolarObjects(gfxc::Camera* camera, const transform2D::ViewportSpace& viewport_space);
     protected:
        transform2D::ViewportSpace viewport_space;
        glm::vec3 color;
        GLenum cullFace;

        float timePassed = 0.0f;

        float TranslateCubeY = 0.5f;
        bool TranslateYCubeUpState = true;

        float TranslateCubeX = 0.0f;
        bool TranslateXCubeUpState = true;

        float ScaleCube = 0.5f;
        bool ScaleCubeGrowState = true;

        float RotateCubeOY = 0.0f;

        glm::vec3 transformCube = glm::vec3(0.0f);

        glm::vec3 earth = glm::vec3(0.0f);

        glm::vec3 moon = glm::vec3(0.0f);
    };
}   // namespace lab
