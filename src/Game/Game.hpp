#pragma once

#include <Updater.hpp>

class Window;
class Cube;

namespace Shader
{
	class Program;
}

class Game : public Core::Updater
{
public:
	Game(Window* window);
	virtual ~Game();

	virtual int update() override;

	virtual void initializeGL();
	virtual void onResize(int width, int height);
	virtual void onMouseMove(double x, double y, uint8_t mouse);
	virtual void draw(float interpolate);

private:
	Window* _window;
	Shader::Program* _program;

	std::vector<Cube*> _floor;

	double _xRot;
	double _yRot;
	double _zRot;
	
	double _lastX;
	double _lastY;

	glm::vec3 _sceneDimensions;
};
