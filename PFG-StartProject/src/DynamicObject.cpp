#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include "DynamicObject.h"
#include "Utility.h"

DynamicObject::DynamicObject()
{
	_force = glm::vec3(0.0f, 0.0f, 0.0f);
	_velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	_mass = 1.0f;
	_bRadius = 0.0f;

	_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	_start = false;
}

DynamicObject::~DynamicObject()
{

}

void DynamicObject::Update(float deltaTs, int updateType)
{
	float collisionImpulse;
	glm::vec3 impulseForce;
	glm::vec3 contactForce(0.0f, 9.8f * _mass * 0.1f, 0.0f);

	if (_start == true)
	{
		ClearForces();

		glm::vec3 gravityForce(0.0f, -9.8 * _mass * 0.1f, 0.0f);
		AddForce(gravityForce);

		float elasticity = 0.7f;
		float r = GetBoundingRadius();

		glm::vec3 pointOnFloor(0.0f, 0.0f, 0.0f);
		glm::vec3 floorNormal(0.0f, 1.0f, 0.0f);

		float d = PFG::DistanceToPlane(floorNormal, _position, pointOnFloor);

		if (d <= r)
		{
			_position.y = r;
			//glm::vec3 bounceForce = glm::vec3(0.0f, 150.0f, 0.0f);
			collisionImpulse = (-(1 + elasticity) * glm::dot(_velocity, floorNormal)) * _mass;
			impulseForce = (collisionImpulse * floorNormal);// / deltaTs;
			//AddForce(impulseForce);
			_velocity += impulseForce / _mass;
			AddForce(contactForce);
		}

		switch (updateType)
		{
		case 1:
			Euler(deltaTs);
			break;
		case 2:
			RungeKutta2(deltaTs);
			break;
		case 3:
			RungeKutta4(deltaTs);
			break;
		case 4:
			Verlet(deltaTs);
			break;
		default:
			Euler(deltaTs);
			break;
		}
	}

	UpdateModelMatrix();
}

void DynamicObject::Euler(float deltaTs)
{
	float oneOverMass = 1 / _mass;
	_velocity += (_force * oneOverMass * deltaTs);
	_position += _velocity * deltaTs;
}

void DynamicObject::RungeKutta2(float deltaTs)
{
	glm::vec3 force;
	glm::vec3 acceleration;
	glm::vec3 k0;
	glm::vec3 k1;

	//Evaluating once at t0
	force = _force;
	acceleration = force / _mass;
	k0 = acceleration * deltaTs;

	force = _force + k0 / 2.0f;
	acceleration = force / _mass;
	k1 = acceleration * deltaTs;

	_velocity += k1;
	_position += _velocity * deltaTs;
}

void DynamicObject::RungeKutta4(float deltaTs)
{
	glm::vec3 force;
	glm::vec3 acceleration;
	glm::vec3 k0;
	glm::vec3 k1;
	glm::vec3 k2;
	glm::vec3 k3;

	//Evaluating once at t0
	force = _force;
	acceleration = force / _mass;
	k0 = acceleration * deltaTs;

	force = _force + k0 / 2.0f;
	acceleration = force / _mass;
	k1 = acceleration * deltaTs;

	force = _force + k1 / 2.0f;
	acceleration = force / _mass;
	k2 = acceleration * deltaTs;

	force = _force + k2;
	acceleration = force / _mass;
	k3 = acceleration * deltaTs;

	_velocity += (k0 + 2.0f * k1 + 2.0f * k2 + k3) / 6.0f;
	_position += _velocity * deltaTs;
}

void DynamicObject::Verlet(float deltaTs)
{
	glm::vec3 force;
	glm::vec3 acceleration;
	glm::vec3 previousPos;
	
	force = _force;
	acceleration = force / _mass;

	previousPos = _position - _velocity * deltaTs + 0.5f * acceleration * pow(deltaTs, 2);
	_position = -previousPos + 2.0f * _position + acceleration * pow(deltaTs, 2);

	_velocity = (_position - previousPos) / (2.0f * deltaTs);

	_velocity += acceleration * deltaTs;
}

void DynamicObject::UpdateModelMatrix()
{
	_modelMatrix = glm::translate(glm::mat4(1), _position); 
	_modelMatrix = glm::scale(_modelMatrix, _scale);
	_invModelMatrix = glm::inverse(_modelMatrix);
}