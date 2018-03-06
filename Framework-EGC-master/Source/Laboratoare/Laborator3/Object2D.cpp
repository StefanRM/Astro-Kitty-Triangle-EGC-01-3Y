#include "Object2D.h"

#include <Core/Engine.h>

#include <iostream>

using namespace std;

Mesh* Object2D::CreateSquare(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, bool fill)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices =
	{
		VertexFormat(corner, color),
		VertexFormat(corner + glm::vec3(length, 0, 0), color),
		VertexFormat(corner + glm::vec3(length, length, 0), color),
		VertexFormat(corner + glm::vec3(0, length, 0), color)
	};

	Mesh* square = new Mesh(name);
	std::vector<unsigned short> indices = { 0, 1, 2, 3 };
	
	if (!fill) {
		square->SetDrawMode(GL_LINE_LOOP);
	}
	else {
		// draw 2 triangles. Add the remaining 2 indices
		indices.push_back(0);
		indices.push_back(2);
	}

	square->InitFromData(vertices, indices);
	return square;
}

Mesh* Object2D::CreateCircle(std::string name, glm::vec3 leftBottomCorner, int nr_triangles, glm::vec3 color)
{
	glm::vec3 corner = leftBottomCorner;

	vector<VertexFormat> vertices_circle;

	vertices_circle.push_back(VertexFormat(corner, color)); // center
	for (int i = 0; i < nr_triangles; i++) // add vertices equal to the number of triangles
	{
		float theta = 2.0f * 3.1415926f * float(i) / float(nr_triangles); //get the current angle

		float x = cosf(theta); //calculate the x component
		float y = sinf(theta); //calculate the y component

		vertices_circle.push_back(VertexFormat(corner + glm::vec3(x, y, 0), color));
	}

	vector<unsigned short> indices_circle;

	// add (nr_triangles - 1) triangles
	for (int i = 2; i <= nr_triangles; i++)
	{
		indices_circle.push_back(0);
		indices_circle.push_back(i - 1);
		indices_circle.push_back(i);
	};

	// add the last triangle
	indices_circle.push_back(0);
	indices_circle.push_back(nr_triangles);
	indices_circle.push_back(1);

	Mesh* circle = new Mesh(name);

	circle->InitFromData(vertices_circle, indices_circle);
	return circle;
}

// this function has the same idea as Object2D::CreateSquare();
Mesh * Object2D::CreateRectangle(std::string name, glm::vec3 leftBottomCorner, float length, float width, glm::vec3 colorDiag1, glm::vec3 colorDiag2)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices_rectangle =
	{
		VertexFormat(corner, colorDiag1),
		VertexFormat(corner + glm::vec3(length, 0, 0), colorDiag2),
		VertexFormat(corner + glm::vec3(length, width, 0), colorDiag1),
		VertexFormat(corner + glm::vec3(0, width, 0), colorDiag2)
	};

	Mesh* rectangle = new Mesh(name);
	std::vector<unsigned short> indices_rectangle = { 0, 1, 2, 3 };

	// draw 2 triangles. Add the remaining 2 indices
	indices_rectangle.push_back(0);
	indices_rectangle.push_back(2);

	rectangle->InitFromData(vertices_rectangle, indices_rectangle);
	return rectangle;
}

Mesh * Object2D::CreateEquilateralTriangle(std::string name, glm::vec3 leftBottomCorner, float length, glm::vec3 color, glm::vec3 colorHead)
{
	glm::vec3 corner = leftBottomCorner;

	std::vector<VertexFormat> vertices_triangle =
	{
		VertexFormat(corner, colorHead),
		VertexFormat(corner - glm::vec3(length / 2.0, length * sqrt(3) / 2.0, 0), color),
		VertexFormat(corner - glm::vec3(-length / 2.0, length * sqrt(3) / 2.0, 0), color)
	};

	Mesh* triangle = new Mesh(name);
	std::vector<unsigned short> indices_triangle = { 0, 1, 2 }; // one triangle to create one triangle

	triangle->InitFromData(vertices_triangle, indices_triangle);
	return triangle;
}
