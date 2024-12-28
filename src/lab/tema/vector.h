#pragma once

#include "glm/glm.hpp"

namespace lab
{
	class Vector
	{
	public:
		Vector();
		Vector(glm::vec3 start, glm::vec3 end);
		~Vector();

		void SetupBuffers();
		void Draw();
		void SetStart(glm::vec3 newStart);
		void SetEnd(glm::vec3 newEnd);
	private:
		unsigned int VAO, VBO;
		unsigned int shaderProgram;
		glm::vec3 start;
		glm::vec3 end;

	};
}