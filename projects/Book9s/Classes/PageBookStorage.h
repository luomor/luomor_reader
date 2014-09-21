//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#ifndef __PAGEBOOKSTORAGE_H__
#define __PAGEBOOKSTORAGE_H__

#include "PageBase.h"

// 书库页面
class PageBookStorage : public PageBase
{
	BookData m_currentClickBook;
	void OnGetMenuComplete( void *pData );
	void OnButton1Down( CAControl* pTarget, CCPoint point );
	void OnButton2Down( CAControl* pTarget, CCPoint point );
	void OnButton3Down( CAControl* pTarget, CCPoint point );
	void OnButton4Down( CAControl* pTarget, CCPoint point );
	void OnButton5Down( CAControl* pTarget, CCPoint point );
	void OnRefreshButtonDown( CAControl* pTarget , CCPoint point );
	void Refresh();
	void DeleteBook();
	void ReadBook();
public:
	PageBookStorage();
	virtual void viewDidLoad();
    virtual void viewDidUnload();
	virtual void OnBookIconClick( BookIcon *pBook );
	virtual void OnBookIconHold( BookIcon *pBook );
	virtual void keyBackClicked();
};

#endif