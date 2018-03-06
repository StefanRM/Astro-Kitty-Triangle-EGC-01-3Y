#pragma once

#include <Component/SimpleScene.h>
#include <string>
#include <Core/Engine.h>

class Laborator3 : public SimpleScene
{
	public:
		Laborator3();
		~Laborator3();

		void Init() override;

	private:
		void FrameStart() override;
		void Update(float deltaTimeSeconds) override;
		void FrameEnd() override;

		void OnInputUpdate(float deltaTime, int mods) override;
		void OnKeyPress(int key, int mods) override;
		void OnKeyRelease(int key, int mods) override;
		void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
		void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
		void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
		void OnWindowResize(int width, int height) override;
		
		// if the astronaut collides with a given platform
		bool isColliding(float platformX, float platformY, float width, float length);

		// new coordinates for the triangle when an event occurs
		void newCoordinatesForTrianglePoints(float* centerTriangleX, float* centerTriangleY, float* currTriangleLeftX, float* currTriangleLeftY, float* currTriangleRightX, float* currTriangleRightY);

	protected:
		glm::mat3 modelMatrix; // matrix used for Model Transformation
		float translateX, translateY; // translation distances
		float scaleX, scaleY; // scaling proportions
		float angularStep; // rotation angle
		int reverse = 1, reverse_scale = 1; // indicator for reverse translation and scaling
		float currPosX = 0, currPosY = 0; // current position
		float destinationX = 0, destinationY = 0, deltaX = 0, deltaY = 0, speed = 250; // destination coordinates, normalized distance vector coordinates
		float angleToDest_tg = 0, angleToDest = 0, pi = 3.141592653589793; // tangent of angle to be rotated to point to destination, that angle and the constant pi
		bool move = false, clicked = false; // indicator for movement and click event
		float platformLandingcoordX = 200, platformLandingcoordY = 500; // landing platform coordinates
		float rectangleWidth, rectangleLength, triangleSide, triangleHeight; // dimensions for rectangle and triangle
		bool platformLandingHit = false; // indicator for hitting the landing platform
		bool ascending = false, launching = false, toTheRight = false; // indicators for ascending/descending movement, if the triangle is launching and for left/right movement
		float centerTriangleX, centerTriangleY, currTriangleLeftX, currTriangleLeftY, currTriangleRightX, currTriangleRightY, oldAngleToDest, triangleCircleRad = 0; // coordinates of triangle's center and base points, previous angle and circle radius of the circumscribed circle
		float permittedError, beforeLaunchX, beforeLaunchY; // permitted error for not detecting collision at launch, and coordinates before launch
		float platformReflectioncoordX = 1050, platformReflectioncoordY = 150; // reflection platform coordinates
		short changeDirX = 1, changeDirY = 1; // direction indicators
		bool reflectedPlatform = false; // if the astronaut was reflected by the platform
		float meteoriteRotX = 800, meteoriteRotY = 400, meteoriteScaleX = 400, meteoriteScaleY = 100, meteoriteTransX = 150, meteoriteTransY = 350; // coordinates of meteorites
		float meteoriteRadiusRot = 50; // radius of meteorites
		bool existMeteoriteRot = true, existMeteoriteTrans = true, existMeteoriteScale = true; // if the meteorites were not hit
		float platformFinishcoordX = 650, platformFinishcoordY = 0; // finish platform coordinates
		bool finished = false; // platform finish hit indicator (also the end of the game)
};
