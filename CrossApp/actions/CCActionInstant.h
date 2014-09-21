

#ifndef __CCINSTANT_ACTION_H__
#define __CCINSTANT_ACTION_H__

#include <string>
#include "ccTypeInfo.h"
#include "CCAction.h"

NS_CC_BEGIN

/**
 * @addtogroup actions
 * @{
 */

/** 
@brief Instant actions are immediate actions. They don't have a duration like
the CCIntervalAction actions.
*/ 
class CC_DLL CCActionInstant : public CCFiniteTimeAction //<NSCopying>
{
public:
    /**
     *  @js ctor
     */
    CCActionInstant();
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCActionInstant(){}
    // CCAction methods
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone *pZone);
    virtual bool isDone(void);
    virtual void step(float dt);
    virtual void update(float time);
    //CCFiniteTimeAction method
    virtual CCFiniteTimeAction * reverse(void);
};

/** @brief Show the node
*/
class CC_DLL CCShow : public CCActionInstant
{
public:
    /**
     *  @js ctor
     *  @lua NA
     */
    CCShow(){}
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCShow(){}
    //super methods
    virtual void update(float time);
    virtual CCFiniteTimeAction * reverse(void);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone *pZone);
public:

    /** Allocates and initializes the action */
    static CCShow * create();
};



/** 
@brief Hide the node
*/
class CC_DLL CCHide : public CCActionInstant
{
public:
    /**
     *  @js ctor
     *  @lua NA
     */
    CCHide(){}
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCHide(){}
    //super methods
    /**
     *  @lua NA
     */
    virtual void update(float time);
    virtual CCFiniteTimeAction * reverse(void);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone *pZone);
public:

    /** Allocates and initializes the action */
    static CCHide * create();
};

/** @brief Toggles the visibility of a node
*/
class CC_DLL CCToggleVisibility : public CCActionInstant
{
public:
    /**
     *  @js ctor
     */
    CCToggleVisibility(){}
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCToggleVisibility(){}
    //super method
    virtual void update(float time);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone *pZone);
public:

    /** Allocates and initializes the action */
    static CCToggleVisibility * create();
};

/** 
 @brief Remove the node
 @js NA
 @lua NA
 */
class CC_DLL CCRemoveSelf : public CCActionInstant
{
public:
	CCRemoveSelf(){}
	virtual ~CCRemoveSelf(){}
	//super methods
	virtual void update(float time);
	virtual CCFiniteTimeAction * reverse(void);
	virtual CAObject* copyWithZone(CAZone *pZone);
public:
	/** create the action */
	static CCRemoveSelf * create(bool isNeedCleanUp = true);
	/** init the action */
	bool init(bool isNeedCleanUp);
protected:
	bool m_bIsNeedCleanUp;
};

/** 
@brief Flips the sprite horizontally
@since v0.99.0
@js NA
*/
class CC_DLL CCFlipX : public CCActionInstant
{
public:
    /**
     *  @js ctor
     */
    CCFlipX()
        :m_bFlipX(false)
    {}
    /**
     *  @js  NA
     *  @lua NA
     */
    virtual ~CCFlipX(){}

    /** create the action */
    static CCFlipX * create(bool x);

    /** init the action */
    bool initWithFlipX(bool x);
    //super methods
    virtual void update(float time);
    virtual CCFiniteTimeAction * reverse(void);
    /**
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone *pZone);

protected:
    bool    m_bFlipX;
};

/** 
@brief Flips the sprite vertically
@since v0.99.0
@js NA
*/
class CC_DLL CCFlipY : public CCActionInstant
{
public:
    /**
     *  @js ctor
     */
    CCFlipY()
        :m_bFlipY(false)
    {}
    /**
     *  @js  NA
     *  @lua NA
     */
    virtual ~CCFlipY(){}

    /** create the action */
    static CCFlipY * create(bool y);

    /** init the action */
    bool initWithFlipY(bool y);
    //super methods
    virtual void update(float time);
    virtual CCFiniteTimeAction * reverse(void);
    /**
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone *pZone);

protected:
    bool    m_bFlipY;
};

/** @brief Places the node in a certain position
*/
class CC_DLL CCPlace : public CCActionInstant //<NSCopying>
{
public:
    /**
     *  @js ctor
     */
    CCPlace(){}
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCPlace(){}

    /** creates a Place action with a position */
    static CCPlace * create(const CCPoint& pos);
    /** Initializes a Place action with a position */
    bool initWithPosition(const CCPoint& pos);
    //super methods
    virtual void update(float time);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone *pZone);
protected:
    CCPoint m_tPosition;
};

/** @brief Calls a 'callback'
*/
class CC_DLL CCCallFunc : public CCActionInstant //<NSCopying>
{
public:
    /**
     *  @js ctor
     */
    CCCallFunc()
        : m_pSelectorTarget(NULL)
        , m_pCallFunc(NULL)
    {
    }
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CCCallFunc();

    /** creates the action with the callback 

    * typedef void (CAObject::*SEL_CallFunc)();
    * @lua NA
    */
    static CCCallFunc * create(CAObject* pSelectorTarget, SEL_CallFunc selector);

