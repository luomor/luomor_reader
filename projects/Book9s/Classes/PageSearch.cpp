//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "PageSearch.h"
#include "utf8.h"
#include "BookIcon.h"
#include "PageManager.h"
#include "PageBookInfo.h"

PageSearch::PageSearch()
{
	CANavigationBarItem *pItem = CANavigationBarItem::create( UTF8("搜书") );
	setNavigationBarItem( pItem );
	pItem->setShowGoBackButton( false );
	pItem->addLeftButtonItem( CreateBackButton() );
}
void PageSearch::viewDidLoad()
{
	PageBase::viewDidLoad();

	CAImageView *pBk = CAImageView::createWithImage(CAImage::create("Bk.png"));
	pBk->setFrame( getView()->getFrame() );
	getView()->addSubview( pBk );

	m_pNotFound = CAImageView::createWithImage(CAImage::create("SearchNotFound.png"));
	m_pNotFound->setFrame( CCRect( 236 * GetScale() , 432 * GetScale() , 0 , 0 ) );
	getView()->addSubview( m_pNotFound );
	m_pNotFound->setVisible( false );

	CAImageView *pSearchBar = CAImageView::createWithImage(CAImage::create("SearchBk.png"));
	pSearchBar->setFrame( CCRect( 26 * GetScale() , 29 * GetScale() , 667 * GetScale() , 89 * GetScale() ) );
	getView()->addSubview( pSearchBar );

	m_pInput = CATextField::createWithFrame( CCRect( 26 * GetScale() , 29 * GetScale() , 667 * GetScale() , 89 * GetScale() ) );
	getView()->addSubview( m_pInput );
	m_pInput->setColor( ccc4( 0 , 0 , 0 , 0 ) );
	m_pInput->setFontSize( 32 * GetScale() );
	m_pInput->setTextColor( ccc4( 255 , 255 , 255 , 255 ) );

	// 按作者搜索按钮
	{
		CAButton *pButton = m_pSearchAuthorButton = CAButton::createWithFrame( CCRect( 28 * GetScale() , 143 * GetScale() , 223 * GetScale() , 64 * GetScale() ) , CAButtonTypeCustom );
		pButton->setBackGroundViewForState( CAControlStateNormal , CAScale9ImageView::createWithImage( CAImage::create( "ButtonNormal.png" ) ) );
		pButton->setBackGroundViewForState( CAControlStateHighlighted , CAScale9ImageView::createWithImage( CAImage::create( "ButtonLight.png" ) ) );
		pButton->setBackGroundViewForState( CAControlStateDisabled , CAScale9ImageView::createWithImage( CAImage::create( "ButtonDisable.png" ) ) );
		CALabel *pLabel = CALabel::createWithFrame( CCRect( 0 , 0 , pButton->getFrame().size.width , pButton->getFrame().size.height ) );
		pLabel->setText( UTF8("按作者搜索") );
		pLabel->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		pLabel->setFontSize( 28 * GetScale() );
		pLabel->setTextAlignment( CATextAlignmentCenter );
		pLabel->setVerticalTextAlignmet( CAVerticalTextAlignmentCenter );
		pButton->addSubview( pLabel );
		getView()->addSubview( pButton );
		pButton->addTarget( this , CAControl_selector(PageSearch::OnSearchAuthorButtonDown) , CAControlTouchUpInSide );
	}

	// 按书名搜索按钮
	{
		CAButton *pButton = m_pSearchBookNameButton = CAButton::createWithFrame( CCRect( 283 * GetScale() , 143 * GetScale() , 223 * GetScale() , 64 * GetScale() ) , CAButtonTypeCustom );
		pButton->setBackGroundViewForState( CAControlStateNormal , CAScale9ImageView::createWithImage( CAImage::create( "ButtonNormal.png" ) ) );
		pButton->setBackGroundViewForState( CAControlStateHighlighted , CAScale9ImageView::createWithImage( CAImage::create( "ButtonLight.png" ) ) );
		pButton->setBackGroundViewForState( CAControlStateDisabled , CAScale9ImageView::createWithImage( CAImage::create( "ButtonDisable.png" ) ) );
		CALabel *pLabel = CALabel::createWithFrame( CCRect( 0 , 0 , pButton->getFrame().size.width , pButton->getFrame().size.height ) );
		pLabel->setText( UTF8("按书名搜索") );
		pLabel->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		pLabel->setFontSize( 28 * GetScale() );
		pLabel->setTextAlignment( CATextAlignmentCenter );
		pLabel->setVerticalTextAlignmet( CAVerticalTextAlignmentCenter );
		pButton->addSubview( pLabel );
		getView()->addSubview( pButton );
		pButton->addTarget( this , CAControl_selector(PageSearch::OnSearchBookNameButtonDown) , CAControlTouchUpInSide );
	}
	m_pSearchList = new CATableView();
	m_pSearchList->initWithFrame( CCRect( 0 , 235 * GetScale() , getView()->getFrame().size.width , getView()->getFrame().size.height - 235 * GetScale() ) );
	m_pSearchList->setTableViewDataSource(this);
    m_pSearchList->setTableViewDelegate(this);
	m_pSearchList->setAllowsSelection( true );
	m_pSearchList->setAlpha( 0 );
	getView()->addSubview( m_pSearchList );
	m_pSearchList->release();
}
void PageSearch::viewDidUnload()
{
	PageBase::viewDidUnload();
}
void PageSearch::OnSearchBookNameButtonDown( CAControl* pTarget, CCPoint point )
{
	std::string strBookName = m_pInput->getText();
	if ( strBookName.size() > 0 )
	{
		m_pSearchBookNameButton->setControlStateDisabled();
		m_pSearchAuthorButton->setControlStateDisabled();
		ShowWait( true );
		BookDB::Instance()->Search( strBookName , "bookname" , this , BookDB_selector(PageSearch::OnComplete) );
	}
}
void PageSearch::OnSearchAuthorButtonDown( CAControl* pTarget, CCPoint point )
{
	std::string strAuthorName = m_pInput->getText();
	if ( strAuthorName.size() > 0 )
	{
		m_pSearchBookNameButton->setControlStateDisabled();
		m_pSearchAuthorButton->setControlStateDisabled();
		ShowWait( true );
		BookDB::Instance()->Search( strAuthorName , "author" , this , BookDB_selector(PageSearch::OnComplete) );
	}
}
void PageSearch::OnComplete( void *pData )
{
	m_pSearchBookNameButton->setControlStateNormal();
	m_pSearchAuthorButton->setControlStateNormal();
	ShowWait( false );
    if ( pData == NULL )
		return;
 
	m_vecBookData = *(std::vector<BookData>*)pData;
	m_pSearchList->reloadData();
	m_pNotFound->setVisible( m_vecBookData.size() == 0 );
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
CATableViewCell* PageSearch::tableCellAtIndex(CATableView* table, const CCSize& cellSize, unsigned int section, unsigned int row)
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
unsigned int PageSearch::numberOfRowsInSection(CATableView *table, unsigned int section)
{
	return m_vecBookData.size();
}
unsigned int PageSearch::tableViewHeightForRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	return BookIcon::GetIconHeight( BookIcon::STYLE_NOICON_IN_LIST );
}
void PageSearch::tableViewDidSelectRowAtIndexPath(CATableView* table, unsigned int section, unsigned int row)
{
	if ( row < (int)m_vecBookData.size() )
	{
		PageBase *pPageBookInfo = PageManager::Instance()->CreateBookInfo( m_vecBookData[row] );
		getNavigationController()->pushViewController( pPageBookInfo , true );
	}
}