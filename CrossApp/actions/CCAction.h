
#ifndef __ACTIONS_CCACTION_H__
#define __ACTIONS_CCACTION_H__

#include "basics/CAObject.h"
#include "basics/CAGeometry.h"
#include "platform/CCPlatformMacros.h"
#include "view/CAView.h"
NS_CC_BEGIN

enum {
    //! Default tag
    kCCActionTagInvalid = -1,
};

/**
 * @addtogroup actions
 * @{
 */

/** 
@brief Base class for CCAction objects.
 */
class CC_DLL CCAction : public CAObject 
{
public:
    /**
     * @js ctor
     */
    CCAction(void);
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CCAction(void);
    /**
     * @js NA
     * @lua NA
     */
    const char* description();
    /**
     * @js NA
     * @lua NA
     */
    virtual CAObject* copyWithZone(CAZone *pZone);

    //! return true if the action has finished
    virtual bool isDone(void);

    //! called before the action start. It will also set the target.
    virtual void startWithTarget(CAView *pTarget);

    /** 
    called after the action has finished. It will set the 'target' to nil.
    IMPORTANT: You should never call "[action stop]" manually. Instead, use: "target->stopAction(action);"
    */
    virtual void stop(void);

    //! called every frame with it's delta time. DON'T override unless you know what you are doing.
    virtual void step(float dt);

    /** 
    called once per frame. time a value between 0 and 1

    For example: 
    - 0 means that the action just started
    - 0.5 means that the action is in the middle
    - 1 means that the action is over
    */
    virtual void update(float time);
    
    inline CAView* getTarget(void) { return m_pTarget; }
    /** The action will modify the target properties. */
    inline void setTarget(CAView *pTarget) { m_pTarget = pTarget; }
    
    inline CAView* getOriginalTarget(void) { return m_pOriginalTarget; } 
    /** Set the original target, since target can be nil.
    Is the target that were used to run the action. Unless you are doing something complex, like CCActionManager, you should NOT call this method.
    The target is 'assigned', it is not 'retained'.
    @since v0.8.2
    */
    inline void setOriginalTarget(CAView *pOriginalTarget) { m_pOriginalTarget = pOriginalTarget; }

    inline int getTag(void) { return m_nTag; }
    inline void setTag(int nTag) { m_nTag = nTag; }

public:
    /** Create an action */
    static CCAction* create();
protected:
    CAView    *m_pOriginalTarget;
    /** The "target".
    The target will be set with the 'startWithTarget' method.
    When the 'stop' method is called, target will be set to nil.
    The target is 'assigned', it is not 'retained'.
    */
    CAView    *m_pTarget;
    /** The action tag. An identifier of the action */
    int     m_nTag;
};

/** 
@brief 
 Base class actions that do have a finite time duration.
 Possible actions:
   - An action with a duration of 0 seconds
   - An action with a duration of 35.5 seconds

 Infinite time actions are valid
 */
class CC_DLL CCFiniteTimeAction : public CCAction
{
public:
    /**
     *  @js ctor
     */
    CCFiniteTimeAction()
        : m_fDuration(0)
    {}
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCFiniteTimeAction(){}
    //! get duration in seconds of the action
    inline float getDuration(void) { return m_fDuration; }
    //! set duration in seconds of the action
    inline void setDuration(float duration) { m_fDuration = duration; }

    /** returns a reversed action */
    virtual CCFiniteTimeAction* reverse(void);
protected:
    //! duration in seconds
    float m_fDuration;
};

class CCActionInterval;
class CCRepeatForever;

/** 
 @brief Changes the speed of an action, making it take longer (speed>1)
 or less (speed<1) time.
 Useful to simulate 'slow motion' or 'fast forward' effect.
 @warning This action can't be Sequenceable because it is not an CCIntervalAction
 */
class CC_DLL CCSpeed : public CCAction
{
public:
    /**
     *  @js ctor
     */
    CCSpeed()
        : m_fSpeed(0.0)
        , m_pInnerAction(NULL)
    {}
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCSpeed(void);

    inline float getSpeed(void) { return m_fSpeed; }
    /** alter the speed of the inner function in runtime */
    inline void setSpeed(float fSpeed) { m_fSpeed = fSpeed; }

    /** initializes the action */
    bool initWithAction(CCActionInterval *pAction, float fSpeed);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CAObject* copyWithZone(CAZone *pZone);
    virtual void startWithTarget(CAView* pTarget);
    virtual void stop();
    virtual void step(float dt);
    virtual bool isDone(void);
    virtual CCActionInterval* reverse(void);

    void setInnerAction(CCActionInterval *pAction);

    inline CCActionInterval* getInnerAction()
    {
        return m_pInnerAction;
    }

public:
    /** create the action */
    static CCSpeed* create(CCActionInterval* pAction, float fSpeed);
protected:
    float m_fSpeed;
    CCActionInterval *m_pInnerAction;
};


// end of actions group
/// @}

NS_CC_END

#endif // __ACTIONS_CCACTION_H__
