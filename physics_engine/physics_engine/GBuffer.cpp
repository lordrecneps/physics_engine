#include <cstdint>
#include <cstring>
#include <iostream>


#include "GBuffer.h"



GBuffer::GBuffer()
    : mFBO(0)
    , mDepthBuffer(0)
    , mWidth(0)
    , mHeight(0)
{
    memset(mBuffers, 0, sizeof(GLuint) * eNUMBUFFERS);
}


GBuffer::~GBuffer()
{
    if(mFBO)
    {
        glDeleteFramebuffers(1, &mFBO);
    }

    if(mBuffers[0])
    {
        glDeleteTextures(eNUMBUFFERS, mBuffers);
    }

    if(mDepthBuffer)
    {
        glDeleteTextures(1, &mDepthBuffer);
    }
}

bool GBuffer::init(GLuint width, GLuint height)
{
    mWidth = width;
    mHeight = height;

    // Create the FBO
    glGenFramebuffers(1, &mFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);

    // Create the gbuffer textures
    glGenTextures(eNUMBUFFERS, mBuffers);
    glGenTextures(1, &mDepthBuffer);

    for(uint32_t i = 0; i < eNUMBUFFERS; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mBuffers[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, mWidth, mHeight, 0, GL_RGB, GL_FLOAT, NULL);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, mBuffers[i], 0);
    }

    // depth
    glBindTexture(GL_TEXTURE_2D, mDepthBuffer);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepthBuffer, 0);

    GLenum drawBuffers[] = {
        GL_COLOR_ATTACHMENT0,
        GL_COLOR_ATTACHMENT1,
        GL_COLOR_ATTACHMENT2
    };

    glDrawBuffers(eNUMBUFFERS, drawBuffers);

    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if(status != GL_FRAMEBUFFER_COMPLETE)
    {
        std::cerr << "Failed to initialize frame buffers for G-buffer. GL framebuffer status: " << status << std::endl;
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    return true;
}

void GBuffer::bindGBuffer()
{
    glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
}

void GBuffer::bindGBufferTextures()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    for(uint32_t i = 0; i < eNUMBUFFERS; ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, mBuffers[ePOSITION + i]);
    }
}


void GBuffer::setReadFrameBuffer()
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, mFBO);
}
