//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "PageManager.h"
#include "BookIcon.h"
#include "MenuCell.h"
#include "PageBookInfo.h"
#include "PageReader.h"
#include "PageHome.h"
#include "PageBookStorage.h"
#include "PageSearch.h"
#include "PageMenu.h"
#include "PageRank.h"
#include "PageSetting.h"

PageManager g_pageManager;
PageManager::PageManager()
{
	m_pNavigationController = NULL;
}
PageManager* PageManager::Instance()
{
	return &g_pageManager;
}
PageBase *PageManager::CreatePageHome()
{
	// init PageHome
	PageHome* pPageHome = new PageHome();
	pPageHome->init();
	pPageHome->autorelease();
	return pPageHome;
}
PageBase *PageManager::CreatePageBookStorage()
{
	// init PageBookStorage
	PageBookStorage *pPageBookStorage = new PageBookStorage();
	pPageBookStorage->init();
	pPageBookStorage->autorelease();
	return pPageBookStorage;
}
PageBase *PageManager::CreatePageSearch()
{
	// init PageSearch
	PageSearch *pPageSearch = new PageSearch();
	pPageSearch->init();
	pPageSearch->autorelease();
	return pPageSearch;
}
PageBase *PageManager::CreatePageReader( BookData &bookData , int menu )
{
	// init PageReader
	PageReader *pPageReader = new PageReader();
	pPageReader->SetBookData( bookData , menu );
	pPageReader->init();
	pPageReader->autorelease();
	return pPageReader;
}
PageBase *PageManager::CreatePageMenu( BookData &bookData )
{
	// init PageMenu
	PageMenu *pPageMenu = new PageMenu();
	pPageMenu->SetBookData( bookData );
	pPageMenu->init();
	pPageMenu->autorelease();
	return pPageMenu;
}
PageBase *PageManager::CreatePageRank()
{
	// init PageRank
	PageRank *pPageRank = new PageRank();
	pPageRank->init();
	pPageRank->autorelease();
	return pPageRank;
}
PageBase *PageManager::CreatePageSetting()
{
	// init PageSetting
	PageSetting *pPageSetting = new PageSetting();
	pPageSetting->init();
	pPageSetting->autorelease();
	return pPageSetting;
}
PageBase *PageManager::CreateBookInfo( BookData &bookData )
{
	// init PageBookInfo
	PageBookInfo *pPageBookInfo = new PageBookInfo();
	pPageBookInfo->SetBookData( bookData );
	pPageBookInfo->init();
	pPageBookInfo->autorelease();
	return pPageBookInfo;
}
void PageManager::SetNavigationController( CANavigationController *p )
{
	m_pNavigationController = p;
}
std::string PageManager::GetUrlHeader()
{
	return "http://115.28.171.13:1101/";
}
void PageManager::SetScale( float fScale )
{
	m_fScreenScale = fScale;
}
float PageManager::GetScale()
{
	return m_fScreenScale;
}