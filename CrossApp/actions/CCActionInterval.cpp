

#include "CCActionInterval.h"
#include "view/CAImageView.h"
#include "view/CAView.h"
#include "support/CCPointExtension.h"
#include "CCStdC.h"
#include "CCActionInstant.h"
#include <stdarg.h>

NS_CC_BEGIN

// Extra action for making a CCSequence or CCSpawn when only adding one action to it.
class ExtraAction : public CCFiniteTimeAction
{
public:
    static ExtraAction* create();
    virtual CAObject* copyWithZone(CAZone* pZone);
    virtual ExtraAction* reverse(void);
    virtual void update(float time);
    virtual void step(float dt);
};

ExtraAction* ExtraAction::create()
{
    ExtraAction* pRet = new ExtraAction();
    if (pRet)
    {
        pRet->autorelease();
    }
    return pRet;
}

CAObject* ExtraAction::copyWithZone(CAZone* pZone)
{
    CC_UNUSED_PARAM(pZone);
    ExtraAction* pRet = new ExtraAction();
    return pRet;
}

ExtraAction* ExtraAction::reverse(void)
{
    return ExtraAction::create();
}

void ExtraAction::update(float time)
{
    CC_UNUSED_PARAM(time);
}

void ExtraAction::step(float dt)
{
    CC_UNUSED_PARAM(dt);
}

//
// IntervalAction
//
CCActionInterval* CCActionInterval::create(float d)
{
    CCActionInterval *pAction = new CCActionInterval();
    pAction->initWithDuration(d);
    pAction->autorelease();

    return pAction;
}

bool CCActionInterval::initWithDuration(float d)
{
    m_fDuration = d;

    // prevent division by 0
    // This comparison could be in step:, but it might decrease the performance
    // by 3% in heavy based action games.
    if (m_fDuration == 0)
    {
        m_fDuration = FLT_EPSILON;
    }

    m_elapsed = 0;
    m_bFirstTick = true;

    return true;
}

CAObject* CCActionInterval::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCActionInterval* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCActionInterval*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCActionInterval();
        pZone = pNewZone = new CAZone(pCopy);
    }

    
    CCFiniteTimeAction::copyWithZone(pZone);

    CC_SAFE_DELETE(pNewZone);

    pCopy->initWithDuration(m_fDuration);

    return pCopy;
}

bool CCActionInterval::isDone(void)
{
    return m_elapsed >= m_fDuration;
}

void CCActionInterval::step(float dt)
{
    if (m_bFirstTick)
    {
        m_bFirstTick = false;
        m_elapsed = 0;
    }
    else
    {
        m_elapsed += dt;
    }
    
    this->update(MAX (0,                                  // needed for rewind. elapsed could be negative
                      MIN(1, m_elapsed /
                          MAX(m_fDuration, FLT_EPSILON)   // division by 0
                          )
                      )
                 );
}

void CCActionInterval::setAmplitudeRate(float amp)
{
    CC_UNUSED_PARAM(amp);
    // Abstract class needs implementation
    CCAssert(0, "");
}

float CCActionInterval::getAmplitudeRate(void)
{
    // Abstract class needs implementation
    CCAssert(0, "");

    return 0;
}

void CCActionInterval::startWithTarget(CAView *pTarget)
{
    CCFiniteTimeAction::startWithTarget(pTarget);
    m_elapsed = 0.0f;
    m_bFirstTick = true;
}

CCActionInterval* CCActionInterval::reverse(void)
{
    CCAssert(false, "CCIntervalAction: reverse not implemented.");
    return NULL;
}

//
// Sequence
//

CCSequence* CCSequence::createWithTwoActions(CCFiniteTimeAction *pActionOne, CCFiniteTimeAction *pActionTwo)
{
    CCSequence *pSequence = new CCSequence();
    pSequence->initWithTwoActions(pActionOne, pActionTwo);
    pSequence->autorelease();

    return pSequence;
}

CCSequence* CCSequence::create(CCFiniteTimeAction *pAction1, ...)
{
    va_list params;
    va_start(params, pAction1);

    CCSequence *pRet = CCSequence::createWithVariableList(pAction1, params);

    va_end(params);
    
    return pRet;
}

CCSequence* CCSequence::createWithVariableList(CCFiniteTimeAction *pAction1, va_list args)
{
    CCFiniteTimeAction *pNow;
    CCFiniteTimeAction *pPrev = pAction1;
    bool bOneAction = true;

    while (pAction1)
    {
        pNow = va_arg(args, CCFiniteTimeAction*);
        if (pNow)
        {
            pPrev = createWithTwoActions(pPrev, pNow);
            bOneAction = false;
        }
        else
        {
            // If only one action is added to CCSequence, make up a CCSequence by adding a simplest finite time action.
            if (bOneAction)
            {
                pPrev = createWithTwoActions(pPrev, ExtraAction::create());
            }
            break;
        }
    }
    
    return ((CCSequence*)pPrev);
}

CCSequence* CCSequence::create(CCArray* arrayOfActions)
{
    CCSequence* pRet = NULL;
    do 
    {
        unsigned  int count = arrayOfActions->count();
        CC_BREAK_IF(count == 0);

        CCFiniteTimeAction* prev = (CCFiniteTimeAction*)arrayOfActions->objectAtIndex(0);

        if (count > 1)
        {
            for (unsigned int i = 1; i < count; ++i)
            {
                prev = createWithTwoActions(prev, (CCFiniteTimeAction*)arrayOfActions->objectAtIndex(i));
            }
        }
        else
        {
            // If only one action is added to CCSequence, make up a CCSequence by adding a simplest finite time action.
            prev = createWithTwoActions(prev, ExtraAction::create());
        }
        pRet = (CCSequence*)prev;
    }while (0);
    return pRet;
}

bool CCSequence::initWithTwoActions(CCFiniteTimeAction *pActionOne, CCFiniteTimeAction *pActionTwo)
{
    CCAssert(pActionOne != NULL, "");
    CCAssert(pActionTwo != NULL, "");

    float d = pActionOne->getDuration() + pActionTwo->getDuration();
    CCActionInterval::initWithDuration(d);

    m_pActions[0] = pActionOne;
    pActionOne->retain();

    m_pActions[1] = pActionTwo;
    pActionTwo->retain();

    return true;
}

CAObject* CCSequence::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCSequence* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCSequence*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCSequence();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithTwoActions((CCFiniteTimeAction*)(m_pActions[0]->copy()->autorelease()), 
                (CCFiniteTimeAction*)(m_pActions[1]->copy()->autorelease()));

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCSequence::~CCSequence(void)
{
    CC_SAFE_RELEASE(m_pActions[0]);
    CC_SAFE_RELEASE(m_pActions[1]);
}

void CCSequence::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_split = m_pActions[0]->getDuration() / m_fDuration;
    m_last = -1;
}

void CCSequence::stop(void)
{
    // Issue #1305
    if( m_last != - 1)
    {
        m_pActions[m_last]->stop();
    }

    CCActionInterval::stop();
}

