#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/ext.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "AABB.h"
#include "IcosphereCreator.h"
#include "OpenGLProperties.h"
#include "Quad.h"
#include "Renderer.h"
#include "Sphere.h"

Renderer::Renderer(std::vector<Object*>& objList)
    : mObjList(objList)
    , mLight()
    , mCamera(NULL)
    , mExit(false)
    , mCurrShader(eFORWARD_RENDER)
    , mWidth(1024)
    , mHeight(768)
    , mLightQuad(1.0, 1.0)
{
    memset(mShaders, 0, sizeof(GLuint) * eNUMSHADERS);
}

Renderer::~Renderer()
{
}

void Renderer::setCamera(Camera& cam)
{
    mCamera = &cam;
}

void Renderer::setLight(PointLight& light)
{
    mLight = light;
}

bool Renderer::init()
{
    glewExperimental = GL_TRUE;
    glewInit();

    // Get version info.
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* version = glGetString(GL_VERSION);
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);

    /*if(!loadShader(eFORWARD_RENDER, "vertex_shader.txt", "fragment_shader.txt"))
    {
        std::cerr << "Error loading forward render shader. Exiting." << std::endl;
        return false;
    }*/

    if(!loadShader(eDEFFERED_GEOM_PASS, "deferred_geo.vs", "deferred_geo.fs"))
    {
        std::cerr << "Error loading deferred geometry shader. Exiting." << std::endl;
        return false;
    }

    if(!loadShader(eDEFERRED_LIGHT_PASS, "light_pass.vs", "light_pass.fs"))
    {
        std::cerr << "Error loading deferred lighting shader. Exiting." << std::endl;
        return false;
    }

    if(!loadShader(eSHADOW_PASS, "shadow_pass.vs", "shadow_pass.fs"))
    {
        std::cerr << "Error loading shadow map shader. Exiting." << std::endl;
        return false;
    }

    if(!mGBuffer.init(mWidth, mHeight))
    {
        std::cerr << "Failed to initialize G-buffer." << std::endl;
        return false;
    }

    if (!mShadowMap.init(1024, 1024))
    {
        std::cerr << "Failed to initialize shadow map." << std::endl;
        return false;
    }

    mCurrShader = eDEFERRED_LIGHT_PASS;
    glUseProgram(mShaders[eDEFERRED_LIGHT_PASS]);
    glUniform1i(glGetUniformLocation(mShaders[eDEFERRED_LIGHT_PASS], "posBuff"), static_cast<GLint>(GBuffer::ePOSITION));
    glUniform1i(glGetUniformLocation(mShaders[eDEFERRED_LIGHT_PASS], "colorBuff"), static_cast<GLint>(GBuffer::eCOLOR));
    glUniform1i(glGetUniformLocation(mShaders[eDEFERRED_LIGHT_PASS], "normalBuff"), static_cast<GLint>(GBuffer::eNORMAL));
    glUniform1i(glGetUniformLocation(mShaders[eDEFERRED_LIGHT_PASS], "shadowMap"), static_cast<GLint>(GBuffer::eNUMBUFFERS));
    glUniform2f(glGetUniformLocation(mShaders[eDEFERRED_LIGHT_PASS], "screenDim"), static_cast<GLfloat>(mWidth), static_cast<GLfloat>(mHeight));
    

    initQuad(&mLightQuad);

    std::cout << "Shaders compiled." << std::endl;
    
    initObjects();
    std::cout << "Objects initialized." << std::endl;
    return true;
}


