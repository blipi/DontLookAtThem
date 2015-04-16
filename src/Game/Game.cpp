
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
    _sceneDimensions(20, 20, 20),
    _cameraMoved(false),
    _cameraSpeed(0.0016f, 0.0016f, 0.0016f)
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
    _camera->toGPU(_program);
}

void Game::mainThread()
{
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
        _cameraMovement += glm::vec3(0, -1, 0);
        _cameraMoved = true;
    }

    updateCamera();
}

int Game::update()
{
    printf(".");
    handleInput();

    for (size_t i = 0; i < _floor.size(); ++i)
    {
        Pool::ThreadPool::get()->enqueue(&Cube::update, (Object*)_floor[i], nullptr);
    }

    return Updater::update();
}

void Game::updateCamera(bool onMain/* = false*/)
{
    if (_deltaX != 0 || _deltaY != 0 || _cameraMoved)
    {
        if (_deltaX != 0 || _deltaY != 0)
        {
            _camera->rotateCamera(_deltaX * 0.0016f, _deltaY * 0.0016f);
        }

        if (_cameraMoved)
        {
            _camera->moveCamera(_cameraSpeed, _cameraMovement);
        }

        if (!onMain)
        {
            Core::Scheduler<time_base>::get()->sync(&Game::mainThread, this);
        }
        else
        {
            mainThread();
        }
    }
}

void Game::draw(float interpolate)
{
    updateCamera();

    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (size_t i = 0; i < _floor.size(); ++i)
    {
        _floor[i]->draw(interpolate);
    }

    glfwSwapBuffers(**_window);
}