void CCSequence::update(float t)
{
    int found = 0;
    float new_t = 0.0f;

    if( t < m_split ) {
        // action[0]
        found = 0;
        if( m_split != 0 )
            new_t = t / m_split;
        else
            new_t = 1;

    } else {
        // action[1]
        found = 1;
        if ( m_split == 1 )
            new_t = 1;
        else
            new_t = (t-m_split) / (1 - m_split );
    }

    if ( found==1 ) {

        if( m_last == -1 ) {
            // action[0] was skipped, execute it.
            m_pActions[0]->startWithTarget(m_pTarget);
            m_pActions[0]->update(1.0f);
            m_pActions[0]->stop();
        }
        else if( m_last == 0 )
        {
            // switching to action 1. stop action 0.
            m_pActions[0]->update(1.0f);
            m_pActions[0]->stop();
        }
    }
	else if(found==0 && m_last==1 )
	{
		// Reverse mode ?
		// XXX: Bug. this case doesn't contemplate when _last==-1, found=0 and in "reverse mode"
		// since it will require a hack to know if an action is on reverse mode or not.
		// "step" should be overriden, and the "reverseMode" value propagated to inner Sequences.
		m_pActions[1]->update(0);
		m_pActions[1]->stop();
	}
    // Last action found and it is done.
    if( found == m_last && m_pActions[found]->isDone() )
    {
        return;
    }

    // Last action found and it is done
    if( found != m_last )
    {
        m_pActions[found]->startWithTarget(m_pTarget);
    }

    m_pActions[found]->update(new_t);
    m_last = found;
}

CCActionInterval* CCSequence::reverse(void)
{
    return CCSequence::createWithTwoActions(m_pActions[1]->reverse(), m_pActions[0]->reverse());
}

//
// Repeat
//

CCRepeat* CCRepeat::create(CCFiniteTimeAction *pAction, unsigned int times)
{
    CCRepeat* pRepeat = new CCRepeat();
    pRepeat->initWithAction(pAction, times);
    pRepeat->autorelease();

    return pRepeat;
}

bool CCRepeat::initWithAction(CCFiniteTimeAction *pAction, unsigned int times)
{
    float d = pAction->getDuration() * times;

    if (CCActionInterval::initWithDuration(d))
    {
        m_uTimes = times;
        m_pInnerAction = pAction;
        pAction->retain();

        m_bActionInstant = dynamic_cast<CCActionInstant*>(pAction) ? true : false;
        //an instant action needs to be executed one time less in the update method since it uses startWithTarget to execute the action
        if (m_bActionInstant) 
        {
            m_uTimes -=1;
        }
        m_uTotal = 0;

        return true;
    }

    return false;
}

CAObject* CCRepeat::copyWithZone(CAZone *pZone)
{
    
    CAZone* pNewZone = NULL;
    CCRepeat* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCRepeat*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCRepeat();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithAction((CCFiniteTimeAction*)(m_pInnerAction->copy()->autorelease()), m_uTimes);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCRepeat::~CCRepeat(void)
{
    CC_SAFE_RELEASE(m_pInnerAction);
}

void CCRepeat::startWithTarget(CAView *pTarget)
{
    m_uTotal = 0;
    m_fNextDt = m_pInnerAction->getDuration()/m_fDuration;
    CCActionInterval::startWithTarget(pTarget);
    m_pInnerAction->startWithTarget(pTarget);
}

void CCRepeat::stop(void)
{
    m_pInnerAction->stop();
    CCActionInterval::stop();
}

// issue #80. Instead of hooking step:, hook update: since it can be called by any 
// container action like CCRepeat, CCSequence, CCEase, etc..
void CCRepeat::update(float dt)
{
    if (dt >= m_fNextDt)
    {
        while (dt > m_fNextDt && m_uTotal < m_uTimes)
        {

            m_pInnerAction->update(1.0f);
            m_uTotal++;

            m_pInnerAction->stop();
            m_pInnerAction->startWithTarget(m_pTarget);
            m_fNextDt += m_pInnerAction->getDuration()/m_fDuration;
        }

        // fix for issue #1288, incorrect end value of repeat
        if(dt >= 1.0f && m_uTotal < m_uTimes) 
        {
            m_uTotal++;
        }

        // don't set an instant action back or update it, it has no use because it has no duration
        if (!m_bActionInstant)
        {
            if (m_uTotal == m_uTimes)
            {
                m_pInnerAction->update(1);
                m_pInnerAction->stop();
            }
            else
            {
                // issue #390 prevent jerk, use right update
                m_pInnerAction->update(dt - (m_fNextDt - m_pInnerAction->getDuration()/m_fDuration));
            }
        }
    }
    else
    {
        m_pInnerAction->update(fmodf(dt * m_uTimes,1.0f));
    }
}

bool CCRepeat::isDone(void)
{
    return m_uTotal == m_uTimes;
}

CCActionInterval* CCRepeat::reverse(void)
{
    return CCRepeat::create(m_pInnerAction->reverse(), m_uTimes);
}

//
// RepeatForever
//
CCRepeatForever::~CCRepeatForever()
{
    CC_SAFE_RELEASE(m_pInnerAction);
}

CCRepeatForever *CCRepeatForever::create(CCActionInterval *pAction)
{
    CCRepeatForever *pRet = new CCRepeatForever();
    if (pRet && pRet->initWithAction(pAction))
    {
        pRet->autorelease();
        return pRet;
    }
    CC_SAFE_DELETE(pRet);
    return NULL;
}

bool CCRepeatForever::initWithAction(CCActionInterval *pAction)
{
    CCAssert(pAction != NULL, "");
    pAction->retain();
    m_pInnerAction = pAction;
    return true;
}
CAObject* CCRepeatForever::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCRepeatForever* pRet = NULL;
    if(pZone && pZone->m_pCopyObject) //in case of being called at sub class
    {
        pRet = (CCRepeatForever*)(pZone->m_pCopyObject);
    }
    else
    {
        pRet = new CCRepeatForever();
        pZone = pNewZone = new CAZone(pRet);
    }
    CCActionInterval::copyWithZone(pZone);
    // win32 : use the m_pOther's copy object.
    pRet->initWithAction((CCActionInterval*)(m_pInnerAction->copy()->autorelease())); 
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

void CCRepeatForever::startWithTarget(CAView* pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_pInnerAction->startWithTarget(pTarget);
}

void CCRepeatForever::step(float dt)
{
    m_pInnerAction->step(dt);
    if (m_pInnerAction->isDone())
    {
        float diff = m_pInnerAction->getElapsed() - m_pInnerAction->getDuration();
        m_pInnerAction->startWithTarget(m_pTarget);
        // to prevent jerk. issue #390, 1247
        m_pInnerAction->step(0.0f);
        m_pInnerAction->step(diff);
    }
}

bool CCRepeatForever::isDone()
{
    return false;
}

CCActionInterval *CCRepeatForever::reverse()
{
    return (CCActionInterval*)(CCRepeatForever::create(m_pInnerAction->reverse()));
}

//
// Spawn
//

CCSpawn* CCSpawn::create(CCFiniteTimeAction *pAction1, ...)
{
    va_list params;
    va_start(params, pAction1);

    CCSpawn *pRet = CCSpawn::createWithVariableList(pAction1, params);

    va_end(params);
    
    return pRet;
}

CCSpawn* CCSpawn::createWithVariableList(CCFiniteTimeAction *pAction1, va_list args)
{
    CCFiniteTimeAction *pNow;
    CCFiniteTimeAction *pPrev = pAction1;
    bool bOneAction = true;

    while (pAction1)
    {
        pNow = va_arg(args, CCFiniteTimeAction*);
        if (pNow)
        {
            pPrev = createWithTwoActions(pPrev, pNow);
            bOneAction = false;
        }
        else
        {
            // If only one action is added to CCSpawn, make up a CCSpawn by adding a simplest finite time action.
            if (bOneAction)
            {
                pPrev = createWithTwoActions(pPrev, ExtraAction::create());
            }
            break;
        }
    }

    return ((CCSpawn*)pPrev);
}

CCSpawn* CCSpawn::create(CCArray *arrayOfActions)
{
    CCSpawn* pRet = NULL;
    do 
    {
        unsigned  int count = arrayOfActions->count();
        CC_BREAK_IF(count == 0);
        CCFiniteTimeAction* prev = (CCFiniteTimeAction*)arrayOfActions->objectAtIndex(0);
        if (count > 1)
        {
            for (unsigned int i = 1; i < arrayOfActions->count(); ++i)
            {
                prev = createWithTwoActions(prev, (CCFiniteTimeAction*)arrayOfActions->objectAtIndex(i));
            }
        }
        else
        {
            // If only one action is added to CCSpawn, make up a CCSpawn by adding a simplest finite time action.
            prev = createWithTwoActions(prev, ExtraAction::create());
        }
        pRet = (CCSpawn*)prev;
    }while (0);

    return pRet;
}

CCSpawn* CCSpawn::createWithTwoActions(CCFiniteTimeAction *pAction1, CCFiniteTimeAction *pAction2)
{
    CCSpawn *pSpawn = new CCSpawn();
    pSpawn->initWithTwoActions(pAction1, pAction2);
    pSpawn->autorelease();

    return pSpawn;
}

bool CCSpawn:: initWithTwoActions(CCFiniteTimeAction *pAction1, CCFiniteTimeAction *pAction2)
{
    CCAssert(pAction1 != NULL, "");
    CCAssert(pAction2 != NULL, "");

    bool bRet = false;

    float d1 = pAction1->getDuration();
    float d2 = pAction2->getDuration();

    if (CCActionInterval::initWithDuration(MAX(d1, d2)))
    {
        m_pOne = pAction1;
        m_pTwo = pAction2;

        if (d1 > d2)
        {
            m_pTwo = CCSequence::createWithTwoActions(pAction2, CCDelayTime::create(d1 - d2));
        } 
        else if (d1 < d2)
        {
            m_pOne = CCSequence::createWithTwoActions(pAction1, CCDelayTime::create(d2 - d1));
        }

        m_pOne->retain();
        m_pTwo->retain();

        bRet = true;
    }

    
    return bRet;
}

CAObject* CCSpawn::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCSpawn* pCopy = NULL;

    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCSpawn*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCSpawn();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithTwoActions((CCFiniteTimeAction*)(m_pOne->copy()->autorelease()), 
                    (CCFiniteTimeAction*)(m_pTwo->copy()->autorelease()));

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCSpawn::~CCSpawn(void)
{
    CC_SAFE_RELEASE(m_pOne);
    CC_SAFE_RELEASE(m_pTwo);
}

void CCSpawn::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_pOne->startWithTarget(pTarget);
    m_pTwo->startWithTarget(pTarget);
}

