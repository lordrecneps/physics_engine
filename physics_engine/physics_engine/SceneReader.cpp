#include <pugixml.hpp>

#include "SceneReader.h"



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
}
