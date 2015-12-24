#pragma once

#include <vector>

#include <GL\glew.h>

#include "AABB.h"
#include "Camera.h"
#include "GBuffer.h"
#include "Object.h"
#include "PointLight.h"
#include "Quad.h"
#include "ShadowMap.h"

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
	 *	@brief	The list of shaders supported by the renderer.
	 */
    enum eShaders
    {
        eFORWARD_RENDER,
        eDEFFERED_GEOM_PASS,
        eDEFERRED_LIGHT_PASS,
        eSHADOW_PASS,
        eNUMSHADERS
    };

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
    bool render();



private:
    /*!
     *  @brief  Generates/buffers the vertex data for all the shapes in the scene.
     */
    void initObjects();

    /*!
     *  @brief  Reads the given vertex and fragment shaders, compiles them and creates a shader program.
     *  @param[in] vert_shader  Path to the vertex shader.
     *  @param[in] frag_shader  Path to the fragment shader.
     *  @return     True if successful. False if any errors occured.
     */
    bool loadShader(uint32_t shader, const char* vert_shader, const char* frag_shader);

    /*!
    *  @brief  Reads the given shader and compiles it.
    *  @param[in] filename  Path to the shader file.
    *  @param[in] shader_type  The type of the shader, e.g. frag or vert.
    */
    GLuint readShader(const char* filename, GLenum shader_type);

    /*!
     *  @brief  Renders the given object using the given projection matrix.
     *  @param[in]  obj The object to render.
     *  @param[in]  cam The projection matrix for the camera.
     */
    void renderObj(Object* obj, const glm::mat4& cam);

	/*!
	 *	@brief	Initializes the GL vertex buffer for a box.
	 *	@param[in]	obj	The box to initialize.
	 */
    void initBox(Object* obj);

    void initQuad(Object* obj);

    void initSphere(Object* obj);

	void shadowPass();

	/*!
	 *	@brief	Performs the geometry pass for deferred rendering, i.e. draw the scene geometry into
	 *			textures for the light pass.
	 *	@param[in]	cam	The camera view matrix.
	 */
    void geometryPass(const glm::mat4& cam);

	/*!
	 *	@brief	Performs the lighting pass for deferred rendering.
	 */
    void lightPass();

private:
    std::vector<Object*>&   mObjList;               /// A reference to the list of objects in the scene.
    
    GLuint                  mShaders[eNUMSHADERS];  /// Handles to the shaders for this scene.
    uint32_t                mCurrShader;            /// The active shader.
    PointLight              mLight;                 /// The point light used for this scene. For now this follows the camera's position.
    Camera*                 mCamera;                /// The camera from which the scene is rendered.
    bool                    mExit;                  /// Whether or not the renderer needs to exit.
    GBuffer                 mGBuffer;               /// Geometry buffer used for deferred rendering.
    ShadowMap               mShadowMap;

    GLuint                  mWidth;                 /// Width of the window.
    GLuint                  mHeight;                /// Height of the window.
    Quad                    mLightQuad;             /// Quad for performing the deferred lighting pass; covers the entire screen.
};

