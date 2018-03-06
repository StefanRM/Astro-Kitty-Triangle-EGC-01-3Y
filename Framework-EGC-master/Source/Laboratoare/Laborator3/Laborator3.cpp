#include "Laborator3.h"

#include <vector>
#include <iostream>

#include <Core/Engine.h>
#include "Transform2D.h"
#include "Object2D.h"

using namespace std;

Laborator3::Laborator3()
{
}

Laborator3::~Laborator3()
{
}

void Laborator3::Init()
{
	printf("\nTema 1 - EGC - AstroKitty\nAuthor: Maftei Stefan - Radu\nGrupa: 336CC\n\nIntro: You're an astronaut (the triangle) which tries to reach the finish platform (the platform with 2 colors). To achieve this goal you may use the Landing Platforms (the red rectangles) and Reflection Platforms (the cyan rectangles). Because you're flying through space you have some massive obstacles: the meteorites... However, you can destroy them by hitting them, but you will be deflected in the opposite direction from the one you came from.\n\nControl: Left Mouse Button Click on the starting direction. When the astronaut landed, you may reuse this control.\n\nTo infinity and beyond!...\n\n");

	glm::ivec2 resolution = window->GetResolution();
	auto camera = GetSceneCamera();
	camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
	camera->SetPosition(glm::vec3(0, 0, 50));
	camera->SetRotation(glm::vec3(0, 0, 0));
	camera->Update();
	GetCameraInput()->SetActive(false);


	// initialization of left bottom corner, dimensions and permitted error
	glm::vec3 corner = glm::vec3(0, 0, 0);
	rectangleWidth = 50;
	rectangleLength = 6 * rectangleWidth;
	triangleSide = 70;
	triangleHeight = triangleSide * sqrt(3) / 2.0;
	centerTriangleX = triangleSide / 2.0;
	centerTriangleY = triangleHeight / 3.0;
	triangleCircleRad = 2.0 * triangleHeight / 3.0;
	currTriangleLeftX = 0, currTriangleLeftY = 0, currTriangleRightX = triangleSide, currTriangleRightY = 0;
	permittedError = 2 * triangleHeight / 3.0;

	// initialize the translation steps
	translateX = 0;
	translateY = 0;

	// initialize the scale factors
	scaleX = 1;
	scaleY = 1;

	// initialize angularStep
	angularStep = 0;

	// initialize current position of astronaut
	currPosX = triangleSide / 2.0;
	currPosY = triangleSide * sqrt(3) / 2.0;

	// create meshes for all the figures in the game
	Mesh* meteoriteRot = Object2D::CreateCircle("meteoriteRot", corner, 8, glm::vec3(0.6, 0, 0.6));
	AddMeshToList(meteoriteRot);

	Mesh* meteoriteTrans = Object2D::CreateCircle("meteoriteTrans", corner, 8, glm::vec3(0.6, 0, 0.6));
	AddMeshToList(meteoriteTrans);

	Mesh* meteoriteScale = Object2D::CreateCircle("meteoriteScale", corner, 8, glm::vec3(0.6, 0, 0.6));
	AddMeshToList(meteoriteScale);

	Mesh* platformLanding = Object2D::CreateRectangle("platformLanding", corner, rectangleLength, rectangleWidth, glm::vec3(1, 0, 0), glm::vec3(1, 0, 0));
	AddMeshToList(platformLanding);

	Mesh* platformReflection = Object2D::CreateRectangle("platformReflection", corner, rectangleWidth, rectangleLength, glm::vec3(0, 1, 1), glm::vec3(0, 1, 1));
	AddMeshToList(platformReflection);

	Mesh* platformFinish = Object2D::CreateRectangle("platformFinish", corner, rectangleLength, rectangleWidth, glm::vec3(1, 1, 0), glm::vec3(1, 0, 0));
	AddMeshToList(platformFinish);

	Mesh* astronaut = Object2D::CreateEquilateralTriangle("astronaut", corner, triangleSide, glm::vec3(0, 1, 0), glm::vec3(0, 0, 1));
	AddMeshToList(astronaut);
}

