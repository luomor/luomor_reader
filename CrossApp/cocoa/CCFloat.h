

#ifndef __CCFLOAT_H__
#define __CCFLOAT_H__

#include "basics/CAObject.h"

NS_CC_BEGIN

/**
 * @addtogroup data_structures
 * @{
 * @js NA
 * @lua NA
 */

class CC_DLL CCFloat : public CAObject
{
public:
    CCFloat(float v)
        : m_fValue(v) {}
    float getValue() const {return m_fValue;}

    static CCFloat* create(float v)
    {
        CCFloat* pRet = new CCFloat(v);
        if (pRet)
        {
            pRet->autorelease();
        }
        return pRet;
    }

    /* override functions */
    virtual void acceptVisitor(CCDataVisitor &visitor) { visitor.visit(this); }

private:
    float m_fValue;
};

// end of data_structure group
/// @}

NS_CC_END

#endif /* __CCFLOAT_H__ */
