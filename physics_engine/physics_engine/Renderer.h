#pragma once

#include <vector>

#include <GL\glew.h>
#include <GLFW\glfw3.h>

#include "Camera.h"
#include "Object.h"
#include "PointLight.h"

struct GLFWwindow;

/*
 *  An OpenGL-based renderer.
 */
class Renderer
{
public:
	Renderer(std::vector<Object*>& objList);
	~Renderer();

    /*!
     *  @brief  Sets the camera properties to those of the given camera.
     *  @param[in]  cam The camera whose properties will be copied.
     */
    void setCamera(Camera& cam);

    /*!
    *  @brief  Sets the point light properties to those of the given light.
    *  @param[in]  cam The light whose properties will be copied.
    */
    void setLight(PointLight& light);

    /*!
     *  @brief  Initializes the GLFW window and creates the OpenGL context.
     *          Also buffers the vertex data for the shapes in the scene.
     *  @return True if the initialization was successful, false otherwise.
     */
    bool init();

    /*!
     *  @brief  Renders all the objects in the scene.
     */
    void render();

    /*!
     *  @brief  Closes the GLFW window and cleans up the OpenGL context.
     */
    void close();

private:
    /*!
     *  @brief  Generates/buffers the vertex data for all the shapes in the scene.
     */
    void initObjects();

    /*!
     *  @brief  Reads the given vertex and fragment shaders, compiles them and creates a shader program.
     *  @param[in] vert_shader  Path to the vertex shader.
     *  @param[in] frag_shader  Path to the fragment shader.
     */
    void load_shader(const char* vert_shader, const char* frag_shader);

    /*!
    *  @brief  Reads the given shader and compiles it.
    *  @param[in] filename  Path to the shader file.
    *  @param[in] shader_type  The type of the shader, e.g. frag or vert.
    */
    GLuint read_shader(const char* filename, GLenum shader_type);

    /*!
     *  @brief  Renders the given object using the given projection matrix.
     *  @param[in]  obj The object to render.
     *  @param[in]  cam The projection matrix for the camera.
     */
    void render_obj(Object* obj, glm::mat4& cam);

private:
    std::vector<Object*>&   mObjList;   /// A reference to the list of objects in the scene.
    GLFWwindow*             mWindow;    /// The GLFW window to render to.
    GLuint                  mShader;    /// Handle to the shader for this scene.
    PointLight              mLight;     /// The point light used for this scene. For now this follows the camera's position.
    Camera                  mCamera;    /// The camera from which the scene is rendered.
};

