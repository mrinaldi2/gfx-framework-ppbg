#pragma once

#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab/tema/perlin.h"

namespace lab
{
	class Tema : public gfxc::SimpleScene
	{
	public:
		Tema();
		~Tema();

		void Init() override;

	private:

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

		void RenderTerrain(Shader* shader);
		void SetModelProjectView(Shader* shader, const glm::mat4& modelMatrix);
		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
		void RenderTextureScreen(Shader* shader, unsigned int textureID);
		void LoadShader(const std::string& name);
		Texture2D* LoadTexture(const char* imagePath);
		Texture2D* CreateTexture(unsigned int width, unsigned int height,
			unsigned int channels, unsigned char* data);
		void GenerateHeightmapTexture();
		void CreateTerrain();
		void DrawFramebufferTextures();
		glm::vec3 GenerateRandomVector();

		float GenerateTerrainHeight(float x, float z);


	private:
		const int gridSize = 100;
		unsigned int terrainNoise;
		glm::vec3 helicopterPosition;
		glm::vec3 targetHelicopterPosition;
		float helicopterCurrentTime = 3.0f;
		float helicopterTargetTime = 3.0f;
		glm::vec3 helicopterDirection;
		glm::vec3 currentPosition;
		std::unordered_map<std::string, Texture2D*> mapTextures;

		bool draw_coordinates = false;
		bool draw_terrain_texture = false;

		PerlinNoise* perlin;
	};
}