#include "lab/tema/tema.h"

using namespace std;
using namespace lab;

Tema::Tema()
{

}

Tema::~Tema()
{

}

void Tema::Init() 
{
    CreateTerrain();
    GenerateHeightmapTexture();
    LoadShader("Terrain");
}

void Tema::LoadShader(const std::string& name)
{
    std::string shaderPath = PATH_JOIN(window->props.selfDir, "src/lab", "tema", "shaders");

    // Create a shader program
    {
        Shader* shader = new Shader(name);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".VS.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(shaderPath, name + ".FS.glsl"), GL_FRAGMENT_SHADER);

        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}

void Tema::CreateTerrain() 
{
    const float halfSize = gridSize / 2.0f; // Centered at 0.0
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<unsigned int> indices;

    // Generate vertices and normals
    for (int z = 0; z < gridSize; ++z) {
        for (int x = 0; x < gridSize; ++x) {
            float posX = x - halfSize;
            float posZ = z - halfSize;
            positions.emplace_back(posX, 0.0f, posZ);
            normals.emplace_back(0.0f, 1.0f, 0.0f);
        }
    }

    // Generate indices for triangles
    for (int z = 0; z < gridSize - 1; ++z) {
        for (int x = 0; x < gridSize - 1; ++x) {
            int topLeft = z * gridSize + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * gridSize + x;
            int bottomRight = bottomLeft + 1;

            // First triangle
            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            // Second triangle
            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    // Initialize the mesh
    Mesh* mesh = new Mesh("Terrain");
    mesh->InitFromData(positions, normals, indices);
    meshes[mesh->GetMeshID()] = mesh;
}

float Tema::PerlinNoise(float x, float z) {
    // Simple pseudo-random noise function
    return 0.5f * (sin(x * 0.1f) + cos(z * 0.1f)); // Adjust the scale for finer detail
}

void Tema::GenerateHeightmapTexture() {
    glGenTextures(1, &terrainNoise);
    glBindTexture(GL_TEXTURE_2D, terrainNoise);

    // Convert heights to a format suitable for the GPU
    std::vector<unsigned char> heightData(gridSize * gridSize);
    for (int i = 0; i < gridSize * gridSize; ++i) {
        float height = PerlinNoise(i % gridSize, i / gridSize);
        heightData[i] = static_cast<unsigned char>((height + 1.0f) * 127.5f); // Normalize to [0, 255]
    }

    // Upload to the GPU
    glTexImage2D(GL_TEXTURE_2D, 0, GL_R8, gridSize, gridSize, 0, GL_RED, GL_UNSIGNED_BYTE, heightData.data());

    // Texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    Texture2D* texture = new Texture2D();
    texture->Init(terrainNoise, gridSize, gridSize, 1);
    TextureManager::SetTexture("TerrainNoise", texture);

    glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
}

void Tema::FrameStart()
{
    ClearScreen();
}


void Tema::Update(float deltaTimeSeconds) {
    glm::mat4 modelMatrix = glm::mat4(1);
    RenderSimpleMesh(meshes["Terrain"], shaders["Terrain"], modelMatrix);
}

void Tema::FrameEnd()
{

}

void Tema::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set model matrix uniform
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Set view matrix uniform
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Set projection matrix uniform
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    if (strcmp(mesh->GetMeshID(), "Terrain") == 0) {

        int gridSize_loc = glGetUniformLocation(shader->program, "gridSize");
        int gridSizeHalf_loc = glGetUniformLocation(shader->program, "gridSizeHalf");
        glUniform1f(gridSize_loc, gridSize);
        glUniform1f(gridSizeHalf_loc, gridSize / 2);

        int loc_texture_noise = glGetUniformLocation(shader->program, "NoiseTexture");
        TextureManager::GetTexture("TerrainNoise")->BindToTextureUnit(GL_TEXTURE0);
        glUniform1i(loc_texture_noise, 0);
    }


    // Draw the object
    mesh->Render();
}

void Tema::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
}

void Tema::OnKeyPress(int key, int mods)
{
    // Add key press event
}

void Tema::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Tema::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
