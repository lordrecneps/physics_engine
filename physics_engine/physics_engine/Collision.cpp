#include <algorithm>
#include <cmath>
#include <iostream>

#include <glm/gtx/string_cast.hpp>

#include "Collision.h"
#include "PlaneCollisionShape.h"
#include "SphereCollisionShape.h"

bool Collision::areObjectsColliding(const PhysicsObject& obj0, const PhysicsObject& obj1, CollisionData* colDataOut)
{
    const CollisionShape* cs1 = obj0.collisionShape();
    const CollisionShape* cs2 = obj1.collisionShape();

    const PhysicsObject* po1 = &obj0;
    const PhysicsObject* po2 = &obj1;

    if(!cs1 || !cs2)
        return false;

    ObjectType t1 = cs1->type();
    ObjectType t2 = cs2->type();

    if(t1 > t2)
    {
        std::swap(po1, po2);
        std::swap(t1, t2);
    }

    if(t1 == eSPHERE && t2 == eSPHERE)
    {
        return sphereSphereCollision(obj0, obj1, colDataOut);
    }

    if(t1 == eSPHERE && t2 == eQUAD)
    {
        return spherePlaneCollision(*po1, *po2, colDataOut);
    }
    return false;
}

bool Collision::sphereSphereCollision(const PhysicsObject& obj0, const PhysicsObject& obj1, CollisionData* colDataOut)
{
    const SphereCollisionShape* sphere0 = static_cast<const SphereCollisionShape*>(obj0.collisionShape());
    const SphereCollisionShape* sphere1 = static_cast<const SphereCollisionShape*>(obj1.collisionShape());

    double radiusSum = sphere0->radius() + sphere1->radius();
    glm::dvec3 ab = obj0.pos() - obj1.pos();
    double distSquared = glm::dot(ab, ab);

    if(distSquared <= radiusSum*radiusSum)
    {
        if(colDataOut)
        {
            colDataOut->penetration = radiusSum - glm::sqrt(distSquared);
            colDataOut->normal = glm::normalize(ab);
            colDataOut->point = obj0.pos() - colDataOut->normal*(sphere0->radius() - colDataOut->penetration);
        }
        return true;
    }
    return false;
}

bool Collision::spherePlaneCollision(const PhysicsObject& obj0, const PhysicsObject& obj1, CollisionData* colDataOut)
{
    const auto sphere0 = static_cast<const SphereCollisionShape*>(obj0.collisionShape());
    const auto plane1  = static_cast<const PlaneCollisionShape*>(obj1.collisionShape());

    glm::dmat3 rotMat = glm::mat3_cast(obj1.rot());
    auto x_axis = rotMat * glm::dvec3(1, 0, 0);
    auto y_axis = rotMat * glm::dvec3(0, 1, 0);
    auto n = rotMat * glm::dvec3(0, 0, 1);

    auto r = sphere0->radius();

    auto ab = obj0.pos() - obj1.pos();
    auto n_dist = glm::dot(ab, n);
    auto n_dist_abs = fabs(n_dist);

    if(n_dist_abs >= r)
        return false;
    //std::cout << "ab: " << glm::to_string(ab) << ", n: " << glm::to_string(n) << ", dist: " << n_dist << std::endl;
    //std::cout << glm::to_string(rotMat) << std::endl;
    //exit(0);
    auto sin_theta = n_dist / r;
    auto r_cos_sq = r*r * (1.0 - pow(sin_theta, 2));

    auto dist_x = glm::dot(ab, x_axis);

    auto dim = plane1->getDim();

    if(pow(dist_x, 2) >= (pow(dim.x, 2) + r_cos_sq))
        return false;

    auto dist_y = glm::dot(ab, y_axis);

    if(pow(dist_y, 2) < (pow(dim.y, 2) + r_cos_sq))
    {
        if(colDataOut)
        {
            auto dist_x_abs = fabs(dist_x);
            auto dist_y_abs = fabs(dist_y);
            if(dist_x_abs < dim.x && dist_y_abs < dim.y)
            {
                colDataOut->penetration = r - n_dist_abs;
                colDataOut->normal = n_dist >= 0.0 ? n : -n;
                colDataOut->point = obj0.pos() - n_dist * n;
            }
            else if(dist_x_abs >= dim.x && dist_y_abs < dim.y) // edge case parallel to y-axis
            {
                auto r_cos = sqrt(r_cos_sq);
                colDataOut->penetration = dim.x + r_cos - dist_x_abs;
                colDataOut->normal = r_cos * x_axis + n_dist * n;
                colDataOut->point = obj0.pos() - colDataOut->normal;

                colDataOut->normal = glm::normalize(colDataOut->normal);
            }
            else if(dist_x_abs < dim.x && dist_y_abs >= dim.y)  // edge case parallel to x-axis
            {
                auto r_cos = sqrt(r_cos_sq);
                colDataOut->penetration = dim.y + r_cos - dist_y_abs;
                colDataOut->normal = r_cos * y_axis + n_dist * n;
                colDataOut->point = obj0.pos() - colDataOut->normal;

                colDataOut->normal = glm::normalize(colDataOut->normal);
            }
            else // corner case
            {
                auto x = dim.x * x_axis;
                auto y = dim.y * y_axis;

                if(dist_x < 0.0)
                    x *= -1.0;
                if(dist_y < 0.0)
                    y *= -1.0;

                colDataOut->point = obj1.pos() + x + y;
                auto ab = obj0.pos() - colDataOut->point;
                auto dist_to_corner = glm::length(ab);

                colDataOut->normal = ab / dist_to_corner;
                colDataOut->penetration = r - dist_to_corner;

            }
        }
        return true;
    }

    return false;
}
