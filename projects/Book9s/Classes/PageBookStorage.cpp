//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "PageBookStorage.h"
#include "utf8.h"
#include "BookIcon.h"
#include "PageManager.h"
#include "PageReader.h"

PageBookStorage::PageBookStorage()
{
	CANavigationBarItem *pItem = CANavigationBarItem::create( UTF8("书库") );
	pItem->setShowGoBackButton( false );
	setNavigationBarItem( pItem );
	pItem->addLeftButtonItem( CreateBackButton() );

	CABarButtonItem *pBarItem = CABarButtonItem::create( "" , CAImage::create( "RefreshButton.png" ) , CAImage::create( "RefreshButton.png" ) );
	pBarItem->setTarget( this , CAControl_selector(PageBookStorage::OnRefreshButtonDown) );
	pItem->addRightButtonItem( pBarItem );

}
void PageBookStorage::viewDidLoad()
{
	PageBase::viewDidLoad();

	Refresh();
}
void PageBookStorage::viewDidUnload()
{
	PageBase::viewDidUnload();
}
void PageBookStorage::OnBookIconHold( BookIcon *pBook )
{
	m_currentClickBook = pBook->GetBookData();
	CAButton *pButton = CAButton::createWithFrame( CCRect( 0 , PageManager::Instance()->GetMessageBoxLayer()->getFrame().size.height - 116 * GetScale() , 359 * GetScale() , 116 * GetScale() ) , CAButtonTypeCustom );
	pButton->setBackGroundViewForState( CAControlStateNormal , CAScale9ImageView::createWithImage( CAImage::create( "BackButton2.png" ) ) );
	pButton->setBackGroundViewForState( CAControlStateHighlighted , CAScale9ImageView::createWithImage( CAImage::create( "ReadButtonLight.png" ) ) );
	pButton->addTarget( this , CAControl_selector(PageBookStorage::OnButton5Down) , CAControlTouchUpInSide );
	PageManager::Instance()->GetMessageBoxLayer()->addSubview( pButton );

	pButton = CAButton::createWithFrame( CCRect( 359 * GetScale() , PageManager::Instance()->GetMessageBoxLayer()->getFrame().size.height - 116 * GetScale() , 361 * GetScale() , 116 * GetScale() ) , CAButtonTypeCustom );
	pButton->setBackGroundViewForState( CAControlStateNormal , CAScale9ImageView::createWithImage( CAImage::create( "DeleteButton.png" ) ) );
	pButton->setBackGroundViewForState( CAControlStateHighlighted , CAScale9ImageView::createWithImage( CAImage::create( "DeleteButtonLight.png" ) ) );
	pButton->addTarget( this , CAControl_selector(PageBookStorage::OnButton4Down) , CAControlTouchUpInSide );
	PageManager::Instance()->GetMessageBoxLayer()->addSubview( pButton );
	PageManager::Instance()->GetMessageBoxLayer()->setVisible( true );
}
void PageBookStorage::OnBookIconClick( BookIcon *pBook )
{
	m_currentClickBook = pBook->GetBookData();
	OnButton3Down( NULL , CCPoint() );
}
void PageBookStorage::OnButton3Down( CAControl* pTarget, CCPoint point )
{
	ShowWait( true );
	CCDelayTime *p2 = CCDelayTime::create( 0.5 );
	CCCallFunc *p3 = CCCallFunc::create( this , callfunc_selector(PageBookStorage::ReadBook) );
	CCSequence *p4 = CCSequence::create( p2 , p3 , NULL );
	this->getView()->runAction( p4 );
	PageManager::Instance()->GetMessageBoxLayer()->setVisible( false );
}
void PageBookStorage::OnButton5Down( CAControl* pTarget, CCPoint point )
{
	PageManager::Instance()->GetMessageBoxLayer()->setVisible( false );
}
void PageBookStorage::ReadBook()
{
	ShowWait( false );
	BookData out = m_currentClickBook;
	if ( !BookDB::Instance()->FindLastReadBook( &out , m_currentClickBook.GetKeyName() ) )
		out.m_nLastReadMenu = 0;
	BookDB::Instance()->GetBookMenu( &out , this , BookDB_selector(PageBookStorage::OnGetMenuComplete) );
	PageManager::Instance()->GetMessageBoxLayer()->setVisible( false );
}
void PageBookStorage::OnButton4Down( CAControl* pTarget, CCPoint point )
{
	ShowWait( true );
	CCDelayTime *p2 = CCDelayTime::create( 0.5 );
	CCCallFunc *p3 = CCCallFunc::create( this , callfunc_selector(PageBookStorage::DeleteBook) );
	CCSequence *p4 = CCSequence::create( p2 , p3 , NULL );
	this->getView()->runAction( p4 );
}
void PageBookStorage::DeleteBook()
{
	BookDB::Instance()->DeleteBook( &m_currentClickBook );
	PageManager::Instance()->GetMessageBoxLayer()->setVisible( false );
	Refresh();
}
void PageBookStorage::OnGetMenuComplete( void *pData )
{
	ShowWait( false );
	if ( pData != NULL )
	{
		BookData m_bookData = *(BookData*)pData;
		if ( m_bookData.m_strBookName.size() )
		{
			PageBase *pPageReader = PageManager::Instance()->CreatePageReader( m_bookData , m_bookData.m_nLastReadMenu );
			getNavigationController()->pushViewController( pPageReader , true );
		}
	}
}
void PageBookStorage::OnButton1Down( CAControl* pTarget, CCPoint point )
{
	getNavigationController()->pushViewController( PageManager::Instance()->CreatePageSearch() , true );
}
void PageBookStorage::OnButton2Down( CAControl* pTarget, CCPoint point )
{
	getNavigationController()->pushViewController( PageManager::Instance()->CreatePageRank() , true );
}
void PageBookStorage::OnRefreshButtonDown( CAControl* pTarget , CCPoint point )
{
	ShowWait( true );
	CCDelayTime *p2 = CCDelayTime::create( 0.5 );
	CCCallFunc *p3 = CCCallFunc::create( this , callfunc_selector(PageBookStorage::Refresh) );
	CCSequence *p4 = CCSequence::create( p2 , p3 , NULL );
	this->getView()->runAction( p4 );
}
void PageBookStorage::Refresh()
{
	ShowWait( false );

	this->getView()->removeAllSubviews();
	CAImageView *pBk = CAImageView::createWithImage(CAImage::create("Bk.png"));
	pBk->setFrame( getView()->getFrame() );
	getView()->addSubview( pBk );
	
	std::vector<BookData> vec = BookDB::Instance()->GetStorage();
	if ( vec.size() == 0 )
	{
		CAImageView *pBk2 = CAImageView::createWithImage(CAImage::create("StorageBk.png"));
		pBk2->setFrame( CCRect( 0 , 0 , 720 * GetScale() , 1114 * GetScale() ) );
		getView()->addSubview( pBk2 );

		CAButton *pButton = CAButton::createWithFrame( CCRect( 0 , 500 * GetScale() , 720 * GetScale() , 188 * GetScale() ) , CAButtonTypeCustom );
		pButton->setBackGroundViewForState( CAControlStateNormal , CAImageView::createWithImage( CAImage::create( "StorageButton1.png" ) ) );
		pButton->setBackGroundViewForState( CAControlStateHighlighted , CAImageView::createWithImage( CAImage::create( "StorageButton1light.png" ) ) );
		pButton->addTarget( this , CAControl_selector(PageBookStorage::OnButton1Down) , CAControlTouchUpInSide );
		getView()->addSubview( pButton );

		pButton = CAButton::createWithFrame( CCRect( 0 , 690 * GetScale() , 720 * GetScale() , 188 * GetScale() ) , CAButtonTypeCustom );
		pButton->setBackGroundViewForState( CAControlStateNormal , CAImageView::createWithImage( CAImage::create( "StorageButton2.png" ) ) );
		pButton->setBackGroundViewForState( CAControlStateHighlighted , CAImageView::createWithImage( CAImage::create( "StorageButton2light.png" ) ) );
		pButton->addTarget( this , CAControl_selector(PageBookStorage::OnButton2Down) , CAControlTouchUpInSide );
		getView()->addSubview( pButton );
	}
	else
	{
		CAScrollView *pScrollView = new CAScrollView();
		pScrollView->initWithFrame( getView()->getFrame() );
		pScrollView->setBounceHorizontal( false );
		pScrollView->setAlpha( 0 );
		getView()->addSubview( pScrollView );

		if ( vec.size() > 0 )
		{
			int k = 0;
			for ( int j = 0 ; ; j ++ )
			{
				for ( int i = 0 ; i < 3 ; i ++ )
				{
					BookIcon *pIcon = new BookIcon();
					pIcon->init();
					pIcon->setFrame( CCRect( (i * 230 + 36) * GetScale() , (j * 308 + 36) * GetScale() , 0 , 0 ) );
					pIcon->LoadFromBookData( &vec[k] );
					pIcon->LoadIconFromUrl( vec[k].m_strImageUrl.c_str() );
					k++;
					pIcon->SetTouchEnable( this );
					pScrollView->addSubview( pIcon );

					int y = pIcon->getFrame().origin.y + pIcon->getFrame().size.height + 50 * GetScale();
					if ( pScrollView->getViewSize().height < y )
						pScrollView->setViewSize( CCSize( pScrollView->getFrame().size.width , y ) );
					if ( k >= (int)vec.size() )
						break;
				}
				if ( k >= (int)vec.size() )
					break;
			}
		}
	}
}
void PageBookStorage::keyBackClicked()
{
	PageManager::Instance()->GetMessageBoxLayer()->setVisible( false );
	PageBase::keyBackClicked();
}