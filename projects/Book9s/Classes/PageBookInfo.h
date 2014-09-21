//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#ifndef __PAGEBOOKINFO_H__
#define __PAGEBOOKINFO_H__
#include "PageBase.h"

// 简介页面
class PageBookInfo : public PageBase
{
	BookIcon *m_pIcon;
	BookData m_bookData;
	CALabel *m_pBookInfo;

	void OnGetMenuComplete( void *pData );
public:
	PageBookInfo();
	virtual void viewDidLoad();
    virtual void viewDidUnload();
	void OnButton1Down( CAControl* pTarget, CCPoint point );
	void OnButton2Down( CAControl* pTarget, CCPoint point );
	void SetBookData( BookData &bookData ) { m_bookData = bookData; }
	void OnComplete( void *pData );
};

#endif