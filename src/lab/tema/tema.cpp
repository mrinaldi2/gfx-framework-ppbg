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

    helicopterPosition = glm::vec3(0, 5, 0);
    helicopterDirection = glm::vec3(0, 0, 1);

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    CreateTerrain();
    GenerateHeightmapTexture();
    LoadShader("Terrain");
    LoadShader("ViewColorTexture");
    LoadShader("Default");
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
    mesh->UseMaterials(false);
    meshes[mesh->GetMeshID()] = mesh;
}

float Tema::GenerateTerrainHeight(float x, float z) {
    printf("(x,z): (%f, %f)\n", x, z);
    float baseHeight = 0.0f; // Base elevation
    float height = OctavePerlinNoise(x, z, 3, 0.5f, 0.01f);
    return height; // Exaggerate peaks
}

float Tema::OctavePerlinNoise(float x, float z, int octaves, float persistence, float scale) {
    float total = 0.0f;
    float maxAmplitude = 0.0f;
    float amplitude = 1.0f;
    float frequency = 1.0f;

    for (int i = 0; i < octaves; ++i) {
        total += PerlinNoise(x * frequency * scale, z * frequency * scale) * amplitude;
        maxAmplitude += amplitude;
        amplitude *= persistence;
        frequency *= 2.0f;
    }

    return total / maxAmplitude;
}

float Tema::PerlinNoise(float x, float z) {
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

void Tema::GenerateHeightmapTexture() {
    glGenTextures(1, &terrainNoise);
    glBindTexture(GL_TEXTURE_2D, terrainNoise);

    // Convert heights to a format suitable for the GPU
    std::vector<unsigned char> heightData(gridSize * gridSize);
    for (int i = 0; i < gridSize * gridSize; ++i) {
        float x = (i % gridSize) - 50.0f; // Normalize x
        float z = (i / gridSize) - 50.0f; // Normalize z
        float height = GenerateTerrainHeight(x, z);
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

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, helicopterPosition);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.2, 0.2));
 
    RenderSimpleMesh(meshes["box"], shaders["Default"], modelMatrix);
    // Set view matrix uniform
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    
    // Set projection matrix uniform
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();

    if (draw_coordinates) {
        DrawCoordinateSystem(viewMatrix, projectionMatrix);
    }
    
    if (draw_terrain_texture) {
        DrawFramebufferTextures();
    }
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

        glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
        GLint eye_position = glGetUniformLocation(shader->program, "eye_position");
        glUniform3f(eye_position, helicopterPosition.x, helicopterPosition.y, helicopterPosition.z);

        int gridSize_loc = glGetUniformLocation(shader->program, "gridSize");
        int gridSizeHalf_loc = glGetUniformLocation(shader->program, "gridHalfSize");
        float gridSizeHalf = gridSize / 2;
        glUniform1f(gridSize_loc, gridSize);
        glUniform1f(gridSizeHalf_loc, gridSizeHalf);

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
    float speed = 2;

    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 forward = glm::normalize(glm::vec3(helicopterDirection.x, 0, helicopterDirection.z));
        glm::vec3 right = glm::cross(up, forward);

        // Control light position using on W, A, S, D, E, Q
        if (window->KeyHold(GLFW_KEY_W)) helicopterPosition += forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_A)) helicopterPosition += right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_S)) helicopterPosition -= forward * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_D)) helicopterPosition -= right * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_E)) helicopterPosition += up * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_Q)) helicopterPosition -= up * deltaTime * speed;
        if (window->KeyHold(GLFW_KEY_LEFT)) {
            helicopterDirection = glm::mat3(glm::rotate(glm::mat4(1), deltaTime, up)) * helicopterDirection;
        }
        if (window->KeyHold(GLFW_KEY_RIGHT)) {
            helicopterDirection = glm::mat3(glm::rotate(glm::mat4(1), -deltaTime, up)) * helicopterDirection;
        }
        if (window->KeyHold(GLFW_KEY_UP)) {
            helicopterDirection = glm::mat3(glm::rotate(glm::mat4(1), -deltaTime, right)) * helicopterDirection;
        }
        if (window->KeyHold(GLFW_KEY_DOWN)) {
            helicopterDirection = glm::mat3(glm::rotate(glm::mat4(1), deltaTime, right)) * helicopterDirection;
        }

        GetSceneCamera()->SetPosition(helicopterPosition + helicopterDirection * glm::vec3(0,0,-3));
    }
}

void Tema::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_F1) {
        draw_coordinates = !draw_coordinates;
    }

    if (key == GLFW_KEY_F2) {
        draw_terrain_texture = !draw_terrain_texture;
    }
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


void Tema::DrawFramebufferTextures()
{
    int screenPosX = window->GetResolution().x - 950;
    int screenPosY = 100;

    int width = 400;
    int height = 400;

    // Render the color texture on the screen
    glViewport(screenPosX, screenPosY, width, height);

    RenderTextureScreen(shaders["ViewColorTexture"], terrainNoise);
}

void Tema::RenderTextureScreen(Shader* shader, unsigned int textureID)
{
    if (!shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Set texture uniform
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);

    // Draw the object
    glBindVertexArray(meshes["quad"]->GetBuffers()->m_VAO);
    glDrawElements(meshes["quad"]->GetDrawMode(), static_cast<int>(meshes["quad"]->indices.size()), GL_UNSIGNED_INT, 0);
}