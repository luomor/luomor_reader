//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9��С˵�Ķ����������� http://www.9miao.com/group-40-59173.html
#ifndef __PAGESETTING_H__
#define __PAGESETTING_H__
#include "PageBase.h"

// ϵͳ����ҳ
class PageSetting : public PageBase
{
	CASegmentedControl *m_pFontSize;
	CASegmentedControl *m_pFontColor;
	CASegmentedControl *m_pBkColor;
	void OnUpdateSetting( CAControl* pTarget , CCPoint point );
public:
	PageSetting();
	virtual void viewDidLoad();
    virtual void viewDidUnload();
};

#endif