	/** initializes the action with the callback 
    
    * typedef void (CAObject::*SEL_CallFunc)();
    * @lua NA
    */
    virtual bool initWithTarget(CAObject* pSelectorTarget);
    /** executes the callback 
     * @lua NA
     */
    virtual void execute();
    /** super methods
     * @lua NA
     */
    virtual void update(float time);
    /**
     * @js  NA
     * @lua NA
     */
    CAObject * copyWithZone(CAZone *pZone);
    /**
     * @lua NA
     */
    inline CAObject* getTargetCallback()
    {
        return m_pSelectorTarget;
    }
    /**
     * @lua NA
     */
    inline void setTargetCallback(CAObject* pSel)
    {
        if (pSel != m_pSelectorTarget)
        {
            CC_SAFE_RETAIN(pSel);
            CC_SAFE_RELEASE(m_pSelectorTarget);
            m_pSelectorTarget = pSel; 
        }
    }

protected:
    /** Target that will be called */
    CAObject*   m_pSelectorTarget;
    union
    {
        SEL_CallFunc    m_pCallFunc;
        SEL_CallFuncN    m_pCallFuncN;
        SEL_CallFuncND    m_pCallFuncND;
        SEL_CallFuncO   m_pCallFuncO;
    };
};

/** 
@brief Calls a 'callback' with the node as the first argument
N means Node
* @js NA
*/
class CC_DLL CCCallFuncN : public CCCallFunc, public TypeInfo
{
public:
    /**
     * @js ctor
     * @lua NA
     */
    CCCallFuncN(){}
    /**
     * @js  NA
     * @lua NA
     */
    virtual ~CCCallFuncN(){}
    /**
     * @lua NA
     */
    virtual long getClassTypeInfo() {
		static const long id = CrossApp::getHashCodeByString(typeid(CrossApp::CCCallFunc).name());
		return id;
    }

    /** creates the action with the callback 

     * typedef void (CAObject::*SEL_CallFuncN)(CCNode*);
     * @lua NA
     */
    static CCCallFuncN * create(CAObject* pSelectorTarget, SEL_CallFuncN selector);

    /** initializes the action with the callback 

     * typedef void (CAObject::*SEL_CallFuncN)(CCNode*);
     * @lua NA
     */
    virtual bool initWithTarget(CAObject* pSelectorTarget, SEL_CallFuncN selector);
    /** super methods
     * @js  NA
     * @lua NA
     */
    virtual CAObject* copyWithZone(CAZone *pZone);
    /**
     * @lua NA
     */
    virtual void execute();
};


/** 
* @brief Calls a 'callback' with the node as the first argument and the 2nd argument is data
* ND means: Node and Data. Data is void *, so it could be anything.
* @js NA
* @lua NA
*/
class CC_DLL CCCallFuncND : public CCCallFuncN
{
public:
    virtual long getClassTypeInfo() {
        static const long id = CrossApp::getHashCodeByString(typeid(CrossApp::CCCallFunc).name());
		return id;
    }

    /** creates the action with the callback and the data to pass as an argument */
    static CCCallFuncND * create(CAObject* pSelectorTarget, SEL_CallFuncND selector, void* d);

    /** initializes the action with the callback and the data to pass as an argument */
    virtual bool initWithTarget(CAObject* pSelectorTarget, SEL_CallFuncND selector, void* d);
    // super methods
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone *pZone);
    virtual void execute();

protected:
    void            *m_pData;
};


/**
@brief Calls a 'callback' with an object as the first argument.
O means Object.
@since v0.99.5
@js NA
@lua NA
*/

class CC_DLL CCCallFuncO : public CCCallFunc, public TypeInfo
{
public:
    CCCallFuncO();
    virtual ~CCCallFuncO();

    virtual long getClassTypeInfo() {
	    static const long id = CrossApp::getHashCodeByString(typeid(CrossApp::CCCallFunc).name());
		return id;
    }

    /** creates the action with the callback 

    typedef void (CAObject::*SEL_CallFuncO)(CAObject*);
    */
    static CCCallFuncO * create(CAObject* pSelectorTarget, SEL_CallFuncO selector, CAObject* pObject);

    /** initializes the action with the callback 

    typedef void (CAObject::*SEL_CallFuncO)(CAObject*);
    */
    virtual bool initWithTarget(CAObject* pSelectorTarget, SEL_CallFuncO selector, CAObject* pObject);
    // super methods
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone *pZone);
    virtual void execute();

    inline CAObject* getObject()
    {
        return m_pObject;
    }

    inline void setObject(CAObject* pObj)
    {
        if (pObj != m_pObject)
        {
            CC_SAFE_RELEASE(m_pObject);
            m_pObject = pObj;
            CC_SAFE_RETAIN(m_pObject);
        }
    }

protected:
    /** object to be passed as argument */
    CAObject* m_pObject;
};

// end of actions group
/// @}

NS_CC_END

#endif //__CCINSTANT_ACTION_H__
