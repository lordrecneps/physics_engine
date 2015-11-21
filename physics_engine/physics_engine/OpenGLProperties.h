#pragma once

#include <GL/glew.h>

#include "RendererProperties.h"

struct OpenGLProperties : public RendererProperties
{
    OpenGLProperties() : VAO(0), VBO(0)
    { }

    GLuint VAO;
    GLuint VBO;
};