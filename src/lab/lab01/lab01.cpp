#include "lab/lab01/lab01.h"

#include <vector>

using namespace std;
using namespace lab;

Lab01::Lab01()
{
    window->SetSize(1280, 720);
}


Lab01::~Lab01()
{
}

void Lab01::Initialize()
{
    Exercise1();
    Exercise2();
    Exercise3();
    Exercise4();
    Exercise5();
    Exercise6();
}

void Lab01::Exercise1()
{
    SetExercise();

    //TODO(student): Ex. 1
    {
        SetPanel("1", { -3, -2, 100 }, { 0, 0, 1280, 720 });

        glm::vec2 v1(5, 2);
        glm::vec2 v2(2, 3);

        glm::vec2 v12 = v1 + v2;

        RenderVector(v1, glm::vec3(1, 0, 0), "v1");
        RenderVector(v2, glm::vec3(0, 1, 0), "v2");

        RenderVector(v12, glm::vec3(0, 0, 1), "v12");

        RenderVector(v1, glm::vec3(1, 0, 0), "v1", v2);
        RenderVector(v2, glm::vec3(0, 1, 0), "v2", v1);
    }
}

void Lab01::Exercise2()
{
    SetExercise();

    //TODO(student): Ex. 2
    {
        SetPanel("2", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        {
            glm::vec2 p1(1, 1);
            glm::vec2 p2(3, -3);

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");

            glm::vec2 v21 = p1 - p2;
            RenderVector(v21, glm::vec3(1, 0, 1), "v21", p2);
        }

        {
            glm::vec2 p3(-1, -2);
            glm::vec2 p4(-5, -4);

            RenderPoint(p3, glm::vec3(1, 0, 0), "P3");
            RenderPoint(p4, glm::vec3(0, 0, 1), "P4");

            glm::vec2 v43 = p3 - p4;
            RenderVector(v43, glm::vec3(1, 0, 1), "v43", p4);
        }

        {
            glm::vec2 p5(6, 3);
            glm::vec2 p6(4, 1);

            RenderPoint(p5, glm::vec3(1, 0, 0), "P5");
            RenderPoint(p6, glm::vec3(0, 0, 1), "P6");

            glm::vec2 v56 = p6 - p5;
            RenderVector(v56, glm::vec3(1, 0, 1), "v56", p5);
        }

        {
            glm::vec2 p7(5, 4);
            glm::vec2 p8(-3, 4);

            RenderPoint(p7, glm::vec3(1, 0, 0), "P7");
            RenderPoint(p8, glm::vec3(0, 0, 1), "P8");

            glm::vec2 v87 = p7 - p8;
            RenderVector(v87, glm::vec3(1, 0, 1), "v87", p8);
        }
    }
}

void Lab01::Exercise3()
{
    SetExercise();

    //TODO(student): Ex. 3
    {
        SetPanel("3", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        glm::vec2 v1(4, 3);
        glm::vec2 v2(-3, 3);
        glm::vec2 v3(-6, -2);
        glm::vec2 v4(-2, -4);
        glm::vec2 v5(3, -2);

        RenderVector(v1, glm::vec3(1, 0, 0), "v1");
        RenderVector(v2, glm::vec3(0, 1, 0), "v2");
        RenderVector(v3, glm::vec3(0, 1, 1), "v3");
        RenderVector(v4, glm::vec3(0, 0, 1), "v4");
        RenderVector(v5, glm::vec3(1, 0, 1), "v5");


        RenderVector(glm::normalize(v1), glm::vec3(0, 0, 0));
        RenderVector(glm::normalize(v2), glm::vec3(0, 0, 0));
        RenderVector(glm::normalize(v3), glm::vec3(0, 0, 0));
        RenderVector(glm::normalize(v4), glm::vec3(0, 0, 0));
        RenderVector(glm::normalize(v5), glm::vec3(0, 0, 0));
    }
}

void Lab01::Exercise4()
{
    SetExercise ();

    //TODO(student): Ex. 4
    {
        SetPanel("4", { -3, -2, 100 }, { 0, 0, 1280, 720 });

        glm::vec2 v1(4, 1.5);
        glm::vec2 v2(2, 3);

        float start_angle = acos(glm::dot(glm::vec2(1, 0), glm::normalize(v1)));
        float angle = acos(glm::dot(glm::normalize(v1), glm::normalize(v2)));

        RenderVector(v1, glm::vec3(1, 0, 0), "v1");
        RenderVector(v2, glm::vec3(0, 1, 0), "v2");

        RenderArc(start_angle, angle, glm::vec3(0, 0, 1));

        RenderArc(start_angle + angle, glm::radians(360.0f) - angle, glm::vec3(1, 0, 0), 1.0f);
    }
}

void Lab01::Exercise5()
{
    SetExercise();

    //TODO(student): Ex. 5
    {
        SetPanel("5", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        {
            glm::vec2 p1(1, 1);
            glm::vec2 p2(3, 1);
            glm::vec2 p3(2, 4);

            RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
            RenderPoint(p3, glm::vec3(0, 1, 0), "P3");

            float area = calculateArea(p1, p2, p3);

            glm::vec2 center = glm::vec2(
                (p1.x + p2.x + p3.x) / 3.0f,
                (p1.y + p2.y + p3.y) / 3.0f
            );

            RenderText(center, std::to_string(area), glm::vec3(0, 0, 0));

        }

        {
            glm::vec2 p1(-1.5, -3.7);
            glm::vec2 p2(-2.5, 3.2);
            glm::vec2 p3(-6.1, 2.9);

            RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
            RenderPoint(p3, glm::vec3(0, 1, 0), "P3");

            float area = calculateArea(p1, p2, p3);

            glm::vec2 center = glm::vec2(
                (p1.x + p2.x + p3.x) / 3.0f,
                (p1.y + p2.y + p3.y) / 3.0f
            );

            RenderText(center, std::to_string(area), glm::vec3(0, 0, 0));
        }

        {
            glm::vec2 p1(2.3, -2.1);
            glm::vec2 p2(7.1, -3.5);
            glm::vec2 p3(7.5, 3.7);

            RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

            RenderPoint(p1, glm::vec3(1, 0, 0), "P1");
            RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
            RenderPoint(p3, glm::vec3(0, 1, 0), "P3");

            float area = calculateArea(p1, p2, p3);

            glm::vec2 center = glm::vec2(
                (p1.x + p2.x + p3.x) / 3.0f,
                (p1.y + p2.y + p3.y) / 3.0f
            );

            RenderText(center, std::to_string(area), glm::vec3(0, 0, 0));
        }
    }
}

void Lab01::Exercise6()
{
    SetExercise();

    //TODO(student): Ex. 6
    {
        SetPanel("6", { -7, -5, 75 }, { 0, 0, 1280, 720 });

        const float EPSILON = 0.01f;

        glm::vec2 p1(-4, 3);
        glm::vec2 p2(6, 2);
        glm::vec2 p3(1, -4);

        RenderTriangle(p1, p2, p3, glm::vec3(0, 1, 1));

        RenderPoint(p1, glm::vec3(0, 0, 1), "P1");
        RenderPoint(p2, glm::vec3(0, 0, 1), "P2");
        RenderPoint(p3, glm::vec3(0, 0, 1), "P3");

        std::vector<glm::vec2> check_points{
            glm::vec2(1, 1), glm::vec2(3.21, 3.2),
            glm::vec2(-2.16, 2.9), glm::vec2(2, -2),
            glm::vec2(-1.1, -0.9), glm::vec2(-1.9, 2.1),
            glm::vec2(-4, 0), glm::vec2(-4, -2.5),
            glm::vec2(4.5, 1.1), glm::vec2(5.9, -3.1),
            glm::vec2(7.1, 1.9), glm::vec2(-5.9, 1.5)
        };

        for (const glm::vec2 &check_point : check_points) {
            float area = calculateArea(p1, p2, p3);
            float area1 = calculateArea(check_point, p2, p3);
            float area2 = calculateArea(p1, check_point, p3);
            float area3 = calculateArea(p1, p2, check_point);

            bool isIn = abs(area - (area1 + area2 + area3)) < EPSILON;

            std::string status = isIn ? "in" : "out";
            glm::vec3 color = isIn ? glm::vec3(0, 1, 0) : glm::vec3(1, 0, 0);

            RenderPoint(check_point, color, status);
        }
    }

}

float Lab01::calculateArea(glm::vec2 p1, glm::vec2 p2, glm::vec2 p3) {
    float a = glm::length(p1 - p2);
    float b = glm::length(p1 - p3);
    float c = glm::length(p2 - p3);

    float s = 0.5f * (a + b + c);


    return sqrt(s * (s - a) * (s - b) * (s - c));
}
