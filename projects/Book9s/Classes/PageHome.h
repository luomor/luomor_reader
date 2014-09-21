//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#ifndef __PAGEHOME_H__
#define __PAGEHOME_H__
#include "PageBase.h"

// 主页
class PageHome : public PageBase , public CAScrollViewDelegate
{
	BookIcon *m_vecIcon[9];
	CAImageView *m_vecPageIcon[3];
	int m_nScrollViewBeginDragPosX;
protected:
	void OnGetMenuComplete( void *pData );
	void TouchBeginFinish();
	void OnRefreshButtonDown( CAControl* pTarget , CCPoint point );
	void Refresh();
	void SetPage( int index );
	virtual void keyBackClicked();
public:
	PageHome();
	virtual void viewDidLoad();
    virtual void viewDidUnload();
	void OnButtonDown( CAControl* pTarget, CCPoint point );
	virtual void OnBookIconClick( BookIcon *pBook );
	virtual void scrollViewWillBeginDragging(CAScrollView* view);
	virtual void scrollViewDidEndDragging(CAScrollView* view);
};

#endif