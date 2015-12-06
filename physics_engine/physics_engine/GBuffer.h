#pragma once

#include <GL\glew.h>

class GBuffer
{
public:
    GBuffer();
    ~GBuffer();

    enum eGBufferTypes
    {
        ePOSITION,
        eCOLOR,
        eNORMAL,
        eNUMBUFFERS
    };

    bool init(GLuint width, GLuint height);

    void bindGBuffer();
    void bindGBufferTextures();
    void setReadFrameBuffer();
private:
    GLuint      mFBO;                   /// GL Frame buffer object for the G-buffer.
    GLuint      mBuffers[eNUMBUFFERS];  /// The texture handles for all the buffers except for the depth buffer.
    GLuint      mDepthBuffer;           /// The texture handle for the depth buffer.

    GLuint      mWidth;                 /// The width of the window.
    GLuint      mHeight;                /// The height of the window.
};

