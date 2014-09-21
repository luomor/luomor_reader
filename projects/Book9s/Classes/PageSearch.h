//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#ifndef __PAGE_SEARCH_H__
#define __PAGE_SEARCH_H__
#include "PageBase.h"

// 搜索页
class PageSearch : public PageBase , public CATableViewDataSource, public CATableViewDelegate
{
	CAButton *m_pSearchBookNameButton;
	CAButton *m_pSearchAuthorButton;
	CATextField *m_pInput;
	CATableView *m_pSearchList;
	std::vector<BookData> m_vecBookData;
	CAImageView *m_pNotFound;

	void OnSearchBookNameButtonDown( CAControl* pTarget, CCPoint point );
	void OnSearchAuthorButtonDown( CAControl* pTarget, CCPoint point );
public:
	PageSearch();
	virtual void viewDidLoad();
    virtual void viewDidUnload();

	void OnComplete( void *pData );
	CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
	unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
	unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
};

#endif