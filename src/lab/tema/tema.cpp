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

}

void Tema::FrameStart()
{
    ClearScreen();
}


void Tema::Update(float deltaTimeSeconds) {

}

void Tema::FrameEnd()
{

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
