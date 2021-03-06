/*
 * Ascent MMORPG Server
 * Copyright (C) 2005-2011 Ascent Team <http://www.ascentemulator.net/>
 *
 * This software is  under the terms of the EULA License
 * All title, including but not limited to copyrights, in and to the Ascent Software
 * and any copies there of are owned by ZEDCLANS INC. or its suppliers. All title
 * and intellectual property rights in and to the content which may be accessed through
 * use of the Ascent is the property of the respective content owner and may be protected
 * by applicable copyright or other intellectual property laws and treaties. This EULA grants
 * you no rights to use such content. All rights not expressly granted are reserved by ZEDCLANS INC.
 *
 */

#include "Collision/g3dlite/G3D/platform.h"
#include "Collision/g3dlite/G3D/Triangle.h"
#include "Collision/g3dlite/G3D/Plane.h"
#include "Collision/g3dlite/G3D/AABox.h"

namespace G3D {

    
void Triangle::init(const Vector3& v0, const Vector3& v1, const Vector3& v2) {

    _plane = Plane(v0, v1, v2);
    _vertex[0] = v0;
    _vertex[1] = v1;
    _vertex[2] = v2;

    static int next[] = {1,2,0};

    for (int i = 0; i < 3; ++i) {
        const Vector3 e  = _vertex[next[i]] - _vertex[i];
        edgeMagnitude[i]    = e.magnitude();

        if (edgeMagnitude[i] == 0) {
            edgeDirection[i] = Vector3::zero();
        } else {
            edgeDirection[i] = e / (float)edgeMagnitude[i];
        }
    }

    edge01 = _vertex[1] - _vertex[0];
    edge02 = _vertex[2] - _vertex[0];

    _primaryAxis = _plane.normal().primaryAxis();
    _area = (float)edgeDirection[0].cross(edgeDirection[2]).magnitude() * (edgeMagnitude[0] * edgeMagnitude[2]);

}


Triangle::Triangle() {
    init(Vector3::zero(), Vector3::zero(), Vector3::zero());
}
    

Triangle::Triangle(const Vector3& v0, const Vector3& v1, const Vector3& v2) {
    init(v0, v1, v2);
}

    
Triangle::~Triangle() {
}


double Triangle::area() const {
    return _area;
}


const Vector3& Triangle::normal() const {
    return _plane.normal();
}


const Plane& Triangle::plane() const {
    return _plane;
}


Vector3 Triangle::center() const {
    return (_vertex[0] + _vertex[1] + _vertex[2]) / 3.0;
}

Vector3 Triangle::randomPoint() const {
    // Choose a random point in the parallelogram

    float s = uniformRandom();
    float t = uniformRandom();

    if (t > 1.0f - s) {
        // Outside the triangle; reflect about the
        // diagonal of the parallelogram
        t = 1.0f - t;
        s = 1.0f - s;
    }

    return edge01 * s + edge02 * t + _vertex[0];
}


void Triangle::getBounds(AABox& out) const {
    Vector3 lo = _vertex[0];
    Vector3 hi = lo;

    for (int i = 1; i < 3; ++i) {
        lo = lo.min(_vertex[i]);
        hi = hi.max(_vertex[i]);
    }

    out = AABox(lo, hi);
}

} // G3D
