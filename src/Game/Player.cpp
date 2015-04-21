#include "Player.hpp"
#include "Program.hpp"
#include "Constants.hpp"
#include "Scheduler.hpp"

Player::Player(glm::vec3 position, Shader::Program* program) :
	Cube(0, program),
	_forces(0, 0, 0),
	_inFloor(true)
{

}

int Player::updateCPU(void* arg0)
{
	if (_position.y > 0 || _forces.y > 0)
	{
		_inFloor = _position.y > 0 ? false : true;

		if (_position.y + _forces.y < 0)
		{
			_forces.y = -_position.y;
			_inFloor = true;
		}
		
		translate(_forces);

		emit((Object*)this, &Player::moved);
	}

	_forces.y -= GRAVITY * Core::Scheduler<time_base>::get()->dt();

	return Cube::updateCPU(arg0);
}

void Player::jump()
{
	_forces.y = 1.3f / 10.0f;
}

