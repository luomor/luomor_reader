

#ifndef __CAObject_H__
#define __CAObject_H__

#include "platform/CCPlatformMacros.h"

#ifdef EMSCRIPTEN
#include <GLES2/gl2.h>
#endif // EMSCRIPTEN

NS_CC_BEGIN

#define kCAObjectTagInvalid -1

class CAZone;
class CAObject;
class CAEvent;
class CAView;

class CC_DLL CACopying
{
public:
    virtual CAObject* copyWithZone(CAZone* pZone);
};

class CC_DLL CAObject : public CACopying
{
public:

    unsigned int        m_uID;

protected:

    unsigned int        m_uReference;

    unsigned int        m_uAutoReleaseCount;
    
public:
    
    CAObject(void);

    virtual ~CAObject(void);
    
    void release(void);
    
    CAObject* retain(void);
    
    CAObject* autorelease(void);
    
    CAObject* copy(void);
    
    bool isSingleReference(void) const;
    
    unsigned int retainCount(void) const;
    
    virtual bool isEqual(const CAObject* pObject);

    virtual void update(float dt) {CC_UNUSED_PARAM(dt);};
    
    CC_SYNTHESIZE(int, m_nTag, Tag);
    
    friend class CAAutoreleasePool;
};

class CC_DLL CAZone
{
public:
    
    CAZone(CAObject *pObject = NULL);
    
public:
    
    CAObject *m_pCopyObject;
};

typedef void (CAObject::*SEL_SCHEDULE)(float);
typedef void (CAObject::*SEL_CallFunc)();
typedef void (CAObject::*SEL_CallFuncN)(CAView*);
typedef void (CAObject::*SEL_CallFuncND)(CAView*, void*);
typedef void (CAObject::*SEL_CallFuncO)(CAObject*);
typedef void (CAObject::*SEL_MenuHandler)(CAObject*);
typedef void (CAObject::*SEL_EventHandler)(CAEvent*);
typedef int (CAObject::*SEL_Compare)(CAObject*);

#define schedule_selector(_SELECTOR) (SEL_SCHEDULE)(&_SELECTOR)
#define callfunc_selector(_SELECTOR) (SEL_CallFunc)(&_SELECTOR)
#define callfuncN_selector(_SELECTOR) (SEL_CallFuncN)(&_SELECTOR)
#define callfuncND_selector(_SELECTOR) (SEL_CallFuncND)(&_SELECTOR)
#define callfuncO_selector(_SELECTOR) (SEL_CallFuncO)(&_SELECTOR)
#define menu_selector(_SELECTOR) (SEL_MenuHandler)(&_SELECTOR)
#define event_selector(_SELECTOR) (SEL_EventHandler)(&_SELECTOR)
#define compare_selector(_SELECTOR) (SEL_Compare)(&_SELECTOR)

NS_CC_END

#endif // __CAObject_H__