void Laborator3::FrameStart()
{
	// clears the color buffer (using the previously set color) and depth buffer
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::ivec2 resolution = window->GetResolution();
	// sets the screen area where to draw
	glViewport(0, 0, resolution.x, resolution.y);
}

void Laborator3::Update(float deltaTimeSeconds)
{
	if (existMeteoriteRot) { // rotation meteorite
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(meteoriteRotX, meteoriteRotY) * Transform2D::Rotate(angularStep) * Transform2D::Translate(meteoriteRadiusRot, meteoriteRadiusRot) * Transform2D::Scale(meteoriteRadiusRot, meteoriteRadiusRot);
		RenderMesh2D(meshes["meteoriteRot"], shaders["VertexColor"], modelMatrix);
	}

	if (existMeteoriteTrans) { // translation meteorite
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(meteoriteTransX + translateX, meteoriteTransY + translateY) * Transform2D::Rotate(angularStep) * Transform2D::Scale(meteoriteRadiusRot, meteoriteRadiusRot);
		RenderMesh2D(meshes["meteoriteTrans"], shaders["VertexColor"], modelMatrix);
	}

	if (existMeteoriteScale) { // scaling meteorite
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(meteoriteScaleX, meteoriteScaleY) * Transform2D::Rotate(angularStep) * Transform2D::Scale(meteoriteRadiusRot * scaleX, meteoriteRadiusRot * scaleY);
		RenderMesh2D(meshes["meteoriteScale"], shaders["VertexColor"], modelMatrix);
	}

	// landing platform
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(platformLandingcoordX, platformLandingcoordY);
	RenderMesh2D(meshes["platformLanding"], shaders["VertexColor"], modelMatrix);

	// reflection platform
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(platformReflectioncoordX, platformReflectioncoordY);
	RenderMesh2D(meshes["platformReflection"], shaders["VertexColor"], modelMatrix);

	// finish platform
	modelMatrix = glm::mat3(1);
	modelMatrix *= Transform2D::Translate(platformFinishcoordX, platformFinishcoordY);
	RenderMesh2D(meshes["platformFinish"], shaders["VertexColor"], modelMatrix);

	if (!platformLandingHit) { // astronaut moves so we have to orientate him in the certain position
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(currPosX, currPosY);
		modelMatrix *= Transform2D::Rotate(angleToDest);
		RenderMesh2D(meshes["astronaut"], shaders["VertexColor"], modelMatrix);
	}
	else { // astronaut landed so we have to put him in a perpendicular position
		modelMatrix = glm::mat3(1);
		modelMatrix *= Transform2D::Translate(currPosX, currPosY);
		if (ascending) { // upside down orientation
			modelMatrix *= Transform2D::Rotate(pi);
		}
		RenderMesh2D(meshes["astronaut"], shaders["VertexColor"], modelMatrix);
	}
}

void Laborator3::FrameEnd()
{

}

bool Laborator3::isColliding(float platformX, float platformY, float width, float length) {
	// centers distances
	float distCentersX = abs(centerTriangleX - platformX - length / 2.0);
	float distCentersY = abs(centerTriangleY - platformY - width / 2.0);

	if (distCentersX > (length / 2.0 + triangleCircleRad)) {
		return false;
	}

	if (distCentersY > (width / 2.0 + triangleCircleRad)) {
		return false;
	}

	if (distCentersX <= (length / 2.0)) {
		return true;
	}

	if (distCentersY <= (width / 2.0)) {
		return true;
	}

	// distances as if we want to check if the triangle touches one of the rectangle's sides
	float distToRectSidex = distCentersX - length / 2.0;
	float distToRectSideY = distCentersY - width / 2.0;

	return (distToRectSidex * distToRectSidex + distToRectSideY * distToRectSideY <= (triangleCircleRad * triangleCircleRad));
}

