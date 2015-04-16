#pragma once

#include "core.hpp"
#include "Object.hpp"

namespace Shader
{
    class Program;
}

class Cube : public Object
{
public:
    Cube(const uint32_t id, Shader::Program* program);

    void quad(int a, int b, int c, int d);
    void initialize() override;
    void draw(float interpolate) override;

    void updateCPU();
    void updateGPU() override;

private:
    const int numVertices = 36;

    GLuint _buffers;
    Shader::Program* _program;

    point4 vertices[8]; // 8 vertexs del cub
    color4 vertex_colors[8]; // 8 colors RGBA associats a cada vertex

    int Index;
};
