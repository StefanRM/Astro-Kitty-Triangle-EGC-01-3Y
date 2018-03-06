#pragma once

#include <string>

#include <include/glm.h>
#include <Core/GPU/Mesh.h>

namespace Object2D
{

	// Create square with given bottom left corner, length and color
	Mesh* CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill = false);

	// Create circle with given center and color (the radius will be 1)
	Mesh* CreateCircle(std::string name, glm::vec3 leftBottomCorner, int nr_triangles, glm::vec3 color);

	// Create rectangle with given bottom left corner, length, width and color on diagonal corners
	Mesh* CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 colorDiag1, glm::vec3 colorDiag2);

	// Create equilateral triangle with given corner (peak), length of the triangle side and colors of peak and base corners
	Mesh* CreateEquilateralTriangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, glm::vec3 colorHead);
}

