#include "lab/lab1/triangle_rasterizer.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace lab;

void TriangleRasterizer::Rasterize(
    const VertexFormat &v1,
    const VertexFormat &v2,
    const VertexFormat &v3,
    Image *image,
    Image *depthImage)
{
    float xMin = min(v1.position.x, min(v2.position.x, v3.position.x));
    float yMin = min(v1.position.y, min(v2.position.y, v3.position.y));
    float xMax = max(v1.position.x, max(v2.position.x, v3.position.x));
    float yMax = max(v1.position.y, max(v2.position.y, v3.position.y));

    for (int i = yMin; i < yMax;i++) {
        for (int j = xMin; j < xMax;j++) {

            glm::vec2 p(j + 0.5f, i + 0.5f);

            if (CheckPointInsideTriangle(p, v1, v2, v3)) {

                float depth = ComputePixelDepth(p, v1, v2, v3);
                float pixelDepth = depthImage->Get(i, j).x;

                if (pixelDepth < depth) {
                    image->Set(i, j, ComputePixelColor (p, v1, v2, v3));
                    depthImage->Set(i, j, glm::vec3(depth, 0, 0));
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
    return 0;
}

bool TriangleRasterizer::CheckPointInsideTriangle(
    const glm::vec2 &p,
    const VertexFormat& v1,
    const VertexFormat& v2,
    const VertexFormat& v3)
{
    const float EPSILON = 5.0f;

    return false;
}

float TriangleRasterizer::ComputePixelDepth(
    const glm::vec2 &p,
    const VertexFormat& v1,
    const VertexFormat& v2,
    const VertexFormat& v3)
{
    return v1.position.x;
}

glm::vec3 TriangleRasterizer::ComputePixelColor(
    const glm::vec2& p,
    const VertexFormat& v1,
    const VertexFormat& v2,
    const VertexFormat& v3)
{
    return v1.color;
}