void CCSpawn::stop(void)
{
    m_pOne->stop();
    m_pTwo->stop();
    CCActionInterval::stop();
}

void CCSpawn::update(float time)
{
    if (m_pOne)
    {
        m_pOne->update(time);
    }
    if (m_pTwo)
    {
        m_pTwo->update(time);
    }
}

CCActionInterval* CCSpawn::reverse(void)
{
    return CCSpawn::createWithTwoActions(m_pOne->reverse(), m_pTwo->reverse());
}

//
// RotateTo
//

CCRotateTo* CCRotateTo::create(float fDuration, float fDeltaAngle)
{
    CCRotateTo* pRotateTo = new CCRotateTo();
    pRotateTo->initWithDuration(fDuration, fDeltaAngle);
    pRotateTo->autorelease();

    return pRotateTo;
}

bool CCRotateTo::initWithDuration(float fDuration, float fDeltaAngle)
{
    if (CCActionInterval::initWithDuration(fDuration))
    {
        m_fDstAngleX = m_fDstAngleY = fDeltaAngle;
        return true;
    }

    return false;
}

CCRotateTo* CCRotateTo::create(float fDuration, float fDeltaAngleX, float fDeltaAngleY)
{
    CCRotateTo* pRotateTo = new CCRotateTo();
    pRotateTo->initWithDuration(fDuration, fDeltaAngleX, fDeltaAngleY);
    pRotateTo->autorelease();
    
    return pRotateTo;
}

bool CCRotateTo::initWithDuration(float fDuration, float fDeltaAngleX, float fDeltaAngleY)
{
    if (CCActionInterval::initWithDuration(fDuration))
    {
        m_fDstAngleX = fDeltaAngleX;
        m_fDstAngleY = fDeltaAngleY;
        
        return true;
    }
    
    return false;
}

CAObject* CCRotateTo::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCRotateTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (CCRotateTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCRotateTo();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_fDstAngleX, m_fDstAngleY);

    //Action *copy = [[[self class] allocWithZone: zone] initWithDuration:[self duration] angle: angle];
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCRotateTo::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    
    // Calculate X
    m_fStartAngleX = pTarget->getRotationX();
    if (m_fStartAngleX > 0)
    {
        m_fStartAngleX = fmodf(m_fStartAngleX, 360.0f);
    }
    else
    {
        m_fStartAngleX = fmodf(m_fStartAngleX, -360.0f);
    }

    m_fDiffAngleX = m_fDstAngleX - m_fStartAngleX;
    if (m_fDiffAngleX > 180)
    {
        m_fDiffAngleX -= 360;
    }
    if (m_fDiffAngleX < -180)
    {
        m_fDiffAngleX += 360;
    }
    
    //Calculate Y: It's duplicated from calculating X since the rotation wrap should be the same
    m_fStartAngleY = m_pTarget->getRotationY();

    if (m_fStartAngleY > 0)
    {
        m_fStartAngleY = fmodf(m_fStartAngleY, 360.0f);
    }
    else
    {
        m_fStartAngleY = fmodf(m_fStartAngleY, -360.0f);
    }

    m_fDiffAngleY = m_fDstAngleY - m_fStartAngleY;
    if (m_fDiffAngleY > 180)
    {
        m_fDiffAngleY -= 360;
    }

    if (m_fDiffAngleY < -180)
    {
        m_fDiffAngleY += 360;
    }
}

void CCRotateTo::update(float time)
{
    if (m_pTarget)
    {
        m_pTarget->setRotationX(m_fStartAngleX + m_fDiffAngleX * time);
        m_pTarget->setRotationY(m_fStartAngleY + m_fDiffAngleY * time);
    }
}

//
// RotateBy
//

CCRotateBy* CCRotateBy::create(float fDuration, float fDeltaAngle)
{
    CCRotateBy *pRotateBy = new CCRotateBy();
    pRotateBy->initWithDuration(fDuration, fDeltaAngle);
    pRotateBy->autorelease();

    return pRotateBy;
}

bool CCRotateBy::initWithDuration(float fDuration, float fDeltaAngle)
{
    if (CCActionInterval::initWithDuration(fDuration))
    {
        m_fAngleX = m_fAngleY = fDeltaAngle;
        return true;
    }

    return false;
}

CCRotateBy* CCRotateBy::create(float fDuration, float fDeltaAngleX, float fDeltaAngleY)
{
    CCRotateBy *pRotateBy = new CCRotateBy();
    pRotateBy->initWithDuration(fDuration, fDeltaAngleX, fDeltaAngleY);
    pRotateBy->autorelease();
    
    return pRotateBy;
}

bool CCRotateBy::initWithDuration(float fDuration, float fDeltaAngleX, float fDeltaAngleY)
{
    if (CCActionInterval::initWithDuration(fDuration))
    {
        m_fAngleX = fDeltaAngleX;
        m_fAngleY = fDeltaAngleY;
        return true;
    }
    
    return false;
}

CAObject* CCRotateBy::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCRotateBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCRotateBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCRotateBy();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_fAngleX, m_fAngleY);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCRotateBy::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_fStartAngleX = pTarget->getRotationX();
    m_fStartAngleY = pTarget->getRotationY();
}

