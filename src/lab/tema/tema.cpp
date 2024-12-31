#include "lab/tema/tema.h"
#include <stb/stb_image.h>
#include <glm/gtx/compatibility.hpp> 
#include <random>


using namespace std;
using namespace lab;

Tema::Tema()
{

}

Tema::~Tema()
{
    delete perlin;
    delete helicopterVector;
}

void Tema::Init() 
{
	helicopterVector = new Vector();
	perlin = new PerlinNoise();
    helicopterPosition = glm::vec3(0, 1, 0);
	targetHelicopterPosition = glm::vec3(0, 1, 0);
    helicopterDirection = glm::vec3(0, 0, 0);

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\textures\\snow.jpg");
        mapTextures["snow"] = texture;
    }

    {
        Texture2D* texture = LoadTexture("src\\lab\\tema\\textures\\ground.jpg");
        mapTextures["ground"] = texture;
    }

    {
        Mesh* mesh = new Mesh("quad");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "quad.obj");
        mesh->UseMaterials(false);
        meshes[mesh->GetMeshID()] = mesh;
    }

    CreateHelicopter();
    CreateTerrain();
    GenerateHeightmapTexture();
    LoadShader("Terrain");
    LoadShader("ViewColorTexture");
    LoadShader("Default");
    LoadShader("Vector");
}

Texture2D* Tema::LoadTexture(const char* imagePath)
{
    int width, height, channels;
    unsigned char* imageData = stbi_load(imagePath, &width, &height, &channels, 0);

    return CreateTexture(width, height, channels, imageData);
}

Texture2D* Tema::CreateTexture(unsigned int width, unsigned int height,
    unsigned int channels, unsigned char* data)
{
    GLuint textureID = 0;
    unsigned int size = width * height * channels;

    // TODO(student): Generate and bind the new texture ID
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

    if (GLEW_EXT_texture_filter_anisotropic) {
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);
    }
    // TODO(student): Set the texture parameters (MIN_FILTER and MAG_FILTER) using glTexParameteri
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    CheckOpenGLError();


    // TODO(student): Use the "glTexImage2D" directive to load the information
    // into the graphics processor's memory. Use the correct format based on
    // the number of components:
    //   - 1 color channel - GL_RED
    //   - 2 color channels - GL_RG
    //   - 3 color channels - GL_RGB
    //   - 4 color channels - GL_RGBA
    GLuint format = 0;
    switch (channels)
    {
    case 2:
        format = GL_RG;
        break;
    case 3:
        format = GL_RGB;
        break;
    case 4:
        format = GL_RGBA;
        break;
    default:
        format = GL_RED;
        break;
    }
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);

    // TODO(student): Generate texture mip-maps
    glGenerateMipmap(GL_TEXTURE_2D);

    CheckOpenGLError();

    // Save the texture into a wrapper Texture2D class for using easier later during rendering phase
    Texture2D* texture = new Texture2D();
    texture->Init(textureID, width, height, channels);

    SAFE_FREE_ARRAY(data);
    return texture;
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

