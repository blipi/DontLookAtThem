#pragma once

#include <Updater.hpp>

class Window;
class Cube;
class Camera;

namespace Shader
{
    class Program;
}

class Game : public Core::Updater
{
public:
    Game(Window* window);
    virtual ~Game();

    void mainThread();
    virtual int update() override;
    void updateCamera(bool onMain = false);
    virtual void handleInput();

    virtual void initializeGL();
    virtual void onResize(int width, int height);
    virtual void draw(float interpolate);

private:
    Window* _window;
    Shader::Program* _program;

    Camera* _camera;
    std::vector<Cube*> _floor;
    
    bool _cameraMoved;
    double _deltaX;
    double _deltaY;
    glm::vec3 _cameraSpeed;
    glm::vec3 _cameraMovement;

    glm::vec3 _sceneDimensions;
};