void CCRotateBy::update(float time)
{
    // XXX: shall I add % 360
    if (m_pTarget)
    {
        m_pTarget->setRotationX(m_fStartAngleX + m_fAngleX * time);
        m_pTarget->setRotationY(m_fStartAngleY + m_fAngleY * time);
    }
}

CCActionInterval* CCRotateBy::reverse(void)
{
    return CCRotateBy::create(m_fDuration, -m_fAngleX, -m_fAngleY);
}



//
// FrameOrginTo
//

CCFrameOrginTo* CCFrameOrginTo::create(float duration, const CCPoint& endFrameOrgin)
{
    CCFrameOrginTo *pRet = new CCFrameOrginTo();
    pRet->initWithDuration(duration, endFrameOrgin);
    pRet->autorelease();
    
    return pRet;
}

bool CCFrameOrginTo::initWithDuration(float duration, const CCPoint& endFrameOrgin)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_endFrameOrgin = endFrameOrgin;
        return true;
    }
    
    return false;
}

CAObject* CCFrameOrginTo::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCFrameOrginTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (CCFrameOrginTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCFrameOrginTo();
        pZone = pNewZone = new CAZone(pCopy);
    }
    
    CCActionInterval::copyWithZone(pZone);
    
    pCopy->initWithDuration(m_fDuration, m_endFrameOrgin);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCFrameOrginTo::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_startFrameOrgin = pTarget->getFrameOrigin();
    m_deltaFrameOrgin = ccpSub(m_endFrameOrgin, m_startFrameOrgin);
}

CCActionInterval* CCFrameOrginTo::reverse(void)
{
    return CCFrameOrginTo::create(m_fDuration, m_startFrameOrgin);
}

void CCFrameOrginTo::update(float t)
{
    if (m_pTarget)
    {
#if CC_ENABLE_STACKABLE_ACTIONS
        CCPoint frameOrgin = m_deltaFrameOrgin;
        frameOrgin = ccpMult(frameOrgin, t);
        frameOrgin = ccpAdd(m_startFrameOrgin, frameOrgin);
        m_pTarget->setFrameOrigin(frameOrgin);
#else
        m_pTarget->setFrameOrgin(m_endFrameOrgin);
#endif // CC_ENABLE_STACKABLE_ACTIONS
    }
}

//
// FrameTo
//

CCFrameTo* CCFrameTo::create(float duration, const CCRect& endFrame)
{
    CCFrameTo *pRet = new CCFrameTo();
    pRet->initWithDuration(duration, endFrame);
    pRet->autorelease();
    
    return pRet;
}

bool CCFrameTo::initWithDuration(float duration, const CCRect& endFrame)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_endFrame = endFrame;
        return true;
    }
    
    return false;
}

CAObject* CCFrameTo::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCFrameTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (CCFrameTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCFrameTo();
        pZone = pNewZone = new CAZone(pCopy);
    }
    
    CCActionInterval::copyWithZone(pZone);
    
    pCopy->initWithDuration(m_fDuration, m_endFrame);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCFrameTo::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_startFrame = pTarget->getFrame();
    m_deltaFrame.origin = ccpSub(m_endFrame.origin, m_startFrame.origin);
    m_deltaFrame.size = ccpSub(m_endFrame.size, m_startFrame.size);
}

CCActionInterval* CCFrameTo::reverse(void)
{
    return CCFrameTo::create(m_fDuration, m_startFrame);
}

void CCFrameTo::update(float t)
{
    if (m_pTarget)
    {
#if CC_ENABLE_STACKABLE_ACTIONS
        CCRect frame = m_deltaFrame;
        frame.origin = ccpMult(frame.origin, t);
        frame.size = ccpMult(frame.size, t);
        frame.origin = ccpAdd(m_startFrame.origin, frame.origin);
        frame.size = ccpAdd(m_startFrame.size, frame.size);
        m_pTarget->setFrame(frame);
#else
        m_pTarget->setFrame(m_endFrame);
#endif // CC_ENABLE_STACKABLE_ACTIONS
    }
}

//
// CenterOrginTo
//

CCCenterOrginTo* CCCenterOrginTo::create(float duration, const CCPoint& endCenterOrgin)
{
    CCCenterOrginTo *pRet = new CCCenterOrginTo();
    pRet->initWithDuration(duration, endCenterOrgin);
    pRet->autorelease();
    
    return pRet;
}

bool CCCenterOrginTo::initWithDuration(float duration, const CCPoint& endCenterOrgin)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_endCenterOrgin = endCenterOrgin;
        return true;
    }
    
    return false;
}

CAObject* CCCenterOrginTo::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCCenterOrginTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (CCCenterOrginTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCCenterOrginTo();
        pZone = pNewZone = new CAZone(pCopy);
    }
    
    CCActionInterval::copyWithZone(pZone);
    
    pCopy->initWithDuration(m_fDuration, m_endCenterOrgin);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCCenterOrginTo::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_startCenterOrgin = pTarget->getCenterOrigin();
    m_deltaCenterOrgin = ccpSub(m_endCenterOrgin, m_startCenterOrgin);
}

CCActionInterval* CCCenterOrginTo::reverse(void)
{
    return CCCenterOrginTo::create(m_fDuration, m_startCenterOrgin);
}

void CCCenterOrginTo::update(float t)
{
    if (m_pTarget)
    {
#if CC_ENABLE_STACKABLE_ACTIONS
        CCPoint centerOrgin = m_deltaCenterOrgin;
        centerOrgin = ccpMult(centerOrgin, t);
        centerOrgin = ccpAdd(m_startCenterOrgin, centerOrgin);
        m_pTarget->setCenterOrigin(centerOrgin);
#else
        m_pTarget->setCenterOrigin(m_endCenterOrgin);
#endif // CC_ENABLE_STACKABLE_ACTIONS
    }
}


//
// CenterTo
//

CCCenterTo* CCCenterTo::create(float duration, const CCRect& endCenter)
{
    CCCenterTo *pRet = new CCCenterTo();
    pRet->initWithDuration(duration, endCenter);
    pRet->autorelease();
    
    return pRet;
}

bool CCCenterTo::initWithDuration(float duration, const CCRect& endCenter)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_endCenter = endCenter;
        return true;
    }
    
    return false;
}

CAObject* CCCenterTo::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCCenterTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (CCCenterTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCCenterTo();
        pZone = pNewZone = new CAZone(pCopy);
    }
    
    CCActionInterval::copyWithZone(pZone);
    
    pCopy->initWithDuration(m_fDuration, m_endCenter);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCCenterTo::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_startCenter = pTarget->getCenter();
    m_deltaCenter.origin = ccpSub(m_startCenter.origin, m_startCenter.origin);
    m_deltaCenter.size = ccpSub(m_startCenter.size, m_startCenter.size);
}

CCActionInterval* CCCenterTo::reverse(void)
{
    return CCCenterTo::create(m_fDuration, m_startCenter);
}


void CCCenterTo::update(float t)
{
    if (m_pTarget)
    {
#if CC_ENABLE_STACKABLE_ACTIONS
        CCRect center = m_deltaCenter;
        center.origin = ccpMult(center.origin, t);
        center.size = ccpMult(center.size, t);
        center.origin = ccpAdd(m_startCenter.origin, center.origin);
        center.size = ccpAdd(m_startCenter.size, center.size);
        m_pTarget->setCenter(center);
#else
        m_pTarget->setCenter(m_endCenter);
#endif // CC_ENABLE_STACKABLE_ACTIONS
    }
}

//
// CCSkewTo
//
CCSkewTo* CCSkewTo::create(float t, float sx, float sy)
{
    CCSkewTo *pSkewTo = new CCSkewTo();
    if (pSkewTo)
    {
        if (pSkewTo->initWithDuration(t, sx, sy))
        {
            pSkewTo->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(pSkewTo);
        }
    }

    return pSkewTo;
}