void Tema::CreateHelicopter()
{
    // Body dimensions
    float bodyX = 1.2f, bodyY = 0.2f, bodyZ = 0.2f;

    // Tail dimensions
    float tailX = 1.0f, tailY = 0.15f, tailZ = 0.15f;

    // Body vertices
    std::vector<glm::vec3> positions = {
        // Body (larger rectangle)
        {0.0f,  bodyY,  bodyZ},  // 0
        {bodyX, bodyY,  bodyZ},  // 1
        {bodyX, -bodyY, bodyZ},  // 2
        {0.0f,  -bodyY, bodyZ},  // 3
        {0.0f,  bodyY,  -bodyZ},   // 4
        {bodyX, bodyY,  -bodyZ},   // 5
        {bodyX, -bodyY, -bodyZ},   // 6
        {0.0f,  -bodyY, -bodyZ},   // 7

        // Tail (smaller rectangle)
        {0.0f,  tailY, tailZ},  // 8
        {-tailX, tailY, tailZ},  // 9
        {-tailX, -tailY, tailZ}, // 10
        {0.0f,  -tailY, tailZ}, // 11
        {0.0f,  tailY,  -tailZ},   // 12
        {-tailX, tailY,  -tailZ},   // 13
        {-tailX, -tailY, -tailZ},   // 14
        {0.0f,  -tailY, -tailZ}    // 15
    };

    // Normals (one normal per face, repeated for each vertex)
    std::vector<glm::vec3> normals = {
        // Body normals
        {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, // Front face
        {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, // Back face
        {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, // Top face
        {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, // Bottom face
        {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, // Right face
        {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, // Left face

        // Tail normals
        {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, // Front face
        {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, // Back face
        {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, // Top face
        {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, // Bottom face
        {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, // Right face
        {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}  // Left face
    };

    // Indices for the body and tail rectangles
    std::vector<unsigned int> indices = {
        // Body rectangle (two triangles per face)
        0, 1, 2, 0, 2, 3,  // Front face
        4, 5, 6, 4, 6, 7,  // Back face
        0, 1, 5, 0, 5, 4,  // Top face
        3, 2, 6, 3, 6, 7,  // Bottom face
        1, 2, 6, 1, 6, 5,  // Right face
        0, 3, 7, 0, 7, 4,  // Left face

        // Tail rectangle
        8, 9, 10, 8, 10, 11, // Front face
        12, 13, 14, 12, 14, 15, // Back face
        8, 9, 13, 8, 13, 12, // Top face
        11, 10, 14, 11, 14, 15, // Bottom face
        9, 10, 14, 9, 14, 13, // Right face
        8, 11, 15, 8, 15, 12  // Left face
    };

    // Initialize the mesh
    Mesh* mesh = new Mesh("Helicopter");
    mesh->InitFromData(positions, normals, indices);
    mesh->UseMaterials(false);
    meshes[mesh->GetMeshID()] = mesh;

    float propellerX = 1.0f, propellerY = 0.02f, propellerZ = 0.1f;

    std::vector<glm::vec3> propellerPositions = {
        // Body (larger rectangle)
        {-propellerX,  propellerY,  propellerZ},  // 0
        {propellerX, propellerY,  propellerZ},  // 1
        {propellerX, -propellerY, propellerZ},  // 2
        {-propellerX,  -propellerY, propellerZ},  // 3
        {-propellerX,  propellerY,  -propellerZ},  // 4
        {propellerX, propellerY,  -propellerZ},  // 5
        {propellerX, -propellerY, -propellerZ},  // 6
        {-propellerX,  -propellerY, -propellerZ},  // 7
    
    };

    // Normals (one normal per face, repeated for each vertex)
    std::vector<glm::vec3> propellerNormals = {
        // Body normals
        {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, // Front face
        {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, // Back face
        {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, // Top face
        {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, // Bottom face
        {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, // Right face
        {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, // Left face

    };

    // Indices for the body and tail rectangles
    std::vector<unsigned int> propellerIndices = {
       
        0, 1, 2, 0, 2, 3,  // Front face
        4, 5, 6, 4, 6, 7,  // Back face
        0, 1, 5, 0, 5, 4,  // Top face
        3, 2, 6, 3, 6, 7,  // Bottom face
        1, 2, 6, 1, 6, 5,  // Right face
        0, 3, 7, 0, 7, 4,  // Left face
    };

    // Initialize the mesh
    Mesh* propeller = new Mesh("Propeller");
    propeller->InitFromData(propellerPositions, propellerNormals, propellerIndices);
    propeller->UseMaterials(false);
    meshes[propeller->GetMeshID()] = propeller;
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
    float baseHeight = 0.0f; // Base elevation
    float height = perlin->OctaveNoise(x, z, 3, 0.5f, 0.01f);
    return height; // Exaggerate peaks
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
    
    RenderHelicopter(deltaTimeSeconds);

    RenderTerrain(shaders["Terrain"]);
    
    if (draw_terrain_texture) {
        DrawFramebufferTextures();
    }

	glUseProgram(shaders["Vector"]->program);
    SetModelProjectView(shaders["Vector"], glm::mat4(1));
	helicopterVector->Draw();

    if(!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT)) {

        auto camera = GetSceneCamera();
        camera->SetPositionAndRotation(
            currentPosition + glm::vec3(0, 1, 2),
            glm::quatLookAt(-glm::normalize(glm::vec3(0, 1, 2)), glm::vec3(0, 1, 0))
        );
		camera->Update();
    }

    if (draw_coordinates) {
        glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
        glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
        DrawCoordinateSystem(viewMatrix, projectionMatrix);
    }
}

double Tema::MapAtan2To360(double y, double x) {
    double angle = atan2(y, x);
    if (angle < 0) {
        angle += 2 * M_PI; // M_PI is the constant for π
    }
    return M_PI * 2 - angle; // Angle in radians
}

void Tema::FrameEnd()
{

}

void Tema::SetModelProjectView(Shader* shader, const glm::mat4& modelMatrix)
{
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

}

void Tema::RenderTerrain(Shader* shader)
{
    if (!shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    SetModelProjectView(shader, glm::mat4(1));

    GLint eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, currentPosition.x, currentPosition.y, currentPosition.z);

    int gridSize_loc = glGetUniformLocation(shader->program, "gridSize");
    int gridSizeHalf_loc = glGetUniformLocation(shader->program, "gridHalfSize");
    float gridSizeHalf = gridSize / 2;
    glUniform1f(gridSize_loc, gridSize);
    glUniform1f(gridSizeHalf_loc, gridSizeHalf);

    int loc_texture_noise = glGetUniformLocation(shader->program, "NoiseTexture");
    TextureManager::GetTexture("TerrainNoise")->BindToTextureUnit(GL_TEXTURE0);
    glUniform1i(loc_texture_noise, 0);

	int loc_texture_snow = glGetUniformLocation(shader->program, "SnowTexture");
	mapTextures["snow"]->BindToTextureUnit(GL_TEXTURE1);
	glUniform1i(loc_texture_snow, 1);

    int loc_texture_ground = glGetUniformLocation(shader->program, "GroundTexture");
    mapTextures["ground"]->BindToTextureUnit(GL_TEXTURE2);
    glUniform1i(loc_texture_ground, 2);

	// Draw the object

	meshes["Terrain"]->Render();

}

void Tema::RenderHelicopter(float deltaTimeSeconds) {
    if (helicopterCurrentTime <= helicopterTargetTime) {
        helicopterCurrentTime += deltaTimeSeconds;
        float lerpedTime = helicopterCurrentTime / helicopterTargetTime;
        //currentPosition = helicopterPosition;
        currentPosition = glm::lerp(helicopterPosition, targetHelicopterPosition, lerpedTime);
    }
    else {
        helicopterPosition = targetHelicopterPosition;
        currentPosition = helicopterPosition;
    }

    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, currentPosition);
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.2, 0.2, 0.2));
    float angle = MapAtan2To360(helicopterDirection.z, helicopterDirection.x);
    glm::mat4 helicopterMatrix = glm::rotate(modelMatrix, angle, glm::vec3(0, 1, 0));

    auto shader = shaders["Default"];
    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);
	int fill_color_loc = glGetUniformLocation(shader->program, "fill_color");
	glUniform3f(fill_color_loc, 0.0f, 1.0f, 0.0f);
    SetModelProjectView(shader, helicopterMatrix);

    // Draw the object
    meshes["Helicopter"]->Render();

  
    glUniform3f(fill_color_loc, 1.0f, 1.0f, 0.0f);
	glm::mat4 propellerMatrix = glm::translate(helicopterMatrix, glm::vec3(0.65f, 0.22f, 0.0f));
	propellerAngle += propellerSpeed * deltaTimeSeconds;
    propellerMatrix = glm::rotate(propellerMatrix, -propellerAngle, glm::vec3(0, 1, 0));
    SetModelProjectView(shader, propellerMatrix);
    meshes["Propeller"]->Render();

    glm::mat4 propellerMatrix2 = glm::translate(helicopterMatrix, glm::vec3(0.65f, 0.22f, 0.0f));
    propellerAngle2 += propellerSpeed * deltaTimeSeconds;
    propellerMatrix2 = glm::rotate(propellerMatrix2, -propellerAngle2, glm::vec3(0, 1, 0));
    SetModelProjectView(shader, propellerMatrix2);
    meshes["Propeller"]->Render();

    glm::mat4 propellerMatrix3 = glm::translate(helicopterMatrix, glm::vec3(-0.9f, 0.0f, 0.20f));
    propellerMatrix3 = glm::scale(propellerMatrix3, glm::vec3(0.4f, 0.4f, 0.4f));
    propellerMatrix3 = glm::rotate(propellerMatrix3, -1.5708f, glm::vec3(1, 0, 0));
    propellerMatrix3 = glm::rotate(propellerMatrix3, propellerAngle, glm::vec3(0, 1, 0));
    SetModelProjectView(shader, propellerMatrix3);
    meshes["Propeller"]->Render();

    glm::mat4 propellerMatrix4 = glm::translate(helicopterMatrix, glm::vec3(-0.9f, 0.0f, 0.20f));
    propellerMatrix4 = glm::scale(propellerMatrix4, glm::vec3(0.4f, 0.4f, 0.4f));
    propellerMatrix4 = glm::rotate(propellerMatrix4, -1.5708f, glm::vec3(1, 0, 0));
    propellerMatrix4 = glm::rotate(propellerMatrix4, propellerAngle2, glm::vec3(0, 1, 0));
    SetModelProjectView(shader, propellerMatrix4);
    meshes["Propeller"]->Render();

}

void Tema::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input
   
}

glm::vec3 Tema::GenerateRandomVector() {

    std::random_device rd; // Non-deterministic random seed
    std::mt19937 gen(rd()); // Mersenne Twister generator
    std::uniform_real_distribution<float> dist(-5.0f, 5.0f);

    float x = dist(gen);
    float z = dist(gen);
    return glm::vec3(x, 1.0f, z);
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

    if (key == GLFW_KEY_M) {
       
        targetHelicopterPosition = GenerateRandomVector();
		helicopterVector->SetEnd(targetHelicopterPosition);
		helicopterVector->SetStart(helicopterPosition);
		helicopterCurrentTime = 0.0f;

		helicopterDirection = glm::normalize(targetHelicopterPosition - helicopterPosition);
		printf("Helicopter direction: %f %f %f\n", helicopterDirection.x, helicopterDirection.y, helicopterDirection.z);
        float angle = MapAtan2To360(helicopterDirection.z, helicopterDirection.x);
        printf("Angle: %f\n", angle);
        printf("Angle: %f\n", glm::degrees(angle));

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