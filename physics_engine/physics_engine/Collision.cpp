#include "Collision.h"
#include "SphereCollisionShape.h"

bool Collision::areObjectsColliding(const PhysicsObject& obj0, const PhysicsObject& obj1, CollisionData* colDataOut)
{
    const CollisionShape* cs1 = obj0.collisionShape();
    const CollisionShape* cs2 = obj1.collisionShape();

    if(!cs1 || !cs2)
        return false;

    ObjectType t0 = cs1->type();
    ObjectType t1 = cs2->type();

    if(t0 == eSPHERE && t1 == eSPHERE)
    {
        return sphereSphereCollision(obj0, obj1, colDataOut);
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
