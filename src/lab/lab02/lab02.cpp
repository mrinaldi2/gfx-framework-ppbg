#include "lab/lab02/lab02.h"

#include <vector>

using namespace std;
using namespace lab;

Lab02::Lab02()
{
    window->SetSize(1280, 720);
}


Lab02::~Lab02()
{
}

void Lab02::Initialize() {
    Exercise2();
}

void Lab02::Exercise1()
{
    image->Init(1280, 720, 3 /* channels */);
    depthImage->Init(1280, 720);

    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(290, 90,  0.5), glm::vec3(1, 0, 0)),
            VertexFormat(glm::vec3(1099, 450,  0.5), glm::vec3(0, 1, 0)),
            VertexFormat(glm::vec3(650, 719,  0.5), glm::vec3(0, 0, 1)),

            VertexFormat(glm::vec3(200, 450,  0), glm::vec3(0, 1, 1)),
            VertexFormat(glm::vec3(830, 719,  1), glm::vec3(1, 1, 0)),
            VertexFormat(glm::vec3(1099, 0,  1), glm::vec3(1, 0, 1)) };

        vector<unsigned int> indices
        {
            0, 1, 2,    // indices for first triangle
            3, 4, 5,    // indices for second triangle
        };

        Rasterize(vertices, indices);
    }
}

void Lab02::Exercise2() {
    
    image->Init(1280, 720, 3 /* channels */);
    depthImage->Init(1280, 720);

    {
        vector<VertexFormat> vertices;
        vector<unsigned int> indices;
        
        glm::vec2 center = glm::vec2(1280 / 2, 720 / 2);

        float r = 200;

        float numTriangles = 20;

        float degrees = 360 / numTriangles;

        vertices.push_back(VertexFormat(glm::vec3(center.x, center.y, 0.0f), glm::vec3(0, 0, 0)));
        
        for (int i = 0; i < numTriangles; i++) {
            float x = center.x + r * cos(glm::radians(degrees * i));
            float y = center.y + r * sin(glm::radians(degrees * i));

            glm::vec3 color;
            switch (i % 3) {
            case 0:
                color = glm::vec3(1, 0, 0);
                break;
            case 1:
                color = glm::vec3(0, 1, 0);
                break;
            case 2:
                color = glm::vec3(0, 0, 1);
                break;
            }

            vertices.push_back(VertexFormat(glm::vec3(x, y, 0.0f), color));
        }

        for (int i = 0; i < numTriangles - 1; i++) {
            
            indices.push_back(0);
            indices.push_back(i + 1);
            indices.push_back(i + 2);
        
        }

        indices.push_back(0);
        indices.push_back(numTriangles);
        indices.push_back(1);
        
        Rasterize(vertices, indices);
    }

}

void Lab02::Rasterize(
    const vector<VertexFormat> &vertices,
    const vector<unsigned int> &indices)
{
    for (int i = 0; i < indices.size(); i += 3) {
        auto v1 = vertices[indices[i]];
        auto v2 = vertices[indices[i+1]];
        auto v3 = vertices[indices[i+2]];

        TriangleRasterizer::Rasterize(
            v1, v2, v3, image, depthImage
        );
    }
}
