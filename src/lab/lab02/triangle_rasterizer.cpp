#include "lab/lab02/triangle_rasterizer.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace lab;

void TriangleRasterizer::Rasterize(
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3,
    Image *image,
    DepthImage *depthImage)
{
    float xMin = min(v1.position.x, min(v2.position.x, v3.position.x));
    float yMin = min(v1.position.y, min(v2.position.y, v3.position.y));
    float xMax = max(v1.position.x, max(v2.position.x, v3.position.x));
    float yMax = max(v1.position.y, max(v2.position.y, v3.position.y));

    for (int row = yMin; row < yMax; row++) {
        for (int column = xMin; column < xMax; column++) {

            if (row < 0 || row >= image->GetHeight() ||
                column < 0 || column >= image->GetWidth()) {
                continue;
            }

            glm::vec2 p(column + 0.5f, row + 0.5f);

            if (CheckPointInsideTriangle(p, v1, v2, v3)) {

                float depth = ComputePixelDepth(p, v1, v2, v3);
                float pixelDepth = depthImage->Get(row, column).x;

                if (pixelDepth > depth) {
                    image->Set(row, column, ComputePixelColor (p, v1, v2, v3));
                    depthImage->Set(row, column, glm::vec3(depth, 0, 0));
                }
            }
        }
    }
}

float TriangleRasterizer::ComputeTriangleArea(
    const glm::vec2 &v1,
    const glm::vec2 &v2,
    const glm::vec2 &v3)
{
    float a = glm::length(v1 - v2);
    float b = glm::length(v1 - v3);
    float c = glm::length(v2 - v3);

    float s = 0.5f * (a + b + c);


    return sqrt(s * (s - a) * (s - b) * (s - c));
}

bool TriangleRasterizer::CheckPointInsideTriangle(
    const glm::vec2 &p,
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3)
{
    const float EPSILON = 5.0f;

    float area = ComputeTriangleArea(v1.position, v2.position, v3.position);

    float areaU = ComputeTriangleArea(p, v1.position, v2.position);
    float areaV = ComputeTriangleArea(p, v1.position, v3.position);
    float areaW = ComputeTriangleArea(p, v2.position, v3.position);

    return abs(area - (areaU + areaV + areaW)) < EPSILON;

}

glm::vec3 TriangleRasterizer::ComputePixelColor(
    const glm::vec2 &p,
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3)
{

    float area = ComputeTriangleArea(v1.position, v2.position, v3.position);

    float u = ComputeTriangleArea(p, v1.position, v2.position) / area;
    float v = ComputeTriangleArea(p, v1.position, v3.position) / area;
    float w = ComputeTriangleArea(p, v2.position, v3.position) / area;

    return u * v3.color + v * v2.color + w * v1.color;
}

float TriangleRasterizer::ComputePixelDepth(
    const glm::vec2 &p,
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3)
{
    float area = ComputeTriangleArea(v1.position, v2.position, v3.position);

    float u = ComputeTriangleArea(p, v1.position, v2.position) / area;
    float v = ComputeTriangleArea(p, v1.position, v3.position) / area;
    float w = ComputeTriangleArea(p, v2.position, v3.position) / area;

    return u * v3.position.z + v * v2.position.z + w * v1.position.z;
}
