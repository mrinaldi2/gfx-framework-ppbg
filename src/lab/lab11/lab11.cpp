#include "lab/lab11/lab11.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace lab;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Lab11::Lab11()
{
    // Parameters related to surface generation
    no_of_generated_points = 10;            // number of points on a Bezier curve
    no_of_instances = 5;                    // number of instances (number of curves that contain the surface)
    max_translate = 8.0f;                   // for the translation surface, it's the distance between the first and the last curve
    max_rotate = glm::radians(360.0f);      // for the rotation surface, it's the angle between the first and the last curve

    surface_type = 0;

    // Define control points
    control_p0 = glm::vec3(-4.0, -2.5, 1.0);
    control_p1 = glm::vec3(-2.5, 1.5, 1.0);
    control_p2 = glm::vec3(-1.5, 3.0, 1.0);
    control_p3 = glm::vec3(-4.0, 5.5, 1.0);
}

Lab11::~Lab11()
{
}

void Lab11::Init()
{
    auto camera = GetSceneCamera();
    camera->SetPositionAndRotation(glm::vec3(0, 8, 8), glm::quat(glm::vec3(-40 * TO_RADIANS, 0, 0)));
    camera->Update();

    // Create a shader program for surface generation
    {
        Shader *shader = new Shader("SurfaceGeneration");
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "lab11", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "lab11", "shaders", "GeometryShader.glsl"), GL_GEOMETRY_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, "src/lab", "lab11", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // Create a bogus mesh with 2 points (a line)
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(control_p0, glm::vec3(0, 1, 1)),
            VertexFormat(control_p3, glm::vec3(0, 1, 0))
        };

        vector<unsigned int> indices =
        {
            0, 1
        };

        meshes["surface"] = new Mesh("generated initial surface points");
        meshes["surface"]->InitFromData(vertices, indices);
        meshes["surface"]->SetDrawMode(GL_LINES);
    }
}

void Lab11::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.121, 0.168, 0.372, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Lab11::Update(float deltaTimeSeconds)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Draw the object instanced
    RenderMeshInstanced(meshes["surface"], shaders["SurfaceGeneration"], glm::mat4(1), no_of_instances);
}

void Lab11::FrameEnd()
{
#if 0
    DrawCoordinateSystem();
#endif
}

void Lab11::RenderMeshInstanced(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, int instances, const glm::vec3& color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader
    glUseProgram(shader->program);

    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Send uniforms to shaders
    glUniform3f(glGetUniformLocation(shader->program, "control_p0"), control_p0.x, control_p0.y, control_p0.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p1"), control_p1.x, control_p1.y, control_p1.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p2"), control_p2.x, control_p2.y, control_p2.z);
    glUniform3f(glGetUniformLocation(shader->program, "control_p3"), control_p3.x, control_p3.y, control_p3.z);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_instances"), no_of_instances);
	

    // TODO(student): Send to the shaders the number of points that approximate
    // a curve (no_of_generated_points), as well as the characteristics for
    // creating the translation/rotation surfaces (max_translate, max_rotate)
    // and the type of generated surface (surface_type) based on which the
    // rendered surface is changed.
    // NOTE: If you're feeling lost and need a frame of reference while doing
    // this lab, go to `FrameEnd()` and activate `DrawCoordinateSystem()`.
	glUniform1f(glGetUniformLocation(shader->program, "max_translate"), max_translate);
    glUniform1i(glGetUniformLocation(shader->program, "no_of_generated_points"), no_of_generated_points);

    // Draw the object instanced
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElementsInstanced(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, (void*)0, instances);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Lab11::OnInputUpdate(float deltaTime, int mods)
{
    // Treat continuous update based on input

    // You can move the control points around by using the dedicated key,
    // in combination with Ctrl, Shift, or both.
    float delta = deltaTime;
    auto keyMaps = std::vector<std::pair<glm::vec3 &, uint32_t>>
    {
        { control_p0, GLFW_KEY_1 },
        { control_p1, GLFW_KEY_2 },
        { control_p2, GLFW_KEY_3 },
        { control_p3, GLFW_KEY_4 }
    };

    for (const auto &k : keyMaps)
    {
        if (window->KeyHold(k.second))
        {
            if (mods & GLFW_MOD_SHIFT && mods & GLFW_MOD_CONTROL)
            {
                k.first.y -= delta;
            }
            else if (mods & GLFW_MOD_CONTROL)
            {
                k.first.y += delta;
            }
            else if (mods & GLFW_MOD_SHIFT)
            {
                k.first.x -= delta;
            }
            else
            {
                k.first.x += delta;
            }

            std::cout << glm::vec2(control_p0) << glm::vec2(control_p1) << glm::vec2(control_p2) << glm::vec2(control_p3) << "\n";
        }
    }
}


void Lab11::OnKeyPress(int key, int mods)
{
    // TODO(student): Use keys to change the number of instances and the
    // number of generated points. Avoid the camera keys, and avoid the
    // the keys from `OnInputUpdate`.

}


void Lab11::OnKeyRelease(int key, int mods)
{
    // Add key release event
    if (key == GLFW_KEY_KP_SUBTRACT)
    {
        if (no_of_generated_points > 0) {
            no_of_generated_points--;
        }
    }

    if (key == GLFW_KEY_KP_ADD)
    {
        no_of_generated_points++;
    }

	if (key == GLFW_KEY_KP_MULTIPLY)
	{
		no_of_instances++;
	}

	if (key == GLFW_KEY_KP_DIVIDE)
	{
		if (no_of_instances > 0)
		{
			no_of_instances--;
		}
	}
}


void Lab11::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Lab11::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Lab11::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Lab11::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
    // Treat mouse scroll event
}


void Lab11::OnWindowResize(int width, int height)
{
    // Treat window resize event
}
