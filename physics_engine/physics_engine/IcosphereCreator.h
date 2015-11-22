#pragma once

#include <map>
#include <vector>

#include <glm/glm.hpp>
#include <glm/detail/type_int.hpp>

/*!
 *  This class creates a triangulation of a sphere based on a subdivision of an icosahedron.
 *  This code is basically a C++ translation of the algorithm presented here:
 *  http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
 *  by Andreas Kahler. Many thanks to him.
 */
class IcosphereCreator
{
public:
    IcosphereCreator();
    ~IcosphereCreator();

    /*!
     *   @brief A triangle, whose vertices are represented by their indices in the vertex list for the icosphere.
     */
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

    /*!
     *  @brief  Returns the list of vertices for this icosphere.
     *  @return The list of vertices for this icosphere.
     */
    std::vector<glm::vec3>& getVertexList();

    /*!
    *  @brief  Returns the list of triangle faces for this icosphere.
    *  @return The list of triangle faces for this icosphere.
    */
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
    std::vector<glm::vec3>                  mVertexList;        /// The list of vertices for the subdivided icosahedron.
    std::vector<TriangleIndices>            mTriList;           /// The list of triangle faces for the subdivided icosahedron.
    std::map<glm::uint64, glm::uint32>      mMiddlePointCache;  /// A cache used for creating the middle vertex when subdividing edges. Since each
                                                                /// edge is shared by two triangles, we don't want to end up duplicating vertices.
};