void Laborator3::newCoordinatesForTrianglePoints(float* centerTriangleX, float* centerTriangleY, float* currTriangleLeftX, float* currTriangleLeftY, float* currTriangleRightX, float* currTriangleRightY) {
	// rotate the center of the triangle around the peak of the triangle with the new angle
	float oldCenterTriangleX = *centerTriangleX, oldCenterTriangleY = *centerTriangleY;
	*centerTriangleX = (oldCenterTriangleX - currPosX) * cosf(-oldAngleToDest + angleToDest) - (oldCenterTriangleY - currPosY) * sinf(-oldAngleToDest + angleToDest) + currPosX;
	*centerTriangleY = (oldCenterTriangleY - currPosY) * cosf(-oldAngleToDest + angleToDest) + (oldCenterTriangleX - currPosX) * sinf(-oldAngleToDest + angleToDest) + currPosY;

	// rotate the base point of the triangle around the peak of the triangle with the new angle
	float oldTriangleLeftX = *currTriangleLeftX, oldTriangleLeftY = *currTriangleLeftY;
	*currTriangleLeftX = (oldTriangleLeftX - currPosX) * cosf(-oldAngleToDest + angleToDest) - (oldTriangleLeftY - currPosY) * sinf(-oldAngleToDest + angleToDest) + currPosX;
	*currTriangleLeftY = (oldTriangleLeftY - currPosY) * cosf(-oldAngleToDest + angleToDest) + (oldTriangleLeftX - currPosX) * sinf(-oldAngleToDest + angleToDest) + currPosY;

	float oldTriangleRightX = *currTriangleRightX, oldTriangleRightY = *currTriangleRightY;
	*currTriangleRightX = (oldTriangleRightX - currPosX) * cosf(-oldAngleToDest + angleToDest) - (oldTriangleRightY - currPosY) * sinf(-oldAngleToDest + angleToDest) + currPosX;
	*currTriangleRightY = (oldTriangleRightY - currPosY) * cosf(-oldAngleToDest + angleToDest) + (oldTriangleRightX - currPosX) * sinf(-oldAngleToDest + angleToDest) + currPosY;
}

