//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#ifndef __PAGERANK_H__
#define __PAGERANK_H__
#include "PageBase.h"

// 排行页
class PageRank : public PageBase , public CATableViewDataSource, public CATableViewDelegate
{
	CATableView *m_pBookList;
	std::vector<BookData> m_vecBookData;
public:
	PageRank();
	virtual void viewDidLoad();
    virtual void viewDidUnload();
	void OnComplete( void *pData );
	CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
	unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
	unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
};

#endif