bool CCSkewTo::initWithDuration(float t, float sx, float sy)
{
    bool bRet = false;

    if (CCActionInterval::initWithDuration(t))
    {
        m_fEndSkewX = sx;
        m_fEndSkewY = sy;

        bRet = true;
    }

    return bRet;
}

CAObject* CCSkewTo::copyWithZone(CAZone* pZone)
{
    CAZone* pNewZone = NULL;
    CCSkewTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCSkewTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCSkewTo();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_fEndSkewX, m_fEndSkewY);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCSkewTo::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);

    m_fStartSkewX = pTarget->getSkewX();

    if (m_fStartSkewX > 0)
    {
        m_fStartSkewX = fmodf(m_fStartSkewX, 180.f);
    }
    else
    {
        m_fStartSkewX = fmodf(m_fStartSkewX, -180.f);
    }

    m_fDeltaX = m_fEndSkewX - m_fStartSkewX;

    if (m_fDeltaX > 180)
    {
        m_fDeltaX -= 360;
    }
    if (m_fDeltaX < -180)
    {
        m_fDeltaX += 360;
    }

    m_fStartSkewY = pTarget->getSkewY();

    if (m_fStartSkewY > 0)
    {
        m_fStartSkewY = fmodf(m_fStartSkewY, 360.f);
    }
    else
    {
        m_fStartSkewY = fmodf(m_fStartSkewY, -360.f);
    }

    m_fDeltaY = m_fEndSkewY - m_fStartSkewY;

    if (m_fDeltaY > 180)
    {
        m_fDeltaY -= 360;
    }
    if (m_fDeltaY < -180)
    {
        m_fDeltaY += 360;
    }
}

void CCSkewTo::update(float t)
{
    m_pTarget->setSkewX(m_fStartSkewX + m_fDeltaX * t);
    m_pTarget->setSkewY(m_fStartSkewY + m_fDeltaY * t);
}

CCSkewTo::CCSkewTo()
: m_fSkewX(0.0)
, m_fSkewY(0.0)
, m_fStartSkewX(0.0)
, m_fStartSkewY(0.0)
, m_fEndSkewX(0.0)
, m_fEndSkewY(0.0)
, m_fDeltaX(0.0)
, m_fDeltaY(0.0)
{
}

//
// CCSkewBy
//
CCSkewBy* CCSkewBy::create(float t, float sx, float sy)
{
    CCSkewBy *pSkewBy = new CCSkewBy();
    if (pSkewBy)
    {
        if (pSkewBy->initWithDuration(t, sx, sy))
        {
            pSkewBy->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(pSkewBy);
        }
    }

    return pSkewBy;
}

bool CCSkewBy::initWithDuration(float t, float deltaSkewX, float deltaSkewY)
{
    bool bRet = false;

    if (CCSkewTo::initWithDuration(t, deltaSkewX, deltaSkewY))
    {
        m_fSkewX = deltaSkewX;
        m_fSkewY = deltaSkewY;

        bRet = true;
    }

    return bRet;
}

void CCSkewBy::startWithTarget(CAView *pTarget)
{
    CCSkewTo::startWithTarget(pTarget);
    m_fDeltaX = m_fSkewX;
    m_fDeltaY = m_fSkewY;
    m_fEndSkewX = m_fStartSkewX + m_fDeltaX;
    m_fEndSkewY = m_fStartSkewY + m_fDeltaY;
}

CCActionInterval* CCSkewBy::reverse()
{
    return create(m_fDuration, -m_fSkewX, -m_fSkewY);
}

//
// JumpBy
//

CCJumpBy* CCJumpBy::create(float duration, const CCPoint& position, float height, unsigned int jumps)
{
    CCJumpBy *pJumpBy = new CCJumpBy();
    pJumpBy->initWithDuration(duration, position, height, jumps);
    pJumpBy->autorelease();

    return pJumpBy;
}

bool CCJumpBy::initWithDuration(float duration, const CCPoint& position, float height, unsigned int jumps)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_delta = position;
        m_height = height;
        m_nJumps = jumps;

        return true;
    }

    return false;
}

CAObject* CCJumpBy::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCJumpBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCJumpBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCJumpBy();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_delta, m_height, m_nJumps);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCJumpBy::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_previousPos = m_startPosition = pTarget->getFrameOrigin();
}

void CCJumpBy::update(float t)
{
    // parabolic jump (since v0.8.2)
    if (m_pTarget)
    {
        float frac = fmodf( t * m_nJumps, 1.0f );
        float y = m_height * 4 * frac * (1 - frac);
        y += m_delta.y * t;

        float x = m_delta.x * t;
#if CC_ENABLE_STACKABLE_ACTIONS
        CCPoint currentPos = m_pTarget->getFrameOrigin();

        CCPoint diff = ccpSub( currentPos, m_previousPos );
        m_startPosition = ccpAdd( diff, m_startPosition);

        CCPoint newPos = ccpAdd( m_startPosition, ccp(x,y));
        m_pTarget->setFrameOrigin(newPos);

        m_previousPos = newPos;
#else
        m_pTarget->setFrameOrigin(ccpAdd( m_startPosition, ccp(x,y)));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
    }
}

CCActionInterval* CCJumpBy::reverse(void)
{
    return CCJumpBy::create(m_fDuration, ccp(-m_delta.x, -m_delta.y),
        m_height, m_nJumps);
}

//
// JumpTo
//

CCJumpTo* CCJumpTo::create(float duration, const CCPoint& position, float height, int jumps)
{
    CCJumpTo *pJumpTo = new CCJumpTo();
    pJumpTo->initWithDuration(duration, position, height, jumps);
    pJumpTo->autorelease();

    return pJumpTo;
}

CAObject* CCJumpTo::copyWithZone(CAZone* pZone)
{
    CAZone* pNewZone = NULL;
    CCJumpTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (CCJumpTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCJumpTo();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCJumpBy::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_delta, m_height, m_nJumps);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCJumpTo::startWithTarget(CAView *pTarget)
{
    CCJumpBy::startWithTarget(pTarget);
    m_delta = ccp(m_delta.x - m_startPosition.x, m_delta.y - m_startPosition.y);
}

// Bezier cubic formula:
//    ((1 - t) + t)3 = 1 
// Expands to�� 
//   (1 - t)3 + 3t(1-t)2 + 3t2(1 - t) + t3 = 1 
static inline float bezierat( float a, float b, float c, float d, float t )
{
    return (powf(1-t,3) * a + 
            3*t*(powf(1-t,2))*b + 
            3*powf(t,2)*(1-t)*c +
            powf(t,3)*d );
}

//
// BezierBy
//

CCBezierBy* CCBezierBy::create(float t, const ccBezierConfig& c)
{
    CCBezierBy *pBezierBy = new CCBezierBy();
    pBezierBy->initWithDuration(t, c);
    pBezierBy->autorelease();

    return pBezierBy;
}

bool CCBezierBy::initWithDuration(float t, const ccBezierConfig& c)
{
    if (CCActionInterval::initWithDuration(t))
    {
        m_sConfig = c;
        return true;
    }

    return false;
}

void CCBezierBy::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_previousPosition = m_startPosition = pTarget->getFrameOrigin();
}

