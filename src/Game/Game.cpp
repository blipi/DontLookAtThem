
#include "Game.hpp"

#include "Cube.hpp"
#include "Object.hpp"
#include "Camera.hpp"

#include <Program.hpp>
#include <Resources.hpp>
#include <Window.hpp>
#include <Scheduler.hpp>


Game::Game(Window* window) :
Updater(),
	_window(window),
	_xRot(0), _yRot(0), _zRot(0),
	_lastX(0), _lastY(0),
	_sceneDimensions(20, 20, 20)
{
	_window->setMouseFixed();
	_program = new Shader::Program();
	_camera = new Camera(_sceneDimensions);

	for (int i = 0; i < 20; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			Cube* cube = new Cube(0, _program);
			cube->initialize();
			cube->translate(glm::vec3(-9.50 + i, 0, -9.50 + j));

			_floor.push_back(cube);
		}
	}
}

Game::~Game()
{
	for (auto* cube : _floor)
	{
		delete cube;
	}
	delete _camera;
	delete _program;
}

void Game::initializeGL()
{
	/* Enable some gl constants */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	/* Create shaders */
	_program->attach(GL_VERTEX_SHADER, Resources::getPath("shaders", "vertex.glsl"));
	_program->attach(GL_FRAGMENT_SHADER, Resources::getPath("shaders", "fragment.glsl"));

	/* Bind varying locations to fixed index */
	_program->bindAttribute("vPosition", 0);
	_program->bindAttribute("vColor", 1);

	/* Link program and bind */
	_program->link();
	_program->bind();

	/* Create Model matrix for MVP */
	glUniformMatrix4fv(_program->uniformLocation("MVP"), 1, GL_FALSE, &_camera->getMVP()[0][0]);
	printf("Error: %u\n", glGetError());

	/* Clear color */
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Update (UNIX won't render unless called, Windows does) */
	_window->update();
}

void Game::onResize(int width, int height)
{
	int side = std::min(width, height);
	glViewport((width - side) / 2, (height - side) / 2, side, side);

	_camera->setAspect(width > height ? float(width) / float(height) : float(height) / float(width));
	glUniformMatrix4fv(_program->uniformLocation("MVP"), 1, GL_FALSE, &_camera->getMVP()[0][0]);
}

void Game::onMouseMove(double x, double y, uint8_t mouse)
{
	double dx = x - _window->getWidth() / 2.0f;
	double dy = y - _window->getHeight() / 2.0f;
	
	if (mouse & Mouse::LeftButton) {
		_xRot += dy;
	}
	else if (mouse & Mouse::RightButton) {
		_xRot += dy;
		_zRot += dx;
	}

	while (_xRot > 360)
		_xRot -= 180;
	while (_zRot > 360)
		_zRot -= 180;

	if (mouse)
	{
		//((Cube*)_cube)->rotate({ 1.0f, 0.0f, 1.0f }, _xRot / 180.0f);
	}
}

int Game::update()
{
	printf(".");

	for (size_t i = 0; i < _floor.size(); ++i)
	{
		Pool::ThreadPool::get()->enqueue(&Cube::update, (Object*)_floor[i], nullptr);
	}

	return Updater::update();
}

void Game::draw(float interpolate)
{
	/* Render here */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (size_t i = 0; i < _floor.size(); ++i)
	{
		_floor[i]->draw(interpolate);
	}

	glfwSwapBuffers(**_window);
}
