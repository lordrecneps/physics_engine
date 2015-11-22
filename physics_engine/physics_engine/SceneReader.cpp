#define GLM_FORCE_CXX14
#include <iostream>

#include <glm/vec3.hpp>

#include "AABB.h"
#include "SceneReader.h"
#include "Sphere.h"



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

        glm::vec3 position(x, y, z);
        glm::quat rotation(glm::vec3(rx, ry, rz));
        glm::vec3 velocity(vx, vy, vz);
        glm::vec3 angularVelocity(ax, ay, az);
        glm::vec3 color(colR, colG, colB);

        double mass = objNode.attribute("mass").as_double(1.0);

        Object* obj = nullptr;

        std::string objType = objNode.attribute("type").as_string();
        if (objType == "sphere")
        {
            double radius = objNode.attribute("radius").as_double(1.0);
            obj = new Sphere(radius);

        }
        else if (objType == "aabb")
        {
            pugi::xml_node dimNode = objNode.child("dim");
            double wx = dimNode.attribute("wx").as_double(1.0);
            double wy = dimNode.attribute("wy").as_double(1.0);
            double wz = dimNode.attribute("wz").as_double(1.0);

            obj = new AABB(wx, wy, wz);
        }
        else
        {
            std::cerr << "Unsupported type: " << objType << std::endl;
            continue;
        }

        obj->phys().setMass(mass);
        obj->phys().setPose(position, rotation);
        obj->rend().setColor(color);
        objList.push_back(obj);
    }
}

void SceneReader::readCamera(Camera & cam)
{
    pugi::xml_node camNode = mRoot.child("camera");
    pugi::xml_node camPosNode = camNode.child("pos");

    cam.mPos.x = camPosNode.attribute("x").as_float();
    cam.mPos.y = camPosNode.attribute("y").as_float();
    cam.mPos.z = camPosNode.attribute("z").as_float();
}

void SceneReader::read(std::string filename)
{
    mDoc.load_file(filename.c_str());
    mRoot = mDoc.child("scene");
}
