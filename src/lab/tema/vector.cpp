#include "vector.h"
#include "GL/glew.h"

using namespace lab;
using namespace std;

Vector::Vector()
{
	start = glm::vec3(0);
	end = glm::vec3(0);
	SetupBuffers();
};

Vector::Vector(glm::vec3 start, glm::vec3 end) {
	this->start = start;
	this->end = end;
	SetupBuffers();
}

Vector::~Vector()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Vector::Draw() {
	glBindVertexArray(VAO);
	glDrawArrays(GL_LINES, 0, 2);
	glBindVertexArray(0);
}

void Vector::SetEnd(glm::vec3 newEnd) {
	end = newEnd;

	// Update buffer data
	float vectorVertices[6] = {
		start.x, start.y, start.z,
		end.x, end.y, end.z
	};

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vectorVertices), vectorVertices);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Vector::SetStart(glm::vec3 newStart) {
    start = newStart;

    // Update buffer data
    float vectorVertices[6] = {
        start.x, start.y, start.z,
        end.x, end.y, end.z
    };

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vectorVertices), vectorVertices);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Vector::SetupBuffers() {
    float vectorVertices[6] = {
        start.x, start.y, start.z,
        end.x, end.y, end.z
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vectorVertices), vectorVertices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
