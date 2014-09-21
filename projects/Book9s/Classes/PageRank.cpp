//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "PageRank.h"
#include "utf8.h"
#include "PageManager.h"
#include "PageBookInfo.h"

PageRank::PageRank()
{
	CANavigationBarItem *pItem = CANavigationBarItem::create( "排行" );
	setNavigationBarItem( pItem );
	pItem->setShowGoBackButton( false );
	pItem->addLeftButtonItem( CreateBackButton() );
}
void PageRank::viewDidLoad()
{
	PageBase::viewDidLoad();

	CAImageView *pBk = CAImageView::createWithImage(CAImage::create("Bk.png"));
	pBk->setFrame( getView()->getFrame() );
	getView()->addSubview( pBk );

	m_pBookList = new CATableView();
	m_pBookList->initWithFrame( CCRect( 0 , 0 , getView()->getFrame().size.width , getView()->getFrame().size.height ) );
	m_pBookList->setTableViewDataSource(this);
    m_pBookList->setTableViewDelegate(this);
	m_pBookList->setAllowsSelection( true );
	m_pBookList->setSeparatorColor( ccc4( 105 , 114 , 118 , 255 ) );
	m_pBookList->setSeparatorViewHeight( 2 );
	m_pBookList->setAlpha( 0 );
	getView()->addSubview( m_pBookList );
	m_pBookList->release();
	ShowWait( true );
	BookDB::Instance()->OpenRank( this , BookDB_selector(PageRank::OnComplete) );
}
void PageRank::viewDidUnload()
{
	PageBase::viewDidUnload();
}
void PageRank::OnComplete( void *pData )
{
	ShowWait( false );
    if ( pData == NULL )
		return;

	m_vecBookData = *(std::vector<BookData>*)pData;
	m_pBookList->reloadData();
}
class MyCell : public CATableViewCell
{
	void normalTableViewCell()
	{
		CC_RETURN_IF(m_pBackgroundView == NULL);
		m_pBackgroundView->setColor(ccc4(0, 0, 0, 0));
	}
	void highlightedTableViewCell()
	{
		CC_RETURN_IF(m_pBackgroundView == NULL);
		m_pBackgroundView->setColor(ccc4(89, 104, 109, 255));
	}
	void selectedTableViewCell()
	{
		CC_RETURN_IF(m_pBackgroundView == NULL);
		m_pBackgroundView->setColor(ccc4(89, 104, 109, 255));
	} 
};
CATableViewCell* PageRank::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
{ 
	CATableViewCell* cell = table->dequeueReusableCellWithIdentifier("aaa");
    if (cell == NULL)
    {
        cell = new MyCell();
		cell->initWithReuseIdentifier("aaa");
		cell->autorelease();
    }
    cell->removeAllSubviews();

	BookIcon *pIcon = new BookIcon( BookIcon::STYLE_NOICON_IN_LIST );
	pIcon->init();
	if ( row < m_vecBookData.size() )
		pIcon->LoadFromBookData( &m_vecBookData[row] );
    cell->addSubview(pIcon);
	pIcon->release();
	return cell;
}
unsigned int PageRank::numberOfRowsInSection(CATableView *table, unsigned int section)
{
	return m_vecBookData.size();
}
unsigned int PageRank::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	return BookIcon::GetIconHeight( BookIcon::STYLE_NOICON_IN_LIST );
}
void PageRank::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	if ( row < (int)m_vecBookData.size() )
	{
		PageBase *pPageBookInfo = PageManager::Instance()->CreateBookInfo( m_vecBookData[row] );
		getNavigationController()->pushViewController( pPageBookInfo , true );
	}
}