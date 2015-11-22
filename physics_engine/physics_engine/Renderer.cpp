#include <fstream>
#include <iostream>
#include <sstream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>


#include "AABB.h"
#include "IcosphereCreator.h"
#include "OpenGLProperties.h"
#include "Renderer.h"
#include "Sphere.h"

#define SHADERSDIR "Shaders/"

Renderer::Renderer(std::vector<Object*>& objList)
    : mObjList(objList)
    , mWindow(NULL)
    , mShader(NULL)
    , mLight()
    , mCamera()
{
}

Renderer::~Renderer()
{
}

void Renderer::setCamera(Camera& cam)
{
    mCamera = cam;
}

bool Renderer::init()
{
    if (!glfwInit())
    {
        std::cerr << "Failed to initialize GLFW." << std::endl;
        return false;
    }

    mWindow = glfwCreateWindow(640, 480, "Boing", NULL, NULL);
    if (!mWindow)
    {
        std::cerr << "Could not open window with GLFW3" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(mWindow);

    // start GLEW extension handler
    glewExperimental = GL_TRUE;
    glewInit();

    // get version info
    const GLubyte* renderer = glGetString(GL_RENDERER); // get renderer string
    const GLubyte* version = glGetString(GL_VERSION); // version as a string
    std::cout << "Renderer: " << renderer << std::endl;
    std::cout << "OpenGL version supported " << version << std::endl;

    // tell GL to only draw onto a pixel if the shape is closer to the viewer
    glEnable(GL_DEPTH_TEST); // enable depth-testing
    glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"

    try
    {
        load_shader(SHADERSDIR"vertex_shader.txt", SHADERSDIR"fragment_shader.txt");
    }
    catch (const std::exception& e)
    {
        std::cerr << "ERROR: " << e.what() << std::endl;
        return false;
    }

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
                AABB* aabb = dynamic_cast<AABB*>(obj);
                glm::vec3 dim = aabb->getDim();
                dim /= 2.0;

                OpenGLProperties* glProp = new OpenGLProperties();
                // make and bind the VAO
                glGenVertexArrays(1, &glProp->VAO);
                glBindVertexArray(glProp->VAO);
                glProp->VBOSize = 36;

                // make and bind the VBO
                glGenBuffers(1, &glProp->VBO);
                glBindBuffer(GL_ARRAY_BUFFER, glProp->VBO);

                // Put the three triangle verticies into the VBO
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
                std::cout << "box vert size: " << sizeof(vertexData) << std::endl;
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

                // connect the xyz to the "vert" attribute of the vertex shader
                glEnableVertexAttribArray(glGetAttribLocation(mShader, "vert"));
                glVertexAttribPointer(glGetAttribLocation(mShader, "vert"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);

                glEnableVertexAttribArray(glGetAttribLocation(mShader, "vertTexCoord"));
                glVertexAttribPointer(glGetAttribLocation(mShader, "vertTexCoord"), 2, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

                glEnableVertexAttribArray(glGetAttribLocation(mShader, "vertNormal"));
                glVertexAttribPointer(glGetAttribLocation(mShader, "vertNormal"), 3, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat)));

                obj->rend().setRendProp(glProp);
            }
            break;

            case eSPHERE:
            {
                Sphere* sphere = dynamic_cast<Sphere*>(obj);

                IcosphereCreator sphereCreator;
                sphereCreator.create(3);

                OpenGLProperties* glProp = new OpenGLProperties();
                // make and bind the VAO
                glGenVertexArrays(1, &glProp->VAO);
                glBindVertexArray(glProp->VAO);

                // make and bind the VBO
                glGenBuffers(1, &glProp->VBO);
                glBindBuffer(GL_ARRAY_BUFFER, glProp->VBO);

                std::vector<glm::vec3>& vertList = sphereCreator.getVertexList();
                std::vector<IcosphereCreator::TriangleIndices>& triList = sphereCreator.getTriList();
                glProp->VBOSize = uint32_t(triList.size() * 3);

                GLfloat* vertexData = new GLfloat[triList.size() * 3 * 8];

                for (glm::uint32 i = 0; i < triList.size(); ++i)
                {
                    for (glm::uint32 j = 0; j < 3; ++j)
                    {
                        glm::uint32 idx = i*24 + j*8;
                        vertexData[idx]     = vertList[triList[i][j]].x;
                        vertexData[idx + 1] = vertList[triList[i][j]].y;
                        vertexData[idx + 2] = vertList[triList[i][j]].z;

                        vertexData[idx + 3] = 0.0f;
                        vertexData[idx + 4] = 0.0f;

                        vertexData[idx + 5] = vertList[triList[i][j]].x;
                        vertexData[idx + 6] = vertList[triList[i][j]].y;
                        vertexData[idx + 7] = vertList[triList[i][j]].z;

                        /*for (int k = 0; k < 8; ++k)
                            std::cout << vertexData[idx + k] << " ";
                        std::cout << std::endl;*/
                    }
                }

                glBufferData(GL_ARRAY_BUFFER, triList.size() * 3 * 8 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW);

                // connect the xyz to the "vert" attribute of the vertex shader
                glEnableVertexAttribArray(glGetAttribLocation(mShader, "vert"));
                glVertexAttribPointer(glGetAttribLocation(mShader, "vert"), 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), NULL);

                glEnableVertexAttribArray(glGetAttribLocation(mShader, "vertTexCoord"));
                glVertexAttribPointer(glGetAttribLocation(mShader, "vertTexCoord"), 2, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));

                glEnableVertexAttribArray(glGetAttribLocation(mShader, "vertNormal"));
                glVertexAttribPointer(glGetAttribLocation(mShader, "vertNormal"), 3, GL_FLOAT, GL_TRUE, 8 * sizeof(GLfloat), (const GLvoid*)(5 * sizeof(GLfloat)));

                obj->rend().setRendProp(glProp);
                std::cout << "Sphere created. " << triList.size() << " " << vertList.size() << std::endl;

                delete[] vertexData;
            }
            break;
        }

        // unbind the VBO and VAO
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
}


