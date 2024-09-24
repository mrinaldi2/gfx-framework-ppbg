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
    // TODO(student): Calculate and return the area of
    // the triangle determined by vertices v1, v2 and v3
    //
    // Use Heron's formula

    return 0;
}

bool TriangleRasterizer::CheckPointInsideTriangle(
    const glm::vec2 &p,
    const VertexFormat& v1,
    const VertexFormat& v2,
    const VertexFormat& v3)
{
    const float EPSILON = 5.0f;

    // TODO(student): Check if the point p, received as a
    // parameter, is found in the triangle determined by
    // the vertices v1, v2 and v3
    //
    // Use the areas for this process, along with the EPSILON
    // value to compare the areas to account for possible
    // precision errors that may occur

    return false;
}

glm::vec3 TriangleRasterizer::ComputePixelColor(
    const glm::vec2& p,
    const VertexFormat& v1,
    const VertexFormat& v2,
    const VertexFormat& v3)
{
    // TODO(student): Calculate and return the color at point p,
    // received as a parameter, by interpolating between the
    // colors of vertices v1, v2 and v3
    //
    // Use the barycentric coordinates of point p

    return v3.color;
}

float TriangleRasterizer::ComputePixelDepth(
    const glm::vec2& p,
    const VertexFormat& v1,
    const VertexFormat& v2,
    const VertexFormat& v3)
{
    // TODO(student): Calculate and return the depth value of
    // point p, received as a parameter, by interpolation between
    // the depth values (position.z) of vertices v1, v2 and v3
    //
    // Use the barycentric coordinates of point p

    return v3.position.z;
}
