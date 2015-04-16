#include "Object.hpp"

Object::Object(const uint32_t id) :
    _id(id),
    _points(nullptr),
    _colors(nullptr)
{
}

Object::~Object()
{}
