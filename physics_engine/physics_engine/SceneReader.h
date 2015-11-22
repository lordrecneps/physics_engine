#pragma once

#include <vector>

#include <pugixml.hpp>

#include "Camera.h"
#include "Object.h"
#include "PointLight.h"

/*!
 *  @brief  A reader for the custom scene file format used by this physics engine.
 */
class SceneReader
{
public:
    SceneReader();
    ~SceneReader();

    /*
     *  @brief  Parses the given scene file as an xml document.
     *  @return True if the given document is a valid xml file and contains a <scene> root node.
     */
    bool read(std::string filename);

    /*!
     *  @brief  Reads all the objects specified in the scene file and instantiates them.
     *  @param[in]  objList The list in which to store the created objects.
     */
    void readObjects(std::vector<Object*>& objList);

    /*!
    *  @brief  Reads the camera properties specified in the scene file into the given camera.
    *  @param[in]  cam The camera to initialize.
    */
    void readCamera(Camera& cam);

    /*!
    *  @brief  Reads all the lights specified in the scene file and instantiates them.
    *  @param[in]  light The light to initialize.
    */
    void readLight(PointLight& light);

private:
    pugi::xml_document      mDoc;   /// The xml document describing the scene.
    pugi::xml_node          mRoot;  /// The root node of the scene xml document.
};

