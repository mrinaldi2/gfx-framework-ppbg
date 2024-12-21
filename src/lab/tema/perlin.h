#pragma once

#include <array>
#include <cmath>

namespace lab
{
	class PerlinNoise
	{
	public:
		PerlinNoise();
		~PerlinNoise();
		float Noise(float x, float z);
		float OctaveNoise(float x, float z, int octaves, float persistence, float scale);
	private:
		float mod289(float x);
		std::array<float, 4> mod289(const std::array<float, 4>& x);
		std::array<float, 4> addArrays(const std::array<float, 4>& a, const std::array<float, 4>& b);
		std::array<float, 4> permute(const std::array<float, 4>& x);
		std::array<float, 4> taylorInvSqrt(const std::array<float, 4>& r);
		std::array<float, 2> fade(const std::array<float, 2>& t);
	};
}