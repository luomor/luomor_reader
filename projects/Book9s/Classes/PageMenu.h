//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#ifndef __PAGEMENU_H__
#define __PAGEMENU_H__
#include "PageBase.h"
#include "MenuCell.h"

// Ŀ¼ҳ
class PageMenu : public PageBase , public CATableViewDataSource , public CATableViewDelegate , public OnCellDel
{
	int m_nPage;
	BookData m_bookData;
	CATableView *m_pMenuList;
	std::vector<MenuCell*> m_vecMenuCell;
	int m_nTouchBeginRow;
	CAProgress *m_pProgress;
	CALabel *m_pProgressLabel;

	void OnComplete( void *pData );
	void OnDownloadComplete( void *pData );
	void TouchBeginFinish();
public:
	PageMenu();
	~PageMenu();
	virtual void viewDidLoad();
    virtual void viewDidUnload();
	void SetBookData( BookData &data );

	virtual CATableViewCell* tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row);
	virtual unsigned int numberOfRowsInSection(CATableView *table, unsigned int section);
    virtual unsigned int tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	virtual void tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row);
	virtual void tableViewDidShowPullDownView(CATableView* table);
    virtual void tableViewDidShowPullUpView(CATableView* table);
	void OnDownloadAllButtonDown( CAControl* pTarget, CCPoint point );
	void OnDownloadButtonDown( CAControl* pTarget, CCPoint point );
	virtual void OnCellDeleted( MenuCell *p );
	static PageMenu *GetCurrentPageMenu();
	void OnUpdateStatus( BookData *pBookData );
};

#endif