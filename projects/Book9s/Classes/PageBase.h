//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#ifndef __PAGEBASE_H__
#define __PAGEBASE_H__
#include "BookIcon.h"
#include "utf8.h"

// 页面基础类
class PageBase : public CAViewController
{
	CAView *m_pWaitView;
	CAImageView *m_pWaitIcon;

	void OnTimer( float time );
	void OnBackButtonDown( CAControl* pTarget , CCPoint point );
	void OnTestBackButtonDown( CAControl* pTarget, CCPoint point );
public:
	PageBase();
	virtual void viewDidLoad();
    virtual void viewDidUnload();
	void HttpRequest( std::string url , SEL_HttpResponse onComplete );
	CAView *getWait();
	void ShowWait( bool bShow );
	void BeginFrameMove( bool bBegin );
	virtual void OnFrameMove( float time );
	virtual void OnBookIconClick( BookIcon *pBook ){}
	virtual void OnBookIconHold( BookIcon *pBook ){}
	void keyBackClicked();
	CABarButtonItem *CreateBackButton();
	float GetScale();
};

#endif