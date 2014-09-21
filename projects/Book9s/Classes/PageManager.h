//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#ifndef __PAGEMANAGER_H__
#define __PAGEMANAGER_H__
#include "PageBase.h"

class BookIcon;
class MenuCell;
class PageManager
{
	CANavigationController *m_pNavigationController;
	CAView *m_pMessageBoxLayer;
	CAView *m_pWaitLayer;
	float m_fScreenScale;
public:
	static PageManager* Instance();
	PageManager();
	PageBase *CreatePageHome();
	PageBase *CreatePageBookStorage();
	PageBase *CreatePageSearch();
	PageBase *CreatePageReader( BookData &bookData , int menu );
	PageBase *CreatePageMenu( BookData &bookData );
	PageBase *CreatePageRank();
	PageBase *CreatePageSetting();
	PageBase *CreateBookInfo( BookData &bookData );
	void SetMessageBoxLayer( CAView *pView ){ m_pMessageBoxLayer = pView; }
	CAView *GetMessageBoxLayer(){ return m_pMessageBoxLayer; }
	void SetWaitLayer( CAView *pView ){ m_pWaitLayer = pView; }
	CAView *GetWaitLayer(){ return m_pWaitLayer; }
	void SetNavigationController( CANavigationController *p );
	std::string GetUrlHeader();
	void SetScale( float fScale );
	float GetScale();
};

#endif