void Renderer::initObjects()
{
    for (auto obj : mObjList)
    {
        switch (obj->type())
        {
            case eAABB:
            {
                initBox(obj);
            }
            break;

            case eQUAD:
            {
                initQuad(obj);
                
            }
            break;
            case eSPHERE:
            {
                initSphere(obj);
            }
            break;
        }

        // unbind the VBO and VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}


bool Renderer::loadShader(uint32_t shader, const char* vert_shader, const char* frag_shader)
{
    GLuint vs_obj = readShader(vert_shader, GL_VERTEX_SHADER);
    GLuint fs_obj = readShader(frag_shader, GL_FRAGMENT_SHADER);

    if(vs_obj == 0 || fs_obj == 0)
    {
        std::cerr << "Error reading shaders." << std::endl;
        return false;
    }

    mShaders[shader] = glCreateProgram();
    if(mShaders[shader] == 0)
    {
        std::cerr << "glCreateProgram failed" << std::endl;
        return false;
    }
        

    glAttachShader(mShaders[shader], vs_obj);
    glAttachShader(mShaders[shader], fs_obj);

    glLinkProgram(mShaders[shader]);

    glDetachShader(mShaders[shader], vs_obj);
    glDetachShader(mShaders[shader], fs_obj);

    GLint status;
    glGetProgramiv(mShaders[shader], GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::string errMsg("Program linking failure: ");

        GLint infoLogLength;
        glGetProgramiv(mShaders[shader], GL_INFO_LOG_LENGTH, &infoLogLength);
        char* infoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(mShaders[shader], infoLogLength, NULL, infoLog);

        errMsg += infoLog;
        delete[] infoLog;

        glDeleteProgram(mShaders[shader]);
        
        std::cerr << errMsg << std::endl;
        return false;
    }

    return true;
}

GLuint Renderer::readShader(const char* filename, GLenum shader_type)
{
    std::ifstream vs(filename, std::ios::in | std::ios::binary);

    std::stringstream vs_buffer;
    vs_buffer.str("");
    vs_buffer << vs.rdbuf();

    std::string vs_str = vs_buffer.str();
    const char* vs_code = vs_str.c_str();

    GLuint vs_obj = glCreateShader(shader_type);
    if(vs_obj == 0)
    {
        std::cerr << "glCreateShader failed" << std::endl;
        return 0;
    }

    glShaderSource(vs_obj, 1, (const GLchar**)&vs_code, NULL);
    glCompileShader(vs_obj);

    GLint status;
    glGetShaderiv(vs_obj, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::string errMsg("Compile failure in shader:\n");

        GLint infoLogLength;
        glGetShaderiv(vs_obj, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* infoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(vs_obj, infoLogLength, NULL, infoLog);

        errMsg += infoLog;
        delete[] infoLog;

        glDeleteShader(vs_obj);
        std::cerr << errMsg << std::endl;
        return 0;
    }

    return vs_obj;
}

void Renderer::renderObj(Object* obj, const glm::mat4& cam)
{
    
    glUseProgram(mShaders[mCurrShader]);
    glUniformMatrix4fv(glGetUniformLocation(mShaders[mCurrShader], "camera"), 1, GL_FALSE, glm::value_ptr(cam));
    glUniform3fv(glGetUniformLocation(mShaders[mCurrShader], "color"), 1, glm::value_ptr(obj->rend().getColor()));
    /*glUniform1f(glGetUniformLocation(mShaders[mCurrShader], "ambient"), mLight.ambient);
    glUniform1f(glGetUniformLocation(mShaders[mCurrShader], "attenuation"), mLight.attenuation);
    glUniform3fv(glGetUniformLocation(mShaders[mCurrShader], "lightPos"), 1, glm::value_ptr(mLight.pos));
    glUniform3fv(glGetUniformLocation(mShaders[mCurrShader], "lightCol"), 1, glm::value_ptr(mLight.color));
    glUniform3fv(glGetUniformLocation(mShaders[mCurrShader], "cameraPos"), 1, glm::value_ptr(mCamera.mPos));

    glUniform1f(glGetUniformLocation(mShaders[mCurrShader], "shininess"), static_cast<float>(obj->rend().getShininess()));
    glUniform3fv(glGetUniformLocation(mShaders[mCurrShader], "color"), 1, glm::value_ptr(obj->rend().getColor()));
    glUniform3fv(glGetUniformLocation(mShaders[mCurrShader], "specColor"), 1, glm::value_ptr(obj->rend().getSpecColor()));*/

    OpenGLProperties* oglProp = static_cast<OpenGLProperties*>(obj->rend().rendProp());

    if (obj->type() == eAABB)
    {
        AABB* aabb = dynamic_cast<AABB*>(obj);
        
        /*glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m->tex);
        glUniform1i(glGetUniformLocation(mShader, "tex"), 0);*/
        
        glm::mat4 transform(glm::translate(glm::mat4(), obj->phys().pos()) * 
                            glm::mat4_cast(obj->phys().rot()) * glm::scale(glm::mat4(), aabb->getDim()));
        glm::mat3 invTranspose = glm::transpose(glm::inverse(glm::mat3(transform)));

        glUniformMatrix4fv(glGetUniformLocation(mShaders[mCurrShader], "model"), 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix3fv(glGetUniformLocation(mShaders[mCurrShader], "normal_matrix"), 1, GL_FALSE, glm::value_ptr(invTranspose));
    }
    else if(obj->type() == eQUAD)
    {
        Quad* quad = dynamic_cast<Quad*>(obj);

        /*glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m->tex);
        glUniform1i(glGetUniformLocation(mShader, "tex"), 0);*/
        glm::vec2 dim = quad->getDim();
        glm::mat4 transform(glm::translate(glm::mat4(), obj->phys().pos()) *
            glm::mat4_cast(obj->phys().rot()) * glm::scale(glm::mat4(), glm::vec3(dim.x, dim.y, 1.0f)));
        glm::mat3 invTranspose = glm::transpose(glm::inverse(glm::mat3(transform)));

        glUniformMatrix4fv(glGetUniformLocation(mShaders[mCurrShader], "model"), 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix3fv(glGetUniformLocation(mShaders[mCurrShader], "normal_matrix"), 1, GL_FALSE, glm::value_ptr(invTranspose));
    }
    else if(obj->type() == eSPHERE)
    {
        Sphere* sphere = dynamic_cast<Sphere*>(obj);

        glm::mat4 transform(glm::translate(glm::mat4(), obj->phys().pos()) * 
                            glm::mat4_cast(obj->phys().rot()) * glm::scale(glm::mat4(), glm::vec3(float(sphere->getRadius()))));
        glm::mat3 invTranspose = glm::transpose(glm::inverse(glm::mat3(transform)));

        glUniformMatrix4fv(glGetUniformLocation(mShaders[mCurrShader], "model"), 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix3fv(glGetUniformLocation(mShaders[mCurrShader], "normal_matrix"), 1, GL_FALSE, glm::value_ptr(invTranspose));
    }
   
    glBindVertexArray(oglProp->VAO);
    glDrawArrays(GL_TRIANGLES, 0, oglProp->VBOSize);

    glBindVertexArray(0);
    //glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}

void Renderer::initBox(Object * obj)
{
    AABB* aabb = dynamic_cast<AABB*>(obj);

    if(!aabb)
        return;

    OpenGLProperties* glProp = new OpenGLProperties();
    glGenVertexArrays(1, &glProp->VAO);
    glBindVertexArray(glProp->VAO);
    glProp->VBOSize = 36;

    glGenBuffers(1, &glProp->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, glProp->VBO);

    GLfloat vertexData[] = {
        //  X     Y     Z       U     V          Normal
        // bottom
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

        // top
        -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        // front
        -1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        // back
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        -1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f,

        // left
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        // right
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

    setVertexAttributes();

    obj->rend().setRendProp(glProp);
}

void Renderer::initQuad(Object* obj)
{
    Quad* quad = dynamic_cast<Quad*>(obj);

    if(!quad)
        return;

    OpenGLProperties* glProp = new OpenGLProperties();
    glGenVertexArrays(1, &glProp->VAO);
    glBindVertexArray(glProp->VAO);
    glProp->VBOSize = 6;

    glGenBuffers(1, &glProp->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, glProp->VBO);

    GLfloat vertexData[] = {
        /*  X     Y      Z     U     V       Normal
        -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, 0.0f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,*/
        //  X     Y      Z     U     V       Normal
        -1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
    };

    glBufferData(GL_ARRAY_BUFFER, 6*8*sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

    setVertexAttributes();

    obj->rend().setRendProp(glProp);
}

void Renderer::initSphere(Object * obj)
{
    Sphere* sphere = dynamic_cast<Sphere*>(obj);

    if(!sphere)
        return;

    IcosphereCreator sphereCreator;
    sphereCreator.create(3);

    OpenGLProperties* glProp = new OpenGLProperties();
    glGenVertexArrays(1, &glProp->VAO);
    glBindVertexArray(glProp->VAO);

    glGenBuffers(1, &glProp->VBO);
    glBindBuffer(GL_ARRAY_BUFFER, glProp->VBO);

    std::vector<glm::vec3>& vertList = sphereCreator.getVertexList();
    std::vector<IcosphereCreator::TriangleIndices>& triList = sphereCreator.getTriList();
    glProp->VBOSize = uint32_t(triList.size() * 3);

    GLfloat* vertexData = new GLfloat[triList.size() * 3 * 8];

    for(glm::uint32 i = 0; i < triList.size(); ++i)
    {
        for(glm::uint32 j = 0; j < 3; ++j)
        {
            glm::uint32 idx = i * 24 + j * 8;
            vertexData[idx] = vertList[triList[i][j]].x;
            vertexData[idx + 1] = vertList[triList[i][j]].y;
            vertexData[idx + 2] = vertList[triList[i][j]].z;

            vertexData[idx + 3] = 0.0f;
            vertexData[idx + 4] = 0.0f;

            vertexData[idx + 5] = vertList[triList[i][j]].x;
            vertexData[idx + 6] = vertList[triList[i][j]].y;
            vertexData[idx + 7] = vertList[triList[i][j]].z;
        }
    }

    glBufferData(GL_ARRAY_BUFFER, triList.size() * 3 * 8 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

    setVertexAttributes();

    obj->rend().setRendProp(glProp);
    delete[] vertexData;
}

void Renderer::setVertexAttributes()
{
	uint32_t shaders[] = { eSHADOW_PASS, eDEFFERED_GEOM_PASS };

	for(uint32_t i = 0; i < 2; ++i)
	{
		glEnableVertexAttribArray(glGetAttribLocation(mShaders[shaders[i]], "vert"));
		glVertexAttribPointer(glGetAttribLocation(mShaders[shaders[i]], "vert"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);

		glEnableVertexAttribArray(glGetAttribLocation(mShaders[shaders[i]], "vertTexCoord"));
		glVertexAttribPointer(glGetAttribLocation(mShaders[shaders[i]], "vertTexCoord"), 2, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat),
			(const GLvoid*)(3 * sizeof(GLfloat)));

		glEnableVertexAttribArray(glGetAttribLocation(mShaders[shaders[i]], "vertNormal"));
		glVertexAttribPointer(glGetAttribLocation(mShaders[shaders[i]], "vertNormal"), 3, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat),
			(const GLvoid*)(5 * sizeof(GLfloat)));
	}
}

struct CameraDirection
{
    GLenum face;
    glm::vec3 forward;
    glm::vec3 up;
};

CameraDirection gCameraDirections[6] =
{
    { GL_TEXTURE_CUBE_MAP_POSITIVE_X, glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_X, glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
    { GL_TEXTURE_CUBE_MAP_POSITIVE_Y, glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f,  1.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f) },
    { GL_TEXTURE_CUBE_MAP_POSITIVE_Z, glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f) },
    { GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f) }
};

void Renderer::shadowPass()
{
    mCurrShader = eSHADOW_PASS;
    glUseProgram(mShaders[mCurrShader]);
    glViewport(0, 0, 1024, 1024);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUniform3fv(glGetUniformLocation(mShaders[mCurrShader], "lightPos"), 1, glm::value_ptr(mLight.pos));

    glClearColor(FLT_MAX, FLT_MAX, FLT_MAX, 1.0);
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    glm::mat4 pMat = glm::perspective(glm::radians(90.0), 1.0, mCamera->mNearPlane, mCamera->mFarPlane);

    for(uint32_t i = 0; i < 6; ++i)
    {
        mShadowMap.writeToShadowMap(gCameraDirections[i].face);
        
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

        glm::mat4 camMat = pMat * glm::lookAt(mLight.pos, mLight.pos + gCameraDirections[i].forward, gCameraDirections[i].up);

        for(Object* obj : mObjList)
        {
            renderObj(obj, camMat);
        }
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glViewport(0, 0, mWidth, mHeight);
}

bool Renderer::render()
{
    glm::vec3 forward, up, right;
    glm::mat4 cam = mCamera->get_matrix(forward, up, right);

    shadowPass();
    geometryPass(cam);
    lightPass();
    
    return true;
}

void Renderer::geometryPass(const glm::mat4& cam)
{
    mCurrShader = eDEFFERED_GEOM_PASS;
    mGBuffer.bindGBuffer();

    // Only the geometry pass updates the depth buffer
    glDepthMask(GL_TRUE);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    for(auto obj : mObjList)
    {
        renderObj(obj, cam);
    }
}

void Renderer::lightPass()
{
    mCurrShader = eDEFERRED_LIGHT_PASS;
    glUseProgram(mShaders[mCurrShader]);

    mGBuffer.bindGBufferTextures();
    mShadowMap.bindShadowTexture(GBuffer::eNUMBUFFERS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_ONE, GL_ONE);
    //glDepthMask(GL_FALSE);
    //glDisable(GL_DEPTH_TEST);

    glUniform1f(glGetUniformLocation(mShaders[mCurrShader], "ambient"), mLight.ambient);
    glUniform1f(glGetUniformLocation(mShaders[mCurrShader], "attenuation"), mLight.attenuation);
    glUniform3fv(glGetUniformLocation(mShaders[mCurrShader], "lightPos"), 1, glm::value_ptr(mLight.pos));
    glUniform3fv(glGetUniformLocation(mShaders[mCurrShader], "lightCol"), 1, glm::value_ptr(mLight.color));
    glUniform3fv(glGetUniformLocation(mShaders[mCurrShader], "cameraPos"), 1, glm::value_ptr(mCamera->mPos));

    renderObj(&mLightQuad, glm::mat4());
}