CAObject* CCBezierBy::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCBezierBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCBezierBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCBezierBy();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_sConfig);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCBezierBy::update(float time)
{
    if (m_pTarget)
    {
        float xa = 0;
        float xb = m_sConfig.controlPoint_1.x;
        float xc = m_sConfig.controlPoint_2.x;
        float xd = m_sConfig.endPosition.x;

        float ya = 0;
        float yb = m_sConfig.controlPoint_1.y;
        float yc = m_sConfig.controlPoint_2.y;
        float yd = m_sConfig.endPosition.y;

        float x = bezierat(xa, xb, xc, xd, time);
        float y = bezierat(ya, yb, yc, yd, time);

#if CC_ENABLE_STACKABLE_ACTIONS
        CCPoint currentPos = m_pTarget->getFrameOrigin();
        CCPoint diff = ccpSub(currentPos, m_previousPosition);
        m_startPosition = ccpAdd( m_startPosition, diff);

        CCPoint newPos = ccpAdd( m_startPosition, ccp(x,y));
        m_pTarget->setFrameOrigin(newPos);

        m_previousPosition = newPos;
#else
        m_pTarget->setFrameOrigin(ccpAdd( m_startPosition, ccp(x,y)));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
    }
}

CCActionInterval* CCBezierBy::reverse(void)
{
    ccBezierConfig r;

    r.endPosition = ccpNeg(m_sConfig.endPosition);
    r.controlPoint_1 = ccpAdd(m_sConfig.controlPoint_2, ccpNeg(m_sConfig.endPosition));
    r.controlPoint_2 = ccpAdd(m_sConfig.controlPoint_1, ccpNeg(m_sConfig.endPosition));

    CCBezierBy *pAction = CCBezierBy::create(m_fDuration, r);
    return pAction;
}

//
// BezierTo
//

CCBezierTo* CCBezierTo::create(float t, const ccBezierConfig& c)
{
    CCBezierTo *pBezierTo = new CCBezierTo();
    pBezierTo->initWithDuration(t, c);
    pBezierTo->autorelease();

    return pBezierTo;
}

bool CCBezierTo::initWithDuration(float t, const ccBezierConfig &c)
{
    bool bRet = false;
    
    if (CCActionInterval::initWithDuration(t))
    {
        m_sToConfig = c;
    }
    
    return bRet;
}

CAObject* CCBezierTo::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCBezierBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCBezierTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCBezierTo();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCBezierBy::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_sConfig);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCBezierTo::startWithTarget(CAView *pTarget)
{
    CCBezierBy::startWithTarget(pTarget);
    m_sConfig.controlPoint_1 = ccpSub(m_sToConfig.controlPoint_1, m_startPosition);
    m_sConfig.controlPoint_2 = ccpSub(m_sToConfig.controlPoint_2, m_startPosition);
    m_sConfig.endPosition = ccpSub(m_sToConfig.endPosition, m_startPosition);
}

//
// ScaleTo
//
CCScaleTo* CCScaleTo::create(float duration, float s)
{
    CCScaleTo *pScaleTo = new CCScaleTo();
    pScaleTo->initWithDuration(duration, s);
    pScaleTo->autorelease();

    return pScaleTo;
}

bool CCScaleTo::initWithDuration(float duration, float s)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_fEndScaleX = s;
        m_fEndScaleY = s;

        return true;
    }

    return false;
}

CCScaleTo* CCScaleTo::create(float duration, float sx, float sy)
{
    CCScaleTo *pScaleTo = new CCScaleTo();
    pScaleTo->initWithDuration(duration, sx, sy);
    pScaleTo->autorelease();

    return pScaleTo;
}

bool CCScaleTo::initWithDuration(float duration, float sx, float sy)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_fEndScaleX = sx;
        m_fEndScaleY = sy;

        return true;
    }

    return false;
}

CAObject* CCScaleTo::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCScaleTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCScaleTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCScaleTo();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);


    pCopy->initWithDuration(m_fDuration, m_fEndScaleX, m_fEndScaleY);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCScaleTo::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_fStartScaleX = pTarget->getScaleX();
    m_fStartScaleY = pTarget->getScaleY();
    m_fDeltaX = m_fEndScaleX - m_fStartScaleX;
    m_fDeltaY = m_fEndScaleY - m_fStartScaleY;
}

void CCScaleTo::update(float time)
{
    if (m_pTarget)
    {
        m_pTarget->setScaleX(m_fStartScaleX + m_fDeltaX * time);
        m_pTarget->setScaleY(m_fStartScaleY + m_fDeltaY * time);
    }
}

//
// ScaleBy
//

CCScaleBy* CCScaleBy::create(float duration, float s)
{
    CCScaleBy *pScaleBy = new CCScaleBy();
    pScaleBy->initWithDuration(duration, s);
    pScaleBy->autorelease();

    return pScaleBy;
}

CCScaleBy* CCScaleBy::create(float duration, float sx, float sy)
{
    CCScaleBy *pScaleBy = new CCScaleBy();
    pScaleBy->initWithDuration(duration, sx, sy);
    pScaleBy->autorelease();

    return pScaleBy;
}

CAObject* CCScaleBy::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCScaleTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (CCScaleBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCScaleBy();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCScaleTo::copyWithZone(pZone);


    pCopy->initWithDuration(m_fDuration, m_fEndScaleX, m_fEndScaleY);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCScaleBy::startWithTarget(CAView *pTarget)
{
    CCScaleTo::startWithTarget(pTarget);
    m_fDeltaX = m_fStartScaleX * m_fEndScaleX - m_fStartScaleX;
    m_fDeltaY = m_fStartScaleY * m_fEndScaleY - m_fStartScaleY;
}

CCActionInterval* CCScaleBy::reverse(void)
{
    return CCScaleBy::create(m_fDuration, 1 / m_fEndScaleX, 1 / m_fEndScaleY);
}

//
// Blink
//

CCBlink* CCBlink::create(float duration, unsigned int uBlinks)
{
    CCBlink *pBlink = new CCBlink();
    pBlink->initWithDuration(duration, uBlinks);
    pBlink->autorelease();

    return pBlink;
}

bool CCBlink::initWithDuration(float duration, unsigned int uBlinks)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_nTimes = uBlinks;
        return true;
    }

    return false;
}

void CCBlink::stop()
{
    m_pTarget->setVisible(m_bOriginalState);
    CCActionInterval::stop();
}

void CCBlink::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_bOriginalState = pTarget->isVisible();
}

CAObject* CCBlink::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCBlink* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCBlink*)(pZone->m_pCopyObject);

    }
    else
    {
        pCopy = new CCBlink();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, (unsigned int)m_nTimes);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCBlink::update(float time)
{
    if (m_pTarget && ! isDone())
    {
        float slice = 1.0f / m_nTimes;
        float m = fmodf(time, slice);
        m_pTarget->setVisible(m > slice / 2 ? true : false);
    }
}

CCActionInterval* CCBlink::reverse(void)
{
    // return 'self'
    return CCBlink::create(m_fDuration, m_nTimes);
}

//
// FadeIn
//

CCFadeIn* CCFadeIn::create(float d)
{
    CCFadeIn* pAction = new CCFadeIn();

    pAction->initWithDuration(d);
    pAction->autorelease();

    return pAction;
}

CAObject* CCFadeIn::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCFadeIn* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject)
    {
        //in case of being called at sub class
        pCopy = (CCFadeIn*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCFadeIn();
        pZone = pNewZone = new CAZone(pCopy);
    }
    
    CCActionInterval::copyWithZone(pZone);

    CC_SAFE_DELETE(pNewZone);

    return pCopy;
}

void CCFadeIn::update(float time)
{
    CARGBAProtocol *pRGBAProtocol = dynamic_cast<CARGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        pRGBAProtocol->setAlpha(time);
    }
    /*m_pTarget->setAlpha(time);*/
}

CCActionInterval* CCFadeIn::reverse(void)
{
    return CCFadeOut::create(m_fDuration);
}

//
// FadeOut
//

