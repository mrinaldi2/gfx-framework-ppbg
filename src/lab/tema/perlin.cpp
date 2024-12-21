#include "lab/tema/perlin.h"

using namespace lab;
using namespace std;

PerlinNoise::PerlinNoise() {}

PerlinNoise::~PerlinNoise() {}

float PerlinNoise::Noise(float x, float z) {
    std::array<float, 4> Pi = {
            std::floor(x),
            std::floor(z),
            std::floor(x) + 1.0f,
            std::floor(z) + 1.0f
    };

    std::array<float, 4> Pf = {
        x - std::floor(x),
        z - std::floor(z),
        x - (std::floor(x) + 1.0f),
        z - (std::floor(z) + 1.0f)
    };

    Pi = mod289(Pi);

    std::array<float, 4> ix = { Pi[0], Pi[2], Pi[0], Pi[2] };
    std::array<float, 4> iy = { Pi[1], Pi[1], Pi[3], Pi[3] };
    std::array<float, 4> fx = { Pf[0], Pf[2], Pf[0], Pf[2] };
    std::array<float, 4> fy = { Pf[1], Pf[1], Pf[3], Pf[3] };

    std::array<float, 4> i = permute(addArrays(permute(ix), iy));

    std::array<float, 4> gx = {
        std::fmod(i[0] * (1.0f / 41.0f), 1.0f) * 2.0f - 1.0f,
        std::fmod(i[1] * (1.0f / 41.0f), 1.0f) * 2.0f - 1.0f,
        std::fmod(i[2] * (1.0f / 41.0f), 1.0f) * 2.0f - 1.0f,
        std::fmod(i[3] * (1.0f / 41.0f), 1.0f) * 2.0f - 1.0f
    };

    std::array<float, 4> gy = {
        std::abs(gx[0]) - 0.5f,
        std::abs(gx[1]) - 0.5f,
        std::abs(gx[2]) - 0.5f,
        std::abs(gx[3]) - 0.5f
    };

    std::array<float, 4> norm = taylorInvSqrt({
        gx[0] * gx[0] + gy[0] * gy[0],
        gx[1] * gx[1] + gy[1] * gy[1],
        gx[2] * gx[2] + gy[2] * gy[2],
        gx[3] * gx[3] + gy[3] * gy[3]
        });

    gx = { gx[0] * norm[0], gx[1] * norm[1], gx[2] * norm[2], gx[3] * norm[3] };
    gy = { gy[0] * norm[0], gy[1] * norm[1], gy[2] * norm[2], gy[3] * norm[3] };

    std::array<float, 2> g00 = { gx[0], gy[0] };
    std::array<float, 2> g10 = { gx[1], gy[1] };
    std::array<float, 2> g01 = { gx[2], gy[2] };
    std::array<float, 2> g11 = { gx[3], gy[3] };

    float n00 = g00[0] * fx[0] + g00[1] * fy[0];
    float n10 = g10[0] * fx[1] + g10[1] * fy[1];
    float n01 = g01[0] * fx[2] + g01[1] * fy[2];
    float n11 = g11[0] * fx[3] + g11[1] * fy[3];

    std::array<float, 2> fade_xy = fade({ Pf[0], Pf[1] });
    std::array<float, 2> n_x = {
        n00 + fade_xy[0] * (n10 - n00),
        n01 + fade_xy[0] * (n11 - n01)
    };

    float n_xy = n_x[0] + fade_xy[1] * (n_x[1] - n_x[0]);
    float scaled_n_xy = std::exp(n_xy) - 1.1f; // Exponential scaling
    return 2.3f * scaled_n_xy;
}

float PerlinNoise::OctaveNoise(float x, float z, int octaves, float persistence, float scale) {
	float total = 0.0f;
	float maxAmplitude = 0.0f;
	float amplitude = 1.0f;
	float frequency = 1.0f;

	for (int i = 0; i < octaves; ++i) {
		total += Noise(x * frequency * scale, z * frequency * scale) * amplitude;
		maxAmplitude += amplitude;
		amplitude *= persistence;
		frequency *= 2.0f;
	}

	return total / maxAmplitude;
}

float PerlinNoise::mod289(float x) {
	return x - std::floor(x * (1.0f / 289.0f)) * 289.0f;
}

std::array<float, 4> PerlinNoise::mod289(const std::array<float, 4>& x) {
	return { mod289(x[0]), mod289(x[1]), mod289(x[2]), mod289(x[3]) };
}

std::array<float, 4> PerlinNoise::addArrays(const std::array<float, 4>& a, const std::array<float, 4>& b) {
	return { a[0] + b[0], a[1] + b[1], a[2] + b[2], a[3] + b[3] };
}

std::array<float, 4> PerlinNoise::permute(const std::array<float, 4>& x) {
	return mod289({
		(x[0] * 34.0f + 10.0f) * x[0],
		(x[1] * 34.0f + 10.0f) * x[1],
		(x[2] * 34.0f + 10.0f) * x[2],
		(x[3] * 34.0f + 10.0f) * x[3]
		});
}

std::array<float, 4> PerlinNoise::taylorInvSqrt(const std::array<float, 4>& r) {
	return {
		1.79284291400159f - 0.85373472095314f * r[0],
		1.79284291400159f - 0.85373472095314f * r[1],
		1.79284291400159f - 0.85373472095314f * r[2],
		1.79284291400159f - 0.85373472095314f * r[3]
	};
}

std::array<float, 2> PerlinNoise::fade(const std::array<float, 2>& t) {
	return {
		t[0] * t[0] * t[0] * (t[0] * (t[0] * 6.0f - 15.0f) + 10.0f),
		t[1] * t[1] * t[1] * (t[1] * (t[1] * 6.0f - 15.0f) + 10.0f)
	};
}