#include <cmath>

#include "IcosphereCreator.h"

IcosphereCreator::IcosphereCreator()
{
}


IcosphereCreator::~IcosphereCreator()
{
}

glm::uint32 IcosphereCreator::addVertex(glm::dvec3 p)
{
    mVertexList.push_back(glm::normalize(p));
    return glm::uint32(mVertexList.size() - 1);
}

int IcosphereCreator::getMiddlePoint(glm::uint32 p1, glm::uint32 p2)
{
    // first check if we have it already
    bool firstIsSmaller = p1 < p2;
    glm::uint64 smallerIndex = firstIsSmaller ? p1 : p2;
    glm::uint64 greaterIndex = firstIsSmaller ? p2 : p1;
    glm::uint64 key = (smallerIndex << 32) + greaterIndex;

    if (mMiddlePointCache.count(key))
    {
        return mMiddlePointCache[key];
    }

    // not in cache, calculate it
    glm::dvec3 point1 = mVertexList[p1];
    glm::dvec3 point2 = mVertexList[p2];
    glm::dvec3 middle = point1 + point2;
    middle /= 2.0;

    // add vertex makes sure point is on the unit sphere
    glm::uint32 i = addVertex(middle);

    // store it, return index
    mMiddlePointCache[key] = i;
    return i;
}

void IcosphereCreator::create(int recursionLevel)
{
    mVertexList.clear();
    mTriList.clear();
    mMiddlePointCache.clear();

    // create 12 vertices of an icosahedron
    double t = (1.0 + sqrt(5.0)) / 2.0;

    addVertex(glm::dvec3(-1, t, 0));
    addVertex(glm::dvec3(1, t, 0));
    addVertex(glm::dvec3(-1, -t, 0));
    addVertex(glm::dvec3(1, -t, 0));

    addVertex(glm::dvec3(0, -1, t));
    addVertex(glm::dvec3(0, 1, t));
    addVertex(glm::dvec3(0, -1, -t));
    addVertex(glm::dvec3(0, 1, -t));

    addVertex(glm::dvec3(t, 0, -1));
    addVertex(glm::dvec3(t, 0, 1));
    addVertex(glm::dvec3(-t, 0, -1));
    addVertex(glm::dvec3(-t, 0, 1));

    // 5 faces around point 0
    mTriList.push_back(TriangleIndices(0, 11, 5));
    mTriList.push_back(TriangleIndices(0, 5, 1));
    mTriList.push_back(TriangleIndices(0, 1, 7));
    mTriList.push_back(TriangleIndices(0, 7, 10));
    mTriList.push_back(TriangleIndices(0, 10, 11));

    // 5 adjacent faces 
    mTriList.push_back(TriangleIndices(1, 5, 9));
    mTriList.push_back(TriangleIndices(5, 11, 4));
    mTriList.push_back(TriangleIndices(11, 10, 2));
    mTriList.push_back(TriangleIndices(10, 7, 6));
    mTriList.push_back(TriangleIndices(7, 1, 8));

    // 5 faces around point 3
    mTriList.push_back(TriangleIndices(3, 9, 4));
    mTriList.push_back(TriangleIndices(3, 4, 2));
    mTriList.push_back(TriangleIndices(3, 2, 6));
    mTriList.push_back(TriangleIndices(3, 6, 8));
    mTriList.push_back(TriangleIndices(3, 8, 9));

    // 5 adjacent faces 
    mTriList.push_back(TriangleIndices(4, 9, 5));
    mTriList.push_back(TriangleIndices(2, 4, 11));
    mTriList.push_back(TriangleIndices(6, 2, 10));
    mTriList.push_back(TriangleIndices(8, 6, 7));
    mTriList.push_back(TriangleIndices(9, 8, 1));


    // refine triangles
    for (int i = 0; i < recursionLevel; i++)
    {
        std::vector<TriangleIndices> newFaces;
        for(auto tri : mTriList)
        {
            // replace triangle by 4 triangles
            int a = getMiddlePoint(tri.v[0], tri.v[1]);
            int b = getMiddlePoint(tri.v[1], tri.v[2]);
            int c = getMiddlePoint(tri.v[2], tri.v[0]);

            newFaces.push_back(TriangleIndices(tri.v[0], a, c));
            newFaces.push_back(TriangleIndices(tri.v[1], b, a));
            newFaces.push_back(TriangleIndices(tri.v[2], c, b));
            newFaces.push_back(TriangleIndices(a, b, c));
        }
        mTriList = newFaces;
    }
}

std::vector<glm::dvec3>& IcosphereCreator::getVertexList()
{
    return mVertexList;
}

std::vector<IcosphereCreator::TriangleIndices>& IcosphereCreator::getTriList()
{
    return mTriList;
}
