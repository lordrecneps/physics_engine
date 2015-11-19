#define GLM_FORCE_CXX14
#include <iostream>

#include <glm/vec3.hpp>
#include <pugixml.hpp>

#include "AABB.h"
#include "SceneReader.h"
#include "Sphere.h"



SceneReader::SceneReader()
{
}


SceneReader::~SceneReader()
{
}

void SceneReader::read(std::string filename, std::vector<Object*>& objList)
{
    pugi::xml_document doc;
    doc.load_file(filename.c_str());

    pugi::xml_node rootNode = doc.child("scene");

    for (pugi::xml_node objNode = rootNode.child("object"); objNode; objNode = objNode.next_sibling())
    {
        pugi::xml_node posNode = objNode.child("pos");
        
        double x = posNode.attribute("x").as_double();
        double y = posNode.attribute("y").as_double();
        double z = posNode.attribute("z").as_double();
        double rx = posNode.attribute("rx").as_double();
        double ry = posNode.attribute("ry").as_double();
        double rz = posNode.attribute("rz").as_double();

        glm::vec3 position(x, y, z);
        glm::vec3 rotation(rx, ry, rz);

        double mass = objNode.attribute("mass").as_double(1.0);

        std::string objType = objNode.attribute("type").as_string();
        if (objType == "sphere")
        {
            double radius = objNode.attribute("radius").as_double(1.0);
            objList.push_back(new Sphere(radius));
        }
        else if (objType == "aabb")
        {
            pugi::xml_node dimNode = objNode.child("dim");
            double wx = dimNode.attribute("wx").as_double(1.0);
            double wy = dimNode.attribute("wy").as_double(1.0);
            double wz = dimNode.attribute("wz").as_double(1.0);

            objList.push_back(new AABB(wx, wy, wz));
        }
        else
        {
            std::cerr << "Unsupported type: " << objType << std::endl;
            continue;
        }

        objList.back()->setMass(mass);
        objList.back()->setPose(position, rotation);
    }
}
