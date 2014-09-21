//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#ifndef __PAGEREADER_H__
#define __PAGEREADER_H__
#include "PageBase.h"

// 全屏切换类
class FullChangedView : public CAView
{
	bool m_bMove;
public:
	FullChangedView()
	{
		m_bMove = false;
	}
	CC_SYNTHESIZE( CANavigationController *, navigation , NavigationController );
	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
	{
		m_bMove = false;
		return true;
	}
    virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
	{
		m_bMove = true;
	}
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
	{
		if ( m_bMove == false )
			navigation->setNavigationBarHidden( !navigation->isNavigationBarHidden() , true );
	}
};
// 阅读页
class PageReader : public PageBase
{
	CAScrollView *m_pScrollView;
	CALabel *m_pMenuName;
	BookData m_bookData;
	int m_nMenu;
	std::vector<CAView*> m_vecControls;
	CAButton *m_pButton;
	CAView *m_pFullScreenButton;
	bool m_bFullScrollDraged;

	void OnNextChapter( CAControl* pTarget, CCPoint point );
	void OnFullScreenChange( CAControl* pTarget , CCPoint point );
	void OnFullScreenTouchBegin( CAControl* pTarget , CCPoint point );
	void OnFullScreenDrag( CAControl* pTarget , CCPoint point );
	void OnMenuButtonDown( CAControl* pTarget , CCPoint point );
	void TouchBeginFinish();
public:
	PageReader();
	virtual void viewDidLoad();
    virtual void viewDidUnload();
	void SetBookData( BookData &bookData , int menu ){ m_bookData = bookData; m_nMenu = menu; }
	void OnComplete( void *pData );
	void reshapeViewRectDidFinish();
};

#endif