#pragma once

#include <glm/vec3.hpp>

struct CollisionData
{ 
	double		penetration;	//!< penetration depth
	glm::dvec3	normal;			//!< contact normal
	glm::dvec3	point;			//!< point where collision detected
};

