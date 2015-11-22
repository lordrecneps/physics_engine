#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/detail/type_int.hpp>

class IcosphereCreator
{
public:
    IcosphereCreator();
    ~IcosphereCreator();

    struct TriangleIndices
    {
        int v[3];

        TriangleIndices(int v1, int v2, int v3)
        {
            v[0] = v1;
            v[1] = v2;
            v[2] = v3;
        }

        int operator[](int i)
        {
            return v[i];
        }
    };

    /*!
    *   @brief  Creates an icosphere with the given number of subdivisions.
    *   @param[in]  recursionLevel  The number of subdivisions to perform.
    */
    void create(int recursionLevel);

    std::vector<glm::vec3>& getVertexList();

    std::vector<TriangleIndices>& getTriList();

private:
    /*!
    *   @brief  Add vertex to mesh, fix position to be on unit sphere, return index.
    */
    glm::uint32 addVertex(glm::vec3 p);

    /*!
    *   @brief  Return index of point in the middle of p1 and p2.
    */
    int getMiddlePoint(glm::uint32 p1, glm::uint32 p2);

private:
    std::vector<glm::vec3>                  mVertexList;
    std::vector<TriangleIndices>            mTriList;
    std::map<glm::uint64, glm::uint32>      mMiddlePointCache;
};

