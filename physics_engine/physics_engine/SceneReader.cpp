#define GLM_FORCE_CXX14
#include <iostream>

#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/matrix_interpolation.hpp>
#include <glm/vec3.hpp>

#include "AABB.h"
#include "Quad.h"
#include "SceneReader.h"
#include "Sphere.h"
#include "SphereCollisionShape.h"



SceneReader::SceneReader()
{
}


SceneReader::~SceneReader()
{
}

void SceneReader::readObjects(std::vector<Object*>& objList)
{
    for (pugi::xml_node objNode = mRoot.child("object"); objNode; objNode = objNode.next_sibling())
    {
        pugi::xml_node posNode = objNode.child("pos");
        pugi::xml_node velNode = objNode.child("vel");
        pugi::xml_node colorNode = objNode.child("color");

        double x = posNode.attribute("x").as_double();
        double y = posNode.attribute("y").as_double();
        double z = posNode.attribute("z").as_double();
        double rx = posNode.attribute("rx").as_double();
        double ry = posNode.attribute("ry").as_double();
        double rz = posNode.attribute("rz").as_double();

        double vx = velNode.attribute("vx").as_double();
        double vy = velNode.attribute("vy").as_double();
        double vz = velNode.attribute("vz").as_double();
        double ax = velNode.attribute("ax").as_double();
        double ay = velNode.attribute("ay").as_double();
        double az = velNode.attribute("az").as_double();

        double colR = colorNode.attribute("r").as_double();
        double colG = colorNode.attribute("g").as_double();
        double colB = colorNode.attribute("b").as_double();

        glm::dvec3 position(x, y, z);
        glm::dquat rotation(glm::dvec3(rx, ry, rz));
        glm::dvec3 velocity(vx, vy, vz);

        glm::dvec3 rotAxis;
        double rotMag;
        glm::axisAngle(glm::eulerAngleYXZ(ax, ay, az), rotAxis, rotMag);

        glm::dvec3 angularVelocity(rotMag * rotAxis);
        glm::dvec3 color(colR, colG, colB);

        float mass = objNode.attribute("mass").as_float(1.0);

        Object* obj = nullptr;

        std::string objType = objNode.attribute("type").as_string();
        if(objType == "sphere")
        {
            double radius = objNode.attribute("radius").as_double(1.0);
            obj = new Sphere(radius);
            obj->phys().setCollisionShape(new SphereCollisionShape(radius));

        }
        else if(objType == "aabb")
        {
            pugi::xml_node dimNode = objNode.child("dim");
            double wx = dimNode.attribute("wx").as_double(1.0);
            double wy = dimNode.attribute("wy").as_double(1.0);
            double wz = dimNode.attribute("wz").as_double(1.0);

            obj = new AABB(wx, wy, wz);
        }
        else if(objType == "quad")
        {
            pugi::xml_node dimNode = objNode.child("dim");
            double wx = dimNode.attribute("wx").as_double(1.0);
            double wz = dimNode.attribute("wz").as_double(1.0);

            obj = new Quad(wx, wz);
        }
        else
        {
            std::cerr << "Unsupported type: " << objType << std::endl;
            continue;
        }

        obj->phys().setMass(mass);
        obj->phys().setPose(position, rotation);
        obj->phys().setVel(velocity);
        obj->phys().setAngVel(angularVelocity);
        obj->rend().setColor(color);
        objList.push_back(obj);
    }
}

void SceneReader::readCamera(Camera& cam)
{
    pugi::xml_node camNode = mRoot.child("camera");
    pugi::xml_node camPosNode = camNode.child("pos");

    cam.mPos.x = camPosNode.attribute("x").as_float();
    cam.mPos.y = camPosNode.attribute("y").as_float();
    cam.mPos.z = camPosNode.attribute("z").as_float();

    cam.mFov = camNode.attribute("fov").as_double(45.0);
    cam.mNearPlane = camNode.attribute("near_plane").as_double(0.1);
    cam.mFarPlane = camNode.attribute("far_plane").as_double(30.0);
}

void SceneReader::readLight(PointLight& light)
{
    pugi::xml_node lNode = mRoot.child("light");
    pugi::xml_node posNode = lNode.child("pos");
    pugi::xml_node colNode = lNode.child("color");

    light.pos.x = posNode.attribute("x").as_float();
    light.pos.y = posNode.attribute("y").as_float();
    light.pos.z = posNode.attribute("z").as_float();

    light.color.x = colNode.attribute("r").as_float();
    light.color.y = colNode.attribute("g").as_float();
    light.color.z = colNode.attribute("b").as_float();

    light.ambient = lNode.attribute("ambient").as_float();
    light.attenuation = lNode.attribute("attenuation").as_float();
}

bool SceneReader::read(std::string filename)
{
    pugi::xml_parse_result result = mDoc.load_file(filename.c_str());
    
    if(result.status != pugi::status_ok)
        return false;

    mRoot = mDoc.child("scene");

    if(!mRoot)
        return false;

    return true;
}
