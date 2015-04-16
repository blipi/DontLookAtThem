#include "Cube.hpp"
#include "Program.hpp"


Cube::Cube(const uint32_t id, Shader::Program* program) :
    Object(id),
    _program(program)
{
    // 36 _points at all

    // Vertices of a unit cube centered at origin, sides aligned with axes
    vertices[0] = point4(-0.5, -0.5, 0.5, 1.0);
    vertices[1] = point4(-0.5, 0.5, 0.5, 1.0);
    vertices[2] = point4(0.5, 0.5, 0.5, 1.0);
    vertices[3] = point4(0.5, -0.5, 0.5, 1.0);
    vertices[4] = point4(-0.5, -0.5, -0.5, 1.0);
    vertices[5] = point4(-0.5, 0.5, -0.5, 1.0);
    vertices[6] = point4(0.5, 0.5, -0.5, 1.0);
    vertices[7] = point4(0.5, -0.5, -0.5, 1.0);

    // RGBA colors
    vertex_colors[0] = color4(0.0, 0.0, 1.0, 1.0);  // cyan
    vertex_colors[1] = color4(1.0, 1.0, 1.0, 1.0);  // cyan
    vertex_colors[2] = color4(1.0, 1.0, 1.0, 1.0);  // cyan
    vertex_colors[3] = color4(1.0, 1.0, 0.0, 1.0);  // cyan
    vertex_colors[4] = color4(0.0, 1.0, 0.0, 1.0);  // cyan
    vertex_colors[5] = color4(0.0, 1.0, 1.0, 1.0);  // cyan
    vertex_colors[6] = color4(0.0, 0.0, 0.0, 1.0);  // cyan
    vertex_colors[7] = color4(0.0, 0.0, 0.5, 1.0);  // cyan
}

void Cube::quad(int a, int b, int c, int d)
{
    _colors[Index] = vertex_colors[a]; _points[Index] = vertices[a]; Index++;
    _colors[Index] = vertex_colors[b]; _points[Index] = vertices[b]; Index++;
    _colors[Index] = vertex_colors[c]; _points[Index] = vertices[c]; Index++;
    _colors[Index] = vertex_colors[a]; _points[Index] = vertices[a]; Index++;
    _colors[Index] = vertex_colors[c]; _points[Index] = vertices[c]; Index++;
    _colors[Index] = vertex_colors[d]; _points[Index] = vertices[d]; Index++;
}

void Cube::initialize()
{
    _points = new point4[numVertices];
    _colors = new point4[numVertices];

    Index = 0;
    quad(1, 0, 3, 2);
    quad(2, 3, 7, 6);
    quad(3, 0, 4, 7);
    quad(6, 5, 1, 2);
    quad(4, 5, 6, 7);
    quad(5, 4, 0, 1);

    /*
    glm::mat4 scale = glm::scale(glm::mat4(), glm::vec3(2 / sceneDimensions[0], 2 / sceneDimensions[1], 2 / sceneDimensions[2]));
    for (int i = 0; i < numVertices; ++i)
    {
        _points[i] = scale * _points[i];
    }*/

    glGenBuffers(1, &_buffers);
    glBindBuffer(GL_ARRAY_BUFFER, _buffers);
    glBufferData(GL_ARRAY_BUFFER, sizeof(point4) * numVertices + sizeof(point4) * numVertices, NULL, GL_STATIC_DRAW);

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point4) * numVertices, &_points[0]);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(point4) * numVertices, sizeof(point4) * numVertices, &_colors[0]);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::draw(float interpolate)
{
    if (interpolate > 0)
    {
        updateCPU();
        updateGPU();
    }

    _program->bind();
    glBindBuffer(GL_ARRAY_BUFFER, _buffers);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(_program->attributeLocation("vPosition"), 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(_program->attributeLocation("vColor"), 4, GL_FLOAT, GL_FALSE, 0, (void *)(sizeof(point4) * numVertices));

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); //GL_LINE
    glDrawArrays(GL_TRIANGLES, 0, numVertices);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Cube::updateCPU()
{
    for (int i = 0; i < numVertices; ++i)
    {
        _points[i] = _transformations * _points[i];
    }

    _transformations = glm::mat4();
}

void Cube::updateGPU()
{
    glBindBuffer(GL_ARRAY_BUFFER, _buffers);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(point4) * numVertices, &_points[0]);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}
