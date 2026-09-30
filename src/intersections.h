#pragma once

#include "sceneStructs.h"

#include <glm/glm.hpp>
#include <glm/gtx/intersect.hpp>

/**
 * The slab method checks if a ray intersects an Axis-Aligned Bounding Box in 3D space.
 */
__host__ __device__ inline bool intersectAABB(
    const Ray& r,
    const glm::vec3& boxMin,
    const glm::vec3& boxMax,
    float& tmin_out)
{
    glm::vec3 invDir = 1.0f / r.direction;
    glm::vec3 t0 = (boxMin - r.origin) * invDir;
    glm::vec3 t1 = (boxMax - r.origin) * invDir;

    glm::vec3 tnear = glm::min(t0, t1);
    glm::vec3 tfar = glm::max(t0, t1);

    float tmin = glm::max(glm::max(tnear.x, tnear.y), tnear.z);
    float tmax = glm::min(glm::min(tfar.x, tfar.y), tfar.z);

    tmin_out = tmin;
    return tmax >= glm::max(0.0f, tmin);
}

/**
 * Custom Möller–Trumbore ray-triangle intersection optimized for CUDA device execution.
 */
__host__ __device__ inline bool intersectTriangle(
    const Ray& r,
    const Triangle& tri,
    float& t,
    glm::vec3& normal,
    glm::vec2& uv)
{
    const float EPSILON = 1e-8f;
    glm::vec3 e1 = tri.p1 - tri.p0;
    glm::vec3 e2 = tri.p2 - tri.p0;

    glm::vec3 pvec = glm::cross(r.direction, e2);
    float det = glm::dot(e1, pvec);

    // If determinant is near zero, ray lies in plane of triangle
    if (fabsf(det) < EPSILON) return false;

    float invDet = 1.0f / det;
    glm::vec3 tvec = r.origin - tri.p0;
    float u = glm::dot(tvec, pvec) * invDet;
    if (u < 0.0f || u > 1.0f) return false;

    glm::vec3 qvec = glm::cross(tvec, e1);
    float v = glm::dot(r.direction, qvec) * invDet;
    if (v < 0.0f || (u + v) > 1.0f) return false;

    float t_temp = glm::dot(e2, qvec) * invDet;
    if (t_temp <= 0.001f) return false;

    t = t_temp;
    float w = 1.0f - u - v;

    // Interpolate surface normal and UV coordinates across barycentric coordinates
    normal = glm::normalize(w * tri.n0 + u * tri.n1 + v * tri.n2);
    uv = w * tri.uv0 + u * tri.uv1 + v * tri.uv2;

    return true;
}

/**
 * Handy-dandy hash function that provides seeds for random number generation.
 */
__host__ __device__ inline unsigned int utilhash(unsigned int a)
{
    a = (a + 0x7ed55d16) + (a << 12);
    a = (a ^ 0xc761c23c) ^ (a >> 19);
    a = (a + 0x165667b1) + (a << 5);
    a = (a + 0xd3a2646c) ^ (a << 9);
    a = (a + 0xfd7046c5) + (a << 3);
    a = (a ^ 0xb55a4f09) ^ (a >> 16);
    return a;
}

// CHECKITOUT
/**
 * Compute a point at parameter value `t` on ray `r`.
 * Falls slightly short so that it doesn't intersect the object it's hitting.
 */
__host__ __device__ inline glm::vec3 getPointOnRay(Ray r, float t)
{
    return r.origin + (t - .0001f) * glm::normalize(r.direction);
}

/**
 * Multiplies a mat4 and a vec4 and returns a vec3 clipped from the vec4.
 */
__host__ __device__ inline glm::vec3 multiplyMV(glm::mat4 m, glm::vec4 v)
{
    return glm::vec3(m * v);
}

// CHECKITOUT
/**
 * Test intersection between a ray and a transformed cube. Untransformed,
 * the cube ranges from -0.5 to 0.5 in each axis and is centered at the origin.
 *
 * @param intersectionPoint  Output parameter for point of intersection.
 * @param normal             Output parameter for surface normal.
 * @param outside            Output param for whether the ray came from outside.
 * @return                   Ray parameter `t` value. -1 if no intersection.
 */
__host__ __device__ float boxIntersectionTest(
    Geom box,
    Ray r,
    glm::vec3& intersectionPoint,
    glm::vec3& normal,
    bool& outside);

// CHECKITOUT
/**
 * Test intersection between a ray and a transformed sphere. Untransformed,
 * the sphere always has radius 0.5 and is centered at the origin.
 *
 * @param intersectionPoint  Output parameter for point of intersection.
 * @param normal             Output parameter for surface normal.
 * @param outside            Output param for whether the ray came from outside.
 * @return                   Ray parameter `t` value. -1 if no intersection.
 */
__host__ __device__ float sphereIntersectionTest(
    Geom sphere,
    Ray r,
    glm::vec3& intersectionPoint,
    glm::vec3& normal,
    bool& outside);
