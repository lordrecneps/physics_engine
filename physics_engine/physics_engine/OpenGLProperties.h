#pragma once

#include <GL/glew.h>

#include "RendererProperties.h"

/*!
 *  @brief  OpenGL specific properties used to render an object.
 */
struct OpenGLProperties : public RendererProperties
{
    OpenGLProperties() 
        : VAO(0)
        , VBO(0)
        , VBOSize(0)
    { }

    GLuint VAO;         /// Vertex Array Object.
    GLuint VBO;         /// Vertex Buffer Object.
    uint32_t VBOSize;   /// The number of vertices that the object contains.

};