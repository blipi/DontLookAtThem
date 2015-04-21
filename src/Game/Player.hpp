#pragma once

#include "Cube.hpp"

class Player : public Cube
{
public:
	Player(glm::vec3 position, Shader::Program* program);

	int updateCPU(void* arg0) override;
	void jump();

	LFS_INLINE bool isInFloor();
	LFS_INLINE void centeredRotation(glm::vec3 rotation, GLfloat angle) override;
	LFS_INLINE void translate(glm::vec3 trans) override;

private:
	Object* _object;
	bool _inFloor;

	glm::vec3 _forces;
};

bool Player::isInFloor()
{
	return _inFloor;
}

void Player::centeredRotation(glm::vec3 rotation, GLfloat angle)
{
	Object::centeredRotation(rotation, angle);
	emit((Object*)this, &Player::rotated, -rotation, angle);
}

void Player::translate(glm::vec3 trans)
{
	Object::translate(trans);
	emit((Object*)this, &Player::moved);
}

