#include <cstdint>
#include <iostream>

#include "ShadowMap.h"



ShadowMap::ShadowMap()
	: mFBO(0)
	, mShadowMap(0)
	, mDepth(0)
{
}


ShadowMap::~ShadowMap()
{
	if (mFBO)
	{
		glDeleteFramebuffers(1, &mFBO);
	}

	if (mShadowMap)
	{
		glDeleteTextures(1, &mShadowMap);
	}

	if (mDepth)
	{
		glDeleteTextures(1, &mDepth);
	}
}

bool ShadowMap::init(GLuint width, GLuint height)
{
	mWidth = width;
	mHeight = height;

	glGenFramebuffers(1, &mFBO);

	// Create the depth buffer
	glGenTextures(1, &mDepth);
	glBindTexture(GL_TEXTURE_2D, mDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, mWidth, mHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Create the cube map
	glGenTextures(1, &mShadowMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mShadowMap);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	for (uint32_t i = 0; i < 6; ++i)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_R32F, mWidth, mHeight, 0, GL_RED, GL_FLOAT, NULL);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, mFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, mDepth, 0);

	// Disable writes to the color buffer
	glDrawBuffer(GL_NONE);

	// Disable reads from the color buffer
	glReadBuffer(GL_NONE);

	GLenum err = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (err != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Framebuffer error: " << err << std::endl;
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}

void ShadowMap::writeToShadowMap(GLenum face)
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, mFBO);
	glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, face, mShadowMap, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}

void ShadowMap::bindShadowTexture(uint32_t unit)
{
	glActiveTexture(GL_TEXTURE0 + unit);
	glBindTexture(GL_TEXTURE_CUBE_MAP, mShadowMap);
}