void Renderer::load_shader(const char* vert_shader, const char* frag_shader)
{
    GLuint vs_obj = read_shader(vert_shader, GL_VERTEX_SHADER);
    GLuint fs_obj = read_shader(frag_shader, GL_FRAGMENT_SHADER);

    mShader = glCreateProgram();
    if (mShader == 0)
        throw std::runtime_error("glCreateProgram failed");

    glAttachShader(mShader, vs_obj);
    glAttachShader(mShader, fs_obj);

    glLinkProgram(mShader);

    glDetachShader(mShader, vs_obj);
    glDetachShader(mShader, fs_obj);

    //throw exception if linking failed
    GLint status;
    glGetProgramiv(mShader, GL_LINK_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Program linking failure: ");

        GLint infoLogLength;
        glGetProgramiv(mShader, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetProgramInfoLog(mShader, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;

        glDeleteProgram(mShader);
        throw std::runtime_error(msg);
    }
}

GLuint Renderer::read_shader(const char* filename, GLenum shader_type)
{
    std::ifstream vs(filename, std::ios::in | std::ios::binary);

    std::stringstream vs_buffer;
    vs_buffer.str("");
    vs_buffer << vs.rdbuf();

    std::string vs_str = vs_buffer.str();
    const char* vs_code = vs_str.c_str();

    GLuint vs_obj = glCreateShader(shader_type);
    if (vs_obj == 0)
        throw std::runtime_error("glCreateShader failed");

    glShaderSource(vs_obj, 1, (const GLchar**)&vs_code, NULL);
    glCompileShader(vs_obj);

    GLint status;
    glGetShaderiv(vs_obj, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Compile failure in shader:\n");

        GLint infoLogLength;
        glGetShaderiv(vs_obj, GL_INFO_LOG_LENGTH, &infoLogLength);
        char* strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(vs_obj, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;

        glDeleteShader(vs_obj);
        throw std::runtime_error(msg);
    }

    return vs_obj;
}

void Renderer::render_obj(Object* obj, glm::mat4& cam)
{
    glUseProgram(mShader);

    glUniform1f(glGetUniformLocation(mShader, "ambient"), mLight.ambient);
    glUniform1f(glGetUniformLocation(mShader, "attenuation"), mLight.attenuation);
    glUniformMatrix4fv(glGetUniformLocation(mShader, "camera"), 1, GL_FALSE, glm::value_ptr(cam));
    glUniform3fv(glGetUniformLocation(mShader, "lightPos"), 1, glm::value_ptr(mLight.pos));
    glUniform3fv(glGetUniformLocation(mShader, "lightCol"), 1, glm::value_ptr(mLight.color));
    glUniform3fv(glGetUniformLocation(mShader, "cameraPos"), 1, glm::value_ptr(mCamera.mPos));

    glUniform1f(glGetUniformLocation(mShader, "shininess"), static_cast<float>(obj->rend().getShininess()));
    glUniform3fv(glGetUniformLocation(mShader, "specColor"), 1, glm::value_ptr(obj->rend().getSpecColor()));

    OpenGLProperties* oglProp = static_cast<OpenGLProperties*>(obj->rend().rendProp());

    if (obj->type() == eAABB)
    {
        AABB* aabb = dynamic_cast<AABB*>(obj);
        
        /*glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m->tex);
        glUniform1i(glGetUniformLocation(mShader, "tex"), 0);*/
        
        glm::mat4 transform(glm::translate(glm::mat4(), obj->phys().pos()) * glm::mat4_cast(obj->phys().rot()) * glm::scale(glm::mat4(), aabb->getDim()));
        glm::mat3 invTranspose = glm::transpose(glm::inverse(glm::mat3(transform)));

        glUniformMatrix4fv(glGetUniformLocation(mShader, "model"), 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix3fv(glGetUniformLocation(mShader, "normal_matrix"), 1, GL_FALSE, glm::value_ptr(invTranspose));
        //std::cout << glm::to_string(transform) << std::endl;
    }
    else if(obj->type() == eSPHERE)
    {
        Sphere* sphere = dynamic_cast<Sphere*>(obj);

        glm::mat4 transform(glm::translate(glm::mat4(), obj->phys().pos()) * glm::mat4_cast(obj->phys().rot()) * glm::scale(glm::mat4(), glm::vec3(float(sphere->getRadius()))));
        glm::mat3 invTranspose = glm::transpose(glm::inverse(glm::mat3(transform)));

        glUniformMatrix4fv(glGetUniformLocation(mShader, "model"), 1, GL_FALSE, glm::value_ptr(transform));
        glUniformMatrix3fv(glGetUniformLocation(mShader, "normal_matrix"), 1, GL_FALSE, glm::value_ptr(invTranspose));
        //std::cout << glm::to_string(transform) << std::endl;
    }

    glBindVertexArray(oglProp->VAO);
    glDrawArrays(GL_TRIANGLES, 0, oglProp->VBOSize);

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
}



void Renderer::render()
{
    glDisable(GL_CULL_FACE);
    glfwSetCursorPos(mWindow, 640 / 2, 480 / 2);
    while (!glfwWindowShouldClose(mWindow)) 
    {
        // wipe the drawing surface clear
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::vec3 forward, up, right;
        glm::mat4 cam = mCamera.get_matrix(&forward, &up, &right);
        
        
        for(auto obj : mObjList)
        {
            render_obj(obj, cam);
        }

        // update other events like input handling 
        glfwPollEvents();

        if (glfwGetKey(mWindow, 'S')) {
            mCamera.mPos -= 0.1f * forward;
        }
        else if (glfwGetKey(mWindow, 'W')) {
            mCamera.mPos += 0.1f * forward;
        }
        if (glfwGetKey(mWindow, 'A')) {
            mCamera.mPos -= 0.1f * right;
        }
        else if (glfwGetKey(mWindow, 'D')) {
            mCamera.mPos += 0.1f * right;
        }
        if (glfwGetKey(mWindow, 'X')) {
            mCamera.mPos -= 0.1f * up;
        }
        else if (glfwGetKey(mWindow, 'Z')) {
            mCamera.mPos += 0.1f * up;
        }
        if (glfwGetKey(mWindow, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(mWindow, GL_TRUE);

        mLight.pos = mCamera.mPos;

        double mouseX, mouseY;
        glfwGetCursorPos(mWindow, &mouseX, &mouseY);
        mCamera.update_angles(0.1f * (float)(mouseY - 480/2), 0.1f * (float)(mouseX - 640/2));
        glfwSetCursorPos(mWindow, 640/2, 480/2);
        // put the stuff we've been drawing onto the display
        glfwSwapBuffers(mWindow);
    }
}

void Renderer::close()
{
    glfwTerminate();
}