CCFadeOut* CCFadeOut::create(float d)
{
    CCFadeOut* pAction = new CCFadeOut();

    pAction->initWithDuration(d);
    pAction->autorelease();

    return pAction;
}

CAObject* CCFadeOut::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCFadeOut* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCFadeOut*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCFadeOut();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    CC_SAFE_DELETE(pNewZone);

    return pCopy;
}

void CCFadeOut::update(float time)
{
    CARGBAProtocol *pRGBAProtocol = dynamic_cast<CARGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        pRGBAProtocol->setAlpha((1 - time));
    }
    /*m_pTarget->setAlpha((1 - time));*/
}

CCActionInterval* CCFadeOut::reverse(void)
{
    return CCFadeIn::create(m_fDuration);
}

//
// FadeTo
//

CCFadeTo* CCFadeTo::create(float duration, float alpha)
{
    CCFadeTo *pFadeTo = new CCFadeTo();
    pFadeTo->initWithDuration(duration, alpha);
    pFadeTo->autorelease();

    return pFadeTo;
}

bool CCFadeTo::initWithDuration(float duration, float alpha)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_toAlpha = alpha;
        return true;
    }

    return false;
}

CAObject* CCFadeTo::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCFadeTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCFadeTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCFadeTo();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_toAlpha);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCFadeTo::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);

    CARGBAProtocol *pRGBAProtocol = dynamic_cast<CARGBAProtocol*>(pTarget);
    if (pRGBAProtocol)
    {
        m_fromAlpha = pRGBAProtocol->getAlpha();
    }
    /*m_fromOpacity = pTarget->getOpacity();*/
}

void CCFadeTo::update(float time)
{
    CARGBAProtocol *pRGBAProtocol = dynamic_cast<CARGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        pRGBAProtocol->setAlpha((m_fromAlpha + (m_toAlpha - m_fromAlpha) * time));
    }
}

//
// TintTo
//
CCTintTo* CCTintTo::create(float duration, GLubyte red, GLubyte green, GLubyte blue)
{
    CCTintTo *pTintTo = new CCTintTo();
    pTintTo->initWithDuration(duration, red, green, blue);
    pTintTo->autorelease();

    return pTintTo;
}

bool CCTintTo::initWithDuration(float duration, GLubyte red, GLubyte green, GLubyte blue)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_to = ccc4(red, green, blue, 255);
        return true;
    }
    return false;
}

CAObject* CCTintTo::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCTintTo* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCTintTo*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCTintTo();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, m_to.r, m_to.g, m_to.b);
    
    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCTintTo::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    CARGBAProtocol *pRGBAProtocol = dynamic_cast<CARGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        m_from = pRGBAProtocol->getColor();
    }
    /*m_from = pTarget->getColor();*/
}

void CCTintTo::update(float time)
{
    CARGBAProtocol *pRGBAProtocol = dynamic_cast<CARGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        pRGBAProtocol->setColor(ccc4(GLubyte(m_from.r + (m_to.r - m_from.r) * time),
            (GLbyte)(m_from.g + (m_to.g - m_from.g) * time),
            (GLbyte)(m_from.b + (m_to.b - m_from.b) * time),
            (GLbyte)255));
    }    
}

//
// TintBy
//

CCTintBy* CCTintBy::create(float duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue)
{
    CCTintBy *pTintBy = new CCTintBy();
    pTintBy->initWithDuration(duration, deltaRed, deltaGreen, deltaBlue);
    pTintBy->autorelease();

    return pTintBy;
}

bool CCTintBy::initWithDuration(float duration, GLshort deltaRed, GLshort deltaGreen, GLshort deltaBlue)
{
    if (CCActionInterval::initWithDuration(duration))
    {
        m_deltaR = deltaRed;
        m_deltaG = deltaGreen;
        m_deltaB = deltaBlue;

        return true;
    }

    return false;
}

CAObject* CCTintBy::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCTintBy* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCTintBy*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCTintBy();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithDuration(m_fDuration, (GLubyte)m_deltaR, (GLubyte)m_deltaG, (GLubyte)m_deltaB);

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

void CCTintBy::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);

    CARGBAProtocol *pRGBAProtocol = dynamic_cast<CARGBAProtocol*>(pTarget);
    if (pRGBAProtocol)
    {
        CAColor4B color = pRGBAProtocol->getColor();
        m_fromR = color.r;
        m_fromG = color.g;
        m_fromB = color.b;
    }    
}

void CCTintBy::update(float time)
{
    CARGBAProtocol *pRGBAProtocol = dynamic_cast<CARGBAProtocol*>(m_pTarget);
    if (pRGBAProtocol)
    {
        pRGBAProtocol->setColor(ccc4((GLubyte)(m_fromR + m_deltaR * time),
            (GLubyte)(m_fromG + m_deltaG * time),
            (GLubyte)(m_fromB + m_deltaB * time),
            (GLubyte)255));
    }
}

CCActionInterval* CCTintBy::reverse(void)
{
    return CCTintBy::create(m_fDuration, -m_deltaR, -m_deltaG, -m_deltaB);
}

//
// DelayTime
//
CCDelayTime* CCDelayTime::create(float d)
{
    CCDelayTime* pAction = new CCDelayTime();

    pAction->initWithDuration(d);
    pAction->autorelease();

    return pAction;
}

CAObject* CCDelayTime::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCDelayTime* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCDelayTime*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCDelayTime();
        pZone = pNewZone = new CAZone(pCopy);
    }

    
    CCActionInterval::copyWithZone(pZone);

    CC_SAFE_DELETE(pNewZone);

    return pCopy;
}

void CCDelayTime::update(float time)
{
    CC_UNUSED_PARAM(time);
    return;
}

CCActionInterval* CCDelayTime::reverse(void)
{
    return CCDelayTime::create(m_fDuration);
}

//
// ReverseTime
//

CCReverseTime* CCReverseTime::create(CCFiniteTimeAction *pAction)
{
    // casting to prevent warnings
    CCReverseTime *pReverseTime = new CCReverseTime();
    pReverseTime->initWithAction(pAction);
    pReverseTime->autorelease();

    return pReverseTime;
}

bool CCReverseTime::initWithAction(CCFiniteTimeAction *pAction)
{
    CCAssert(pAction != NULL, "");
    CCAssert(pAction != m_pOther, "");

    if (CCActionInterval::initWithDuration(pAction->getDuration()))
    {
        // Don't leak if action is reused
        CC_SAFE_RELEASE(m_pOther);

        m_pOther = pAction;
        pAction->retain();

        return true;
    }

    return false;
}

CAObject* CCReverseTime::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCReverseTime* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCReverseTime*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCReverseTime();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithAction((CCFiniteTimeAction*)(m_pOther->copy()->autorelease()));

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCReverseTime::CCReverseTime() : m_pOther(NULL) 
{

}

CCReverseTime::~CCReverseTime(void)
{
    CC_SAFE_RELEASE(m_pOther);
}

void CCReverseTime::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_pOther->startWithTarget(pTarget);
}

void CCReverseTime::stop(void)
{
    m_pOther->stop();
    CCActionInterval::stop();
}

void CCReverseTime::update(float time)
{
    if (m_pOther)
    {
        m_pOther->update(1 - time);
    }
}

CCActionInterval* CCReverseTime::reverse(void)
{
    return (CCActionInterval*)(m_pOther->copy()->autorelease());
}

//
// Animate
//
CCAnimate* CCAnimate::create(CAAnimation *pAnimation)
{
    CCAnimate *pAnimate = new CCAnimate();
    pAnimate->initWithAnimation(pAnimation);
    pAnimate->autorelease();

    return pAnimate;
}

