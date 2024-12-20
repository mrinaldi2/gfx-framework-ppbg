#pragma once

#include "components/simple_scene.h"
#include "components/transform.h"
#include <array>

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

		void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
		void RenderTextureScreen(Shader* shader, unsigned int textureID);
		void LoadShader(const std::string& name);
		void GenerateHeightmapTexture();
		void CreateTerrain();
		void DrawFramebufferTextures();

		float PerlinNoise(float x, float z);
		float OctavePerlinNoise(float x, float z, int octaves, float persistence, float scale);
		float GenerateTerrainHeight(float x, float z);

		float mod289(float x) {
			return x - std::floor(x * (1.0f / 289.0f)) * 289.0f;
		}

		std::array<float, 4> mod289(const std::array<float, 4>& x) {
			return { mod289(x[0]), mod289(x[1]), mod289(x[2]), mod289(x[3]) };
		}

		std::array<float, 4> addArrays(const std::array<float, 4>& a, const std::array<float, 4>& b) {
			return { a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3] };
		}

		std::array<float, 4> permute(const std::array<float, 4>& x) {
			return mod289({
				(x[0] * 34.0f + 10.0f) * x[0],
				(x[1] * 34.0f + 10.0f) * x[1],
				(x[2] * 34.0f + 10.0f) * x[2],
				(x[3] * 34.0f + 10.0f) * x[3]
				});
		}

		std::array<float, 4> taylorInvSqrt(const std::array<float, 4>& r) {
			return {
				1.79284291400159f - 0.85373472095314f * r[0],
				1.79284291400159f - 0.85373472095314f * r[1],
				1.79284291400159f - 0.85373472095314f * r[2],
				1.79284291400159f - 0.85373472095314f * r[3]
			};
		}

		std::array<float, 2> fade(const std::array<float, 2>& t) {
			return {
				t[0] * t[0] * t[0] * (t[0] * (t[0] * 6.0f - 15.0f) + 10.0f),
				t[1] * t[1] * t[1] * (t[1] * (t[1] * 6.0f - 15.0f) + 10.0f)
			};
		}


	private:
		const int gridSize = 100;
		unsigned int terrainNoise;
		glm::vec3 helicopterPosition;
		glm::vec3 helicopterDirection;

		bool draw_coordinates = true;
		bool draw_terrain_texture = true;
	};
}