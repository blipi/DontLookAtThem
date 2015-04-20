
#include "Game.hpp"

#include "Cube.hpp"
#include "Object.hpp"
#include "Camera.hpp"

#include <Program.hpp>
#include <Resources.hpp>
#include <Window.hpp>
#include <Scheduler.hpp>
#include <Texture.hpp>


Game::Game(Window* window) :
Updater(),
    _window(window),
    _sceneDimensions(20, 20, 20),
    _deltaX(0),
    _deltaY(0),
    _cameraMoved(false),
	_baseSpeed(2 / 10.0f * Scheduler::get()->dt(), 2 / 10.0f * Scheduler::get()->dt(), 2 / 10.0f * Scheduler::get()->dt()),
	_cameraSpeed(_baseSpeed)
{
    _window->setMouseFixed();
    _program = new Shader::Program();
    _camera = new Camera(_sceneDimensions);

	Shader::Texture* texture = new Shader::Texture(Resources::getPath("terrain", "grass_green_d.DDS").c_str());

    for (int i = 0; i < 20; ++i)
    {
        for (int j = 0; j < 20; ++j)
        {
            Cube* cube = new Cube(0, _program);
            cube->initialize((void*)texture);
            cube->translate(glm::vec3(-9.50 + i, 0, -9.50 + j));

            _floor.push_back(cube);
        }
    }
    
    uint8_t b;
    double x, y;
    _window->getMouseDelta(b, x, y);
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
	_program->bindAttribute("vTexCoords", 2);

    /* Link program and bind */
    _program->link();
    _program->bind();

    /* Create Model matrix for MVP */
    glUniformMatrix4fv(_program->uniformLocation("MVP"), 1, GL_FALSE, &_camera->getMVP()[0][0]);
	glUniform1i(_program->uniformLocation("MVP"), 0);

    /* Clear color */
    glClearColor(0, 0, 0, 1);
}

void Game::onResize(int width, int height)
{
    int side = std::min(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    _camera->setAspect(width > height ? float(width) / float(height) : float(height) / float(width));
    _camera->toGPU(_program);
}

void Game::handleInput()
{
    uint8_t buttons;

    _window->getMouseDelta(buttons, _deltaX, _deltaY);
    
    if (buttons & Mouse::LeftButton) {
        
    }
    else if (buttons & Mouse::RightButton) {
        
    }

    // L
    if (_window->isKeyPressed(Keys::L))
    {
        _camera->setLocked(!_camera->getLocked());
    }

    //WASD
    _cameraMoved = false;

    if (_window->isKeyPressed(Keys::W))
    {
        _cameraMovement = glm::vec3(1, 0, 0);
        _cameraMoved = true;
    }
    else if (_window->isKeyPressed(Keys::S))
    {
        _cameraMovement = glm::vec3(-1, 0, 0);
        _cameraMoved = true;
    }
    else
    {
        _cameraMovement = glm::vec3(0, 0, 0);
    }
    
    if (_window->isKeyPressed(Keys::A))
    {
        _cameraMovement += glm::vec3(0, 0, -1);
        _cameraMoved = true;
    }
    else if (_window->isKeyPressed(Keys::D))
    {
        _cameraMovement += glm::vec3(0, 0, 1);
        _cameraMoved = true;
    }

    if (_window->isKeyPressed(Keys::SPACE))
    {
        _cameraMovement += glm::vec3(0, 1, 0);
        _cameraMoved = true;
    }
    else if (_window->isKeyPressed(Keys::LEFT_SHIFT))
    {
		if (_camera->getLocked())
		{
			_cameraSpeed = _baseSpeed * 2.5f;
		}
		else
		{
			_cameraMovement += glm::vec3(0, -1, 0);
		}
		_cameraMoved = true;
    }

	if (!_window->isKeyPressed(Keys::LEFT_SHIFT) || !_camera->getLocked())
	{
		_cameraSpeed = _baseSpeed;
	}
}

int Game::updateCPU(void* arg0)
{
    handleInput();
	updateCamera();
	
    return Updater::updateCPU(arg0);
}

int Game::updateGPU()
{
	_camera->toGPU(_program);

	return Updater::updateGPU();
}

void Game::updateCamera(bool interpolate/* = false*/, float interValue/* = 1.0f*/)
{
	_camera->setInterpolation(interpolate);
	
    if (_deltaX != 0 || _deltaY != 0 || _cameraMoved)
    {
        if (_deltaX != 0 || _deltaY != 0)
        {
			_camera->rotateCamera(_deltaX * 0.0016f * (interpolate ? interValue : 1.0f), _deltaY * 0.0016f * (interpolate ? interValue : 1.0f));
        }

        if (_cameraMoved)
        {
			_camera->moveCamera(_cameraSpeed * (interpolate ? interValue : 1.0f), _cameraMovement);
        }
    }
}

void Game::draw(float interpolate)
{
	updateCamera(true, interpolate);
	_camera->toGPU(_program);

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (size_t i = 0; i < _floor.size(); ++i)
    {
        _floor[i]->draw(interpolate);
    }

    glfwSwapBuffers(**_window);
}
