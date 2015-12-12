#pragma once

#include <glm/vec3.hpp>

struct CollisionData
{ 
	float		penetration;	//!< penetration depth
	glm::vec3	normal;			//!< contact normal
	glm::vec3	point;			//!< point where collision detected
};

