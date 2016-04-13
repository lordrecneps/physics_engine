#pragma once

#include <GL\glew.h>

class ShadowMap
{
public:
	ShadowMap();
	~ShadowMap();

	bool init(GLuint width, GLuint height);

	void writeToShadowMap(GLenum face);

	void bindShadowTexture(uint32_t unit);

    struct CameraDirection
    {
        GLenum face;
        glm::dvec3 forward;
        glm::dvec3 up;
    };

    static CameraDirection sCamDir[6];

private:
	GLuint		mFBO;
	GLuint      mShadowMap;
	GLuint		mDepth;

	GLuint      mWidth;                 /// The width of the shadow map.
	GLuint      mHeight;                /// The height of the shadow map.
};