void Laborator3::OnInputUpdate(float deltaTime, int mods)
{
		// meteorites motion parameters set
		angularStep += 1.5f * deltaTime;
		scaleX += deltaTime * (reverse_scale);
		scaleY += deltaTime * (reverse_scale);
		translateX += 200 * deltaTime * reverse;

		// translation boundaries
		if (translateX >= 400 || translateX < 0) {
			reverse = (-1) * reverse;
		}

		// scaling boundaries
		if (scaleX >= 3 || scaleX < 1) {
			reverse_scale = (-1) * reverse_scale;
		}

	if (clicked) { // click event
		changeDirX = 1;
		changeDirY = 1;
		angleToDest_tg = (float)(destinationX - currPosX) / (float)(destinationY - currPosY); // slope
		deltaX = (float) (destinationX - currPosX) / sqrt((destinationX - currPosX) * (destinationX - currPosX) + (destinationY - currPosY) * (destinationY - currPosY));
		deltaY = (float)(destinationY - currPosY) / sqrt((destinationX - currPosX) * (destinationX - currPosX) + (destinationY - currPosY) * (destinationY - currPosY));
		if (destinationY < currPosY ) // depending on the orientation we have to add a pi to the angle
		{
			angleToDest = -atan(angleToDest_tg) + pi;
			ascending = false;
		}
		else {
			angleToDest = -atan(angleToDest_tg);
			ascending = true;
		}

		if (destinationX > currPosX) {
			toTheRight = true;
		}
		else {
			toTheRight = false;
		}

		bool canContinue = true; // variable for checking if the astronaut can launch
		
		if (platformLandingHit) { // while landed, the astronaut cannot launch through the platform
			if (centerTriangleY < platformLandingcoordY && ascending) {
				canContinue = false;
			}
			if (centerTriangleY > platformLandingcoordY && !ascending) {
				canContinue = false;
			}
		}

		if (canContinue) { // if he can launch safely
			clicked = false;
			move = true;
			platformLandingHit = false;
			reflectedPlatform = false;
			launching = true;
			beforeLaunchX = currPosX;
			beforeLaunchY = currPosY;

			// calculate new coordinates based on the last ones
			newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);

			oldAngleToDest = angleToDest;
		}
	}

	if (move) { // movement
		// collision with the landing platform
		if (!launching && isColliding(platformLandingcoordX, platformLandingcoordY, rectangleWidth, rectangleLength)) {
			move = false;
			platformLandingHit = true;

			// calculate new coordinates
			currPosX = platformLandingcoordX + rectangleLength / 2.0;
			currPosY = platformLandingcoordY + rectangleWidth / 2.0;
			centerTriangleX = platformLandingcoordX + rectangleLength / 2.0;

			// based on the ascendin / descending movement we have some variations for the calculation
			if (ascending) {
				currPosY -= triangleHeight + rectangleWidth / 2.0;
				centerTriangleY = currPosY + 2.0 * triangleHeight / 3.0;
				oldAngleToDest = pi; // it is upside-down

				currTriangleLeftX = platformLandingcoordX + rectangleLength / 2.0 + triangleSide / 2.0;
				currTriangleRightX = platformLandingcoordX + rectangleLength / 2.0 - triangleSide / 2.0;
				currTriangleLeftY = platformLandingcoordY;
				currTriangleRightY = platformLandingcoordY;
			}
			else {
				currPosY += triangleHeight + rectangleWidth / 2.0;
				centerTriangleY = currPosY - 2.0 * triangleHeight / 3.0;
				oldAngleToDest = 0;

				currTriangleLeftX = platformLandingcoordX + rectangleLength / 2.0 - triangleSide / 2.0;
				currTriangleRightX = platformLandingcoordX + rectangleLength / 2.0 + triangleSide / 2.0;
				currTriangleLeftY = platformLandingcoordY + rectangleWidth;
				currTriangleRightY = platformLandingcoordY + rectangleWidth;
			}
		}

		// colision with the finish platform
		if (!launching && isColliding(platformFinishcoordX, platformFinishcoordY, rectangleWidth, rectangleLength)) {
			move = false;
			finished = true;
			angleToDest = 0;
			currPosX = platformFinishcoordX + rectangleLength / 2.0;
			currPosY = rectangleWidth + triangleHeight;

			printf("CONGRATULATION! You beat the game!\nYou can now close the app window...\n");
		}

		// collision with the reflection platform
		else if (!reflectedPlatform && isColliding(platformReflectioncoordX, platformReflectioncoordY, rectangleLength, rectangleWidth)) {
			reflectedPlatform = true;
			oldAngleToDest = angleToDest;

			if (centerTriangleX <= platformReflectioncoordX) { // left side of the platform
				angleToDest = 2 * pi - angleToDest; // reflection angle
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0; // after rotation we have to translate outside the platform

				// depending on the orientation
				if (ascending) {
					translateRefX = -abs(platformReflectioncoordX - currTriangleRightX);
					translateRefY = abs(currPosY - currTriangleLeftY);
				}
				else {
					translateRefX = -abs(platformReflectioncoordX - currTriangleLeftX);
					translateRefY = -abs(currPosY - currTriangleRightY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirX *= -1; // direction is opposite
				toTheRight = !toTheRight;
			}
			else if (centerTriangleY <= platformReflectioncoordY) { // down side of the platform
				angleToDest = pi - angleToDest; // reflection angle
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0; // after rotation we have to translate outside the platform

				// depending on the orientation
				if (toTheRight) {
					translateRefX = abs(currPosX - currTriangleRightX);
					translateRefY = -abs(platformReflectioncoordY - currTriangleLeftY);
				}
				else {
					translateRefX = -abs(currPosX - currTriangleLeftX);
					translateRefY = -abs(platformReflectioncoordY - currTriangleRightY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirY *= -1; // direction is opposite
				ascending = !ascending;
			}
			else if (centerTriangleX >= platformReflectioncoordX + rectangleWidth) { // right side of the platform
				angleToDest = 2 * pi - angleToDest; // reflection angle
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0; // after rotation we have to translate outside the platform

				// depending on the orientation
				if (ascending) {
					translateRefX = abs(platformReflectioncoordX + rectangleWidth - currTriangleLeftX);
					translateRefY = abs(currPosY - currTriangleRightY);
				}
				else {
					translateRefX = abs(platformReflectioncoordX + rectangleWidth - currTriangleRightX);
					translateRefY = -abs(currPosY - currTriangleLeftY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirX *= -1; // direction is opposite
				toTheRight = !toTheRight;
			}
			else if (centerTriangleY >= platformReflectioncoordY + rectangleLength) { // up side of the platform
				angleToDest = pi - angleToDest; // reflection angle
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0; // after rotation we have to translate outside the platform
				
				// depending on the orientation
				if (toTheRight) {
					translateRefX = abs(currPosX - currTriangleLeftX);
					translateRefY = abs(platformReflectioncoordY + rectangleLength - currTriangleRightY);
				}
				else {
					translateRefX = -abs(currPosX - currTriangleRightX);
					translateRefY = abs(platformReflectioncoordY + rectangleLength - currTriangleLeftY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirY *= -1; // direction is opposite
				ascending = !ascending;
			}

			oldAngleToDest = angleToDest;
		}

		// walls landing
		else if (((currPosX >= (window->GetResolution()).x)) ||
				  (currPosX <= 0) ||
				  (currPosY >= (window->GetResolution()).y) ||
				  (currPosY <= 0)) {
			reflectedPlatform = false;
			move = false;
			
			// compute the new coordinates for the triangle points depending on the astronaut's orientation
			// right wall
			if (currPosX >= (window->GetResolution()).x) {
				if (ascending && toTheRight) {
					angleToDest = - 3 * pi / 2.0;
					oldAngleToDest = angleToDest;

					currTriangleRightX = currPosX;
					currTriangleRightY = currPosY;
					currTriangleLeftX = currPosX;
					currTriangleLeftY = currPosY - triangleSide;
					currPosX -= triangleHeight;
					currPosY = (currTriangleLeftY + currTriangleRightY) / 2.0;
					centerTriangleX = currPosX + 2 * triangleHeight / 3.0;
					centerTriangleY = currPosY;
				}

				if (!ascending && toTheRight) {
					angleToDest = -3 * pi / 2.0;
					oldAngleToDest = angleToDest;

					currTriangleLeftX = currPosX;
					currTriangleLeftY = currPosY;
					currTriangleRightX = currPosX;
					currTriangleRightY = currPosY + triangleSide;
					currPosX -= triangleHeight;
					currPosY = (currTriangleLeftY + currTriangleRightY) / 2.0;
					centerTriangleX = currPosX + 2 * triangleHeight / 3.0;
					centerTriangleY = currPosY;
				}
			}

			// left wall
			if (currPosX <= 0) {
				if (!ascending && !toTheRight) {
					angleToDest = - pi / 2.0;
					oldAngleToDest = angleToDest;

					currTriangleRightX = currPosX;
					currTriangleRightY = currPosY;
					currTriangleLeftX = currPosX;
					currTriangleLeftY = currPosY + triangleSide;
					currPosX += triangleHeight;
					currPosY = (currTriangleLeftY + currTriangleRightY) / 2.0;
					centerTriangleX = currPosX - 2 * triangleHeight / 3.0;
					centerTriangleY = currPosY;
				}

				if (ascending && !toTheRight) {
					angleToDest = - pi / 2.0;
					oldAngleToDest = angleToDest;

					currTriangleLeftX = currPosX;
					currTriangleLeftY = currPosY;
					currTriangleRightX = currPosX;
					currTriangleRightY = currPosY - triangleSide;
					currPosX += triangleHeight;
					currPosY = (currTriangleLeftY + currTriangleRightY) / 2.0;
					centerTriangleX = currPosX - 2 * triangleHeight / 3.0;
					centerTriangleY = currPosY;
				}
			}

			// top wall
			if (currPosY >= (window->GetResolution()).y) {
				if (ascending && !toTheRight) {
					angleToDest = pi;
					oldAngleToDest = angleToDest;

					currTriangleRightX = currPosX;
					currTriangleRightY = currPosY;
					currTriangleLeftX = currPosX + triangleSide;
					currTriangleLeftY = currPosY;
					currPosX = (currTriangleLeftX + currTriangleRightX) / 2.0;;
					currPosY -= triangleHeight;
					centerTriangleX = currPosX;
					centerTriangleY = currPosY + 2 * triangleHeight / 3.0;
				}

				if (ascending && toTheRight) {
					angleToDest = pi;
					oldAngleToDest = angleToDest;

					currTriangleLeftX = currPosX;
					currTriangleLeftY = currPosY;
					currTriangleRightX = currPosX - triangleSide;
					currTriangleRightY = currPosY;
					currPosX = (currTriangleLeftX + currTriangleRightX) / 2.0;
					currPosY -= triangleHeight;
					centerTriangleX = currPosX;
					centerTriangleY = currPosY + 2 * triangleHeight / 3.0;
				}
			}

			// bottom wall
			if (currPosY <= 0) {
				if (!ascending && toTheRight) {
					angleToDest = 0;
					oldAngleToDest = angleToDest;

					currTriangleRightX = currPosX;
					currTriangleRightY = currPosY;
					currTriangleLeftX = currPosX - triangleSide;
					currTriangleLeftY = currPosY;
					currPosX = (currTriangleLeftX + currTriangleRightX) / 2.0;;
					currPosY += triangleHeight;
					centerTriangleX = currPosX;
					centerTriangleY = currPosY - 2 * triangleHeight / 3.0;
				}

				if (!ascending && !toTheRight) {
					angleToDest = 0;
					oldAngleToDest = angleToDest;

					currTriangleLeftX = currPosX;
					currTriangleLeftY = currPosY;
					currTriangleRightX = currPosX + triangleSide;
					currTriangleRightY = currPosY;
					currPosX = (currTriangleLeftX + currTriangleRightX) / 2.0;
					currPosY += triangleHeight;
					centerTriangleX = currPosX;
					centerTriangleY = currPosY - 2 * triangleHeight / 3.0;
				}
			}
		}

		// hitting the meteorites are treated as reflection platforms, described above
		// meteorite translate
		else if (existMeteoriteTrans && isColliding(meteoriteTransX - meteoriteRadiusRot + translateX, meteoriteTransY - meteoriteRadiusRot + translateY, 2 * meteoriteRadiusRot, 2 * meteoriteRadiusRot)) {
			existMeteoriteTrans = false;
			reflectedPlatform = false;

			// we put the meteorite in a square for easy treatment
			float botLeftX = meteoriteTransX - meteoriteRadiusRot + translateX;
			float botLeftY = meteoriteTransY - meteoriteRadiusRot + translateY;
			float side = 2 * meteoriteRadiusRot;

			oldAngleToDest = angleToDest;

			if (centerTriangleX <= botLeftX) {
				angleToDest = 2 * pi - angleToDest;
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0;

				if (ascending) {
					translateRefX = -abs(botLeftX - currTriangleRightX);
					translateRefY = abs(currPosY - currTriangleLeftY);
				}
				else {
					translateRefX = -abs(botLeftX - currTriangleLeftX);
					translateRefY = -abs(currPosY - currTriangleRightY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirX *= -1;
				toTheRight = !toTheRight;
			}
			else if (centerTriangleY <= botLeftY) {
				angleToDest = pi - angleToDest;
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0;

				if (toTheRight) {
					translateRefX = abs(currPosX - currTriangleRightX);
					translateRefY = -abs(botLeftY - currTriangleLeftY);
				}
				else {
					translateRefX = -abs(currPosX - currTriangleLeftX);
					translateRefY = -abs(botLeftY - currTriangleRightY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirY *= -1;
				ascending = !ascending;
			}
			else if (centerTriangleX >= botLeftX + side) {
				angleToDest = 2 * pi - angleToDest;
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0;

				if (ascending) {
					translateRefX = abs(botLeftX + side - currTriangleLeftX);
					translateRefY = abs(currPosY - currTriangleRightY);
				}
				else {
					translateRefX = abs(botLeftX + side - currTriangleRightX);
					translateRefY = -abs(currPosY - currTriangleLeftY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirX *= -1;
				toTheRight = !toTheRight;
			}
			else if (centerTriangleY >= botLeftY + side) {
				angleToDest = pi - angleToDest;
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0;

				if (toTheRight) {
					translateRefX = abs(currPosX - currTriangleLeftX);
					translateRefY = abs(botLeftY + side - currTriangleRightY);
				}
				else {
					translateRefX = -abs(currPosX - currTriangleRightX);
					translateRefY = abs(botLeftY + side - currTriangleLeftY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirY *= -1;
				ascending = !ascending;
			}

			oldAngleToDest = angleToDest;
		}

		// meteorite scale
		else if (existMeteoriteScale && isColliding(meteoriteScaleX - meteoriteRadiusRot * scaleX, meteoriteScaleY - meteoriteRadiusRot * scaleY, 2 * meteoriteRadiusRot * scaleX, 2 * meteoriteRadiusRot * scaleY)) {
			existMeteoriteScale = false;
			reflectedPlatform = false;

			// we put the meteorite in a square for easy treatment
			float botLeftX = meteoriteScaleX - meteoriteRadiusRot * scaleX;
			float botLeftY = meteoriteScaleY - meteoriteRadiusRot * scaleY;
			float side = 2 * meteoriteRadiusRot * scaleX; // scaleX = scaleY

			oldAngleToDest = angleToDest;

			if (centerTriangleX <= botLeftX) {
				angleToDest = 2 * pi - angleToDest;
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0;

				if (ascending) {
					translateRefX = -abs(botLeftX - currTriangleRightX);
					translateRefY = abs(currPosY - currTriangleLeftY);
				}
				else {
					translateRefX = -abs(botLeftX - currTriangleLeftX);
					translateRefY = -abs(currPosY - currTriangleRightY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirX *= -1;
				toTheRight = !toTheRight;
			}
			else if (centerTriangleY <= botLeftY) {
				angleToDest = pi - angleToDest;
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0;

				if (toTheRight) {
					translateRefX = abs(currPosX - currTriangleRightX);
					translateRefY = -abs(botLeftY - currTriangleLeftY);
				}
				else {
					translateRefX = -abs(currPosX - currTriangleLeftX);
					translateRefY = -abs(botLeftY - currTriangleRightY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirY *= -1;
				ascending = !ascending;
			}
			else if (centerTriangleX >= botLeftX + side) {
				angleToDest = 2 * pi - angleToDest;
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0;

				if (ascending) {
					translateRefX = abs(botLeftX + side - currTriangleLeftX);
					translateRefY = abs(currPosY - currTriangleRightY);
				}
				else {
					translateRefX = abs(botLeftX + side - currTriangleRightX);
					translateRefY = -abs(currPosY - currTriangleLeftY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirX *= -1;
				toTheRight = !toTheRight;
			}
			else if (centerTriangleY >= botLeftY + side) {
				angleToDest = pi - angleToDest;
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0;

				if (toTheRight) {
					translateRefX = abs(currPosX - currTriangleLeftX);
					translateRefY = abs(botLeftY + side - currTriangleRightY);
				}
				else {
					translateRefX = -abs(currPosX - currTriangleRightX);
					translateRefY = abs(botLeftY + side - currTriangleLeftY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirY *= -1;
				ascending = !ascending;
			}

			oldAngleToDest = angleToDest;
		}

		// meteorite rotation
		else if (existMeteoriteRot && isColliding(cosf(angularStep) * (meteoriteRadiusRot)-sinf(angularStep) * (meteoriteRadiusRot)+meteoriteRotX - meteoriteRadiusRot, sinf(angularStep) * (meteoriteRadiusRot)+cosf(angularStep) * (meteoriteRadiusRot)+meteoriteRotY - meteoriteRadiusRot, 2 * meteoriteRadiusRot, 2 * meteoriteRadiusRot)) {
			existMeteoriteRot = false;
			reflectedPlatform = false;

			// we put the meteorite in a square for easy treatment
			float botLeftX = cosf(angularStep) * (meteoriteRadiusRot)-sinf(angularStep) * (meteoriteRadiusRot)+meteoriteRotX - meteoriteRadiusRot;
			float botLeftY = sinf(angularStep) * (meteoriteRadiusRot)+cosf(angularStep) * (meteoriteRadiusRot)+meteoriteRotY - meteoriteRadiusRot;
			float side = 2 * meteoriteRadiusRot;

			oldAngleToDest = angleToDest;

			if (centerTriangleX <= botLeftX) {
				angleToDest = 2 * pi - angleToDest;
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0;

				if (ascending) {
					translateRefX = -abs(botLeftX - currTriangleRightX);
					translateRefY = abs(currPosY - currTriangleLeftY);
				}
				else {
					translateRefX = -abs(botLeftX - currTriangleLeftX);
					translateRefY = -abs(currPosY - currTriangleRightY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirX *= -1;
				toTheRight = !toTheRight;
			}
			else if (centerTriangleY <= botLeftY) {
				angleToDest = pi - angleToDest;
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0;

				if (toTheRight) {
					translateRefX = abs(currPosX - currTriangleRightX);
					translateRefY = -abs(botLeftY - currTriangleLeftY);
				}
				else {
					translateRefX = -abs(currPosX - currTriangleLeftX);
					translateRefY = -abs(botLeftY - currTriangleRightY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirY *= -1;
				ascending = !ascending;
			}
			else if (centerTriangleX >= botLeftX + side) {
				angleToDest = 2 * pi - angleToDest;
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0;

				if (ascending) {
					translateRefX = abs(botLeftX + side - currTriangleLeftX);
					translateRefY = abs(currPosY - currTriangleRightY);
				}
				else {
					translateRefX = abs(botLeftX + side - currTriangleRightX);
					translateRefY = -abs(currPosY - currTriangleLeftY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirX *= -1;
				toTheRight = !toTheRight;
			}
			else if (centerTriangleY >= botLeftY + side) {
				angleToDest = pi - angleToDest;
				newCoordinatesForTrianglePoints(&centerTriangleX, &centerTriangleY, &currTriangleLeftX, &currTriangleLeftY, &currTriangleRightX, &currTriangleRightY);
				float translateRefX = 0, translateRefY = 0;

				if (toTheRight) {
					translateRefX = abs(currPosX - currTriangleLeftX);
					translateRefY = abs(botLeftY + side - currTriangleRightY);
				}
				else {
					translateRefX = -abs(currPosX - currTriangleRightX);
					translateRefY = abs(botLeftY + side - currTriangleLeftY);
				}

				currPosX += translateRefX;
				currPosY += translateRefY;
				centerTriangleX += translateRefX;
				centerTriangleY += translateRefY;
				currTriangleLeftX += translateRefX;
				currTriangleLeftY += translateRefY;
				currTriangleRightX += translateRefX;
				currTriangleRightY += translateRefY;
				changeDirY *= -1;
				ascending = !ascending;
			}

			oldAngleToDest = angleToDest;
		}

		else { // move normally without events, adding the necessary speed
			// peak of the triangle
			currPosX += changeDirX * deltaX * speed * deltaTime;
			currPosY += changeDirY * deltaY * speed * deltaTime;

			// center of triangle
			centerTriangleX += changeDirX * deltaX * speed * deltaTime;
			centerTriangleY += changeDirY * deltaY * speed * deltaTime;

			// triangle base points
			currTriangleLeftX += changeDirX * deltaX * speed * deltaTime;
			currTriangleLeftY += changeDirY * deltaY * speed * deltaTime;
			currTriangleRightX += changeDirX * deltaX * speed * deltaTime;
			currTriangleRightY += changeDirY * deltaY * speed * deltaTime;

			// if the astronaut has left platform after a permitted error distance that he is good to go
			if ((abs(beforeLaunchX - currPosX) > permittedError) || (abs(beforeLaunchY - currPosY) > permittedError))
			{
				launching = false;
			}
		}
	}
}

void Laborator3::OnKeyPress(int key, int mods)
{
	if (key == GLFW_KEY_SPACE) // reactivate the meteorites
	{
		existMeteoriteRot = true;
		existMeteoriteScale = true;
		existMeteoriteTrans = true;
	}
}

void Laborator3::OnKeyRelease(int key, int mods)
{
	// add key release event
}

void Laborator3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
	// add mouse move event
}

void Laborator3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
	// orientate the astronaut
	if (!finished && !move && IS_BIT_SET(button, GLFW_MOUSE_BUTTON_LEFT)) // if finished or while moving this is forbidden
	{
		clicked = true;
		destinationX = mouseX;
		destinationY = (window->GetResolution()).y - mouseY;
	}
}

void Laborator3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
	// add mouse button release event
}

void Laborator3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}

void Laborator3::OnWindowResize(int width, int height)
{
}