bool CCAnimate::initWithAnimation(CAAnimation *pAnimation)
{
    CCAssert( pAnimation!=NULL, "Animate: argument Animation must be non-NULL");

    float singleDuration = pAnimation->getDuration();

    if ( CCActionInterval::initWithDuration(singleDuration * pAnimation->getLoops() ) ) 
    {
        m_nNextFrame = 0;
        setAnimation(pAnimation);
        m_pOrigFrame = NULL;
        m_uExecutedLoops = 0;

        m_pSplitTimes->reserve(pAnimation->getFrames()->count());

        float accumUnitsOfTime = 0;
        float newUnitOfTimeValue = singleDuration / pAnimation->getTotalDelayUnits();

        CCArray* pFrames = pAnimation->getFrames();
        CCARRAY_VERIFY_TYPE(pFrames, CAAnimationFrame*);

        CAObject* pObj = NULL;
        CCARRAY_FOREACH(pFrames, pObj)
        {
            CAAnimationFrame* frame = (CAAnimationFrame*)pObj;
            float value = (accumUnitsOfTime * newUnitOfTimeValue) / singleDuration;
            accumUnitsOfTime += frame->getDelayUnits();
            m_pSplitTimes->push_back(value);
        }    
        return true;
    }
    return false;
}

CAObject* CCAnimate::copyWithZone(CAZone *pZone)
{
    CAZone* pNewZone = NULL;
    CCAnimate* pCopy = NULL;
    if(pZone && pZone->m_pCopyObject) 
    {
        //in case of being called at sub class
        pCopy = (CCAnimate*)(pZone->m_pCopyObject);
    }
    else
    {
        pCopy = new CCAnimate();
        pZone = pNewZone = new CAZone(pCopy);
    }

    CCActionInterval::copyWithZone(pZone);

    pCopy->initWithAnimation((CAAnimation*)m_pAnimation->copy()->autorelease());

    CC_SAFE_DELETE(pNewZone);
    return pCopy;
}

CCAnimate::CCAnimate()
: m_pAnimation(NULL)
, m_pSplitTimes(new std::vector<float>)
, m_nNextFrame(0)
, m_pOrigFrame(NULL)
, m_uExecutedLoops(0)
{

}

CCAnimate::~CCAnimate()
{
    CC_SAFE_RELEASE(m_pAnimation);
    CC_SAFE_RELEASE(m_pOrigFrame);
    CC_SAFE_DELETE(m_pSplitTimes);
}

void CCAnimate::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    CAImageView *pSprite = (CAImageView*)(pTarget);

    CC_SAFE_RELEASE(m_pOrigFrame);

    if (m_pAnimation->getRestoreOriginalFrame())
    {
        m_pOrigFrame = pSprite->displayFrame();
        m_pOrigFrame->retain();
    }
    m_nNextFrame = 0;
    m_uExecutedLoops = 0;
}

void CCAnimate::stop(void)
{
    if (m_pAnimation->getRestoreOriginalFrame() && m_pTarget)
    {
        ((CAImageView*)(m_pTarget))->setDisplayFrame(m_pOrigFrame);
    }

    CCActionInterval::stop();
}

void CCAnimate::update(float t)
{
    // if t==1, ignore. Animation should finish with t==1
    if( t < 1.0f ) {
        t *= m_pAnimation->getLoops();

        // new loop?  If so, reset frame counter
        unsigned int loopNumber = (unsigned int)t;
        if( loopNumber > m_uExecutedLoops ) {
            m_nNextFrame = 0;
            m_uExecutedLoops++;
        }

        // new t for animations
        t = fmodf(t, 1.0f);
    }

    CCArray* frames = m_pAnimation->getFrames();
    unsigned int numberOfFrames = frames->count();
    CAImageFrame *frameToDisplay = NULL;

    for( unsigned int i=m_nNextFrame; i < numberOfFrames; i++ ) {
        float splitTime = m_pSplitTimes->at(i);

        if( splitTime <= t ) {
            CAAnimationFrame* frame = (CAAnimationFrame*)frames->objectAtIndex(i);
            frameToDisplay = frame->getSpriteFrame();
            ((CAImageView*)m_pTarget)->setDisplayFrame(frameToDisplay);

            CCDictionary* dict = frame->getUserInfo();
            if( dict )
            {
                //TODO: [[NSNotificationCenter defaultCenter] postNotificationName:CAAnimationFrameDisplayedNotification object:target_ userInfo:dict];
            }
            m_nNextFrame = i+1;
        }
        // Issue 1438. Could be more than one frame per tick, due to low frame rate or frame delta < 1/FPS
        else {
            break;
        }
    }
}

CCActionInterval* CCAnimate::reverse(void)
{
    CCArray* pOldArray = m_pAnimation->getFrames();
    CCArray* pNewArray = CCArray::createWithCapacity(pOldArray->count());
   
    CCARRAY_VERIFY_TYPE(pOldArray, CAAnimationFrame*);

    if (pOldArray->count() > 0)
    {
        CAObject* pObj = NULL;
        CCARRAY_FOREACH_REVERSE(pOldArray, pObj)
        {
            CAAnimationFrame* pElement = (CAAnimationFrame*)pObj;
            if (! pElement)
            {
                break;
            }

            pNewArray->addObject((CAAnimationFrame*)(pElement->copy()->autorelease()));
        }
    }

    CAAnimation *newAnim = CAAnimation::create(pNewArray, m_pAnimation->getDelayPerUnit(), m_pAnimation->getLoops());
    newAnim->setRestoreOriginalFrame(m_pAnimation->getRestoreOriginalFrame());
    return create(newAnim);
}

// CCTargetedAction

CCTargetedAction::CCTargetedAction()
: m_pForcedTarget(NULL)
, m_pAction(NULL)
{

}

CCTargetedAction::~CCTargetedAction()
{
    CC_SAFE_RELEASE(m_pForcedTarget);
    CC_SAFE_RELEASE(m_pAction);
}

CCTargetedAction* CCTargetedAction::create(CAView* pTarget, CCFiniteTimeAction* pAction)
{
    CCTargetedAction* p = new CCTargetedAction();
    p->initWithTarget(pTarget, pAction);
    p->autorelease();
    return p;
}


bool CCTargetedAction::initWithTarget(CAView* pTarget, CCFiniteTimeAction* pAction)
{
    if(CCActionInterval::initWithDuration(pAction->getDuration()))
    {
        CC_SAFE_RETAIN(pTarget);
        m_pForcedTarget = pTarget;
        CC_SAFE_RETAIN(pAction);
        m_pAction = pAction;
        return true;
    }
    return false;
}

CAObject* CCTargetedAction::copyWithZone(CAZone* pZone)
{
    CAZone* pNewZone = NULL;
    CCTargetedAction* pRet = NULL;
    if(pZone && pZone->m_pCopyObject) //in case of being called at sub class
    {
        pRet = (CCTargetedAction*)(pZone->m_pCopyObject);
    }
    else
    {
        pRet = new CCTargetedAction();
        pZone = pNewZone = new CAZone(pRet);
    }
    CCActionInterval::copyWithZone(pZone);
    // win32 : use the m_pOther's copy object.
    pRet->initWithTarget(m_pForcedTarget, (CCFiniteTimeAction*)m_pAction->copy()->autorelease()); 
    CC_SAFE_DELETE(pNewZone);
    return pRet;
}

void CCTargetedAction::startWithTarget(CAView *pTarget)
{
    CCActionInterval::startWithTarget(pTarget);
    m_pAction->startWithTarget(m_pForcedTarget);
}

void CCTargetedAction::stop(void)
{
    m_pAction->stop();
}

void CCTargetedAction::update(float time)
{
    m_pAction->update(time);
}

NS_CC_END
