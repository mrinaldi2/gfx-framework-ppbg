#include "lab/lab1/lab1.h"

#include <vector>

using namespace std;
using namespace lab;

Lab1::Lab1()
{
    window->SetSize(1280, 720);
}


Lab1::~Lab1()
{
}

void Lab1::Initialize()
{
    image->Init(1280, 720, 3 /* channels */);
    depthImage->Init(1280, 720, 1 /* channels */);

    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(200, 450,  0.5), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(830, 719,  0.5), glm::vec3(1, 1, 0)),
            VertexFormat(glm::vec3(1099, 0,  0.5), glm::vec3(1, 0, 1)),

            VertexFormat(glm::vec3(290, 90,  0.0), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(1099, 450,  1), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(650, 719,  1), glm::vec3(0, 0, 1)),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,    // indices for first triangle
            3, 4, 5,    // indices for second triangle
        };

        Rasterize(vertices, indices);
    }
}

void Lab1::Rasterize(
    const vector<VertexFormat> &vertices,
    const vector<unsigned int> &indices
)
{
    for (int i = 0; i < indices.size(); i+=3) {
        auto v1 = vertices[indices[i]];
        auto v2 = vertices[indices[i+1]];
        auto v3 = vertices[indices[i+2]];

        TriangleRasterizer::Rasterize(
            v1, v2, v3, image, depthImage
        );
    }
}
