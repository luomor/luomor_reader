
#include "CATransformation.h"
#include <algorithm>
#include <math.h>

using namespace std;


NS_CC_BEGIN

CATransformation __CATransformationMake(float a, float b, float c, float d, float tx, float ty)
{
    CATransformation t;
    t.a = a;
    t.b = b;
    t.c = c;
    t.d = d;
    t.tx = tx;
    t.ty = ty;
    
    return t;
}

CCPoint __CCPointApplyAffineTransform(const CCPoint& point, const CATransformation& t)
{
    CCPoint p;
    p.x = (float)((double)t.a * point.x + (double)t.c * point.y + t.tx);
    p.y = (float)((double)t.b * point.x + (double)t.d * point.y + t.ty);
    
    return p;
}

CCSize __CCSizeApplyAffineTransform(const CCSize& size, const CATransformation& t)
{
    CCSize s;
    s.width = (float)((double)t.a * size.width + (double)t.c * size.height);
    s.height = (float)((double)t.b * size.width + (double)t.d * size.height);
    
    return s;
}


CATransformation CATransformationMakeIdentity()
{
    return __CATransformationMake(1.0, 0.0, 0.0, 1.0, 0.0, 0.0);
}

extern const CATransformation CATransformationIdentity = CATransformationMakeIdentity();


CCRect CCRectApplyAffineTransform(const CCRect& rect, const CATransformation& anAffineTransform)
{
    float top    = rect.getMinY();
    float left   = rect.getMinX();
    float right  = rect.getMaxX();
    float bottom = rect.getMaxY();
    
    CCPoint topLeft = CCPointApplyAffineTransform(CCPointMake(left, top), anAffineTransform);
    CCPoint topRight = CCPointApplyAffineTransform(CCPointMake(right, top), anAffineTransform);
    CCPoint bottomLeft = CCPointApplyAffineTransform(CCPointMake(left, bottom), anAffineTransform);
    CCPoint bottomRight = CCPointApplyAffineTransform(CCPointMake(right, bottom), anAffineTransform);

    float minX = min(min(topLeft.x, topRight.x), min(bottomLeft.x, bottomRight.x));
    float maxX = max(max(topLeft.x, topRight.x), max(bottomLeft.x, bottomRight.x));
    float minY = min(min(topLeft.y, topRight.y), min(bottomLeft.y, bottomRight.y));
    float maxY = max(max(topLeft.y, topRight.y), max(bottomLeft.y, bottomRight.y));
        
    return CCRectMake(minX, minY, (maxX - minX), (maxY - minY));
}

CATransformation CATransformationTranslate(const CATransformation& t, float tx, float ty)
{
    return __CATransformationMake(t.a,
                                   t.b,
                                   t.c,
                                   t.d,
                                   t.tx + t.a * tx + t.c * ty,
                                   t.ty + t.b * tx + t.d * ty);
}

CATransformation CATransformationScale(const CATransformation& t, float sx, float sy)
{
    return __CATransformationMake(t.a * sx, t.b * sx, t.c * sy, t.d * sy, t.tx, t.ty);
}

CATransformation CATransformationRotate(const CATransformation& t, float anAngle)
{
    float fSin = sin(anAngle);
    float fCos = cos(anAngle);

    return __CATransformationMake(    t.a * fCos + t.c * fSin,
                                    t.b * fCos + t.d * fSin,
                                    t.c * fCos - t.a * fSin,
                                    t.d * fCos - t.b * fSin,
                                    t.tx,
                                    t.ty);
}

CATransformation CATransformationConcat(const CATransformation& t1, const CATransformation& t2)
{
    return __CATransformationMake( t1.a * t2.a + t1.b * t2.c,
                                    t1.a * t2.b + t1.b * t2.d,                  //a,b
                                    t1.c * t2.a + t1.d * t2.c,
                                    t1.c * t2.b + t1.d * t2.d,                  //c,d
                                    t1.tx * t2.a + t1.ty * t2.c + t2.tx,        //tx
                                    t1.tx * t2.b + t1.ty * t2.d + t2.ty);       //ty
}


bool CATransformationEqualToTransform(const CATransformation& t1, const CATransformation& t2)
{
    return (t1.a == t2.a
            && t1.b == t2.b
            && t1.c == t2.c
            && t1.d == t2.d
            && t1.tx == t2.tx
            && t1.ty == t2.ty);
}

CATransformation CATransformationInvert(const CATransformation& t)
{
    float determinant = 1 / (t.a * t.d - t.b * t.c);

    return __CATransformationMake(determinant * t.d,
                                   -determinant * t.b,
                                   -determinant * t.c,
                                   determinant * t.a,
                                   determinant * (t.c * t.ty - t.d * t.tx),
                                   determinant * (t.b * t.tx - t.a * t.ty) );
}

NS_CC_END
