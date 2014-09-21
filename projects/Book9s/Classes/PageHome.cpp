//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "PageHome.h"
#include "BookIcon.h"
#include "utf8.h"
#include "PageManager.h"
#include "PageReader.h"

PageHome::PageHome()
{
	m_nScrollViewBeginDragPosX = 0;
	CANavigationBarItem *pItem = CANavigationBarItem::create( UTF8("9秒小说阅读器") );
	setNavigationBarItem( pItem );
	CABarButtonItem *pBarItem = CABarButtonItem::create( "" , CAImage::create( "RefreshButton.png" ) , CAImage::create( "RefreshButton.png" ) );
	pBarItem->setTarget( this , CAControl_selector(PageHome::OnRefreshButtonDown) );
	pItem->addRightButtonItem( pBarItem );
}
void PageHome::viewDidLoad()
{
	PageBase::viewDidLoad();

	CAImageView *pBk = CAImageView::createWithImage(CAImage::create("Bk.png"));
	pBk->setFrame( getView()->getFrame() );
	getView()->addSubview( pBk );

	CAImageView *pBk3 = CAImageView::createWithImage(CAImage::create("Bk3.png"));
	pBk3->setFrame( CCRect( 0 , 360 * GetScale() , getView()->getFrame().size.width , getView()->getFrame().size.height - 320 * GetScale() ) );
	getView()->addSubview( pBk3 );

	for ( int i = 0 ; i < 3 ; i ++ )
	{
		m_vecPageIcon[i] = CAImageView::createWithImage( CAImage::create("HomePageIcon1.png") );
		m_vecPageIcon[i]->setFrame( CCRect( (300 + i * 46) * GetScale() , 370 * GetScale() , 29 * GetScale() , 6 * GetScale() ) );
		getView()->addSubview( m_vecPageIcon[i] );
	}
	SetPage( 0 );

	CAScrollView *pScrollView = new CAScrollView();
	pScrollView->initWithFrame( CCRect( 0 , 0 , getView()->getFrame().size.width , 480 * GetScale() ) );
	pScrollView->setViewSize( CCSize( getView()->getFrame().size.width * 3 , 480 * GetScale() ) );
	pScrollView->setBounceVertical( false );
	pScrollView->setScrollViewDelegate( this );
	pScrollView->setAlpha( 0 );
	getView()->addSubview( pScrollView );
	pScrollView->setShowsHorizontalScrollIndicator( false );
	pScrollView->setShowsVerticalScrollIndicator( false );
	pScrollView->release();

	for ( int j = 0 ; j < 3 ; j ++ )
	{
		for ( int i = 0 ; i < 3 ; i ++ )
		{
			m_vecIcon[j*3+i] = new BookIcon();
			m_vecIcon[j*3+i]->init();
			m_vecIcon[j*3+i]->setFrame( CCRect( (i * 230 + 35 + j * 720) * GetScale() , 50 * GetScale(), 0 , 0 ) );
			pScrollView->addSubview( m_vecIcon[j*3+i] );
		}
	}

	const char* buttonNames[] = { "PageHomeButton1.png" , "PageHomeButton2.png" , "PageHomeButton3.png" , "PageHomeButton4.png" };
	for ( int i = 0 ; i < 4 ; i ++ )
	{
		CAButton *pButton = CAButton::createWithFrame( CCRect( 36 * GetScale() , (pBk3->getFrame().size.height / 4 - 30 * GetScale() ) * i + 420 * GetScale() , 650 * GetScale() , 120 * GetScale() ) , CAButtonTypeCustom );
		pButton->setBackGroundViewForState( CAControlStateNormal , CAScale9ImageView::createWithImage( CAImage::create( "ButtonNormalHome.png" ) ) );
		pButton->setBackGroundViewForState( CAControlStateHighlighted , CAScale9ImageView::createWithImage( CAImage::create( "ButtonLightHome.png" ) ) );
		CAImageView *pImage = CAImageView::createWithImage( CAImage::create( buttonNames[i] ) );
		pImage->setFrame( CCRect( 0 , 0 , pImage->getFrame().size.width * GetScale() , pImage->getFrame().size.height * GetScale() ) );
		pImage->setCenter( CCRect( pButton->getFrame().size.width / 2 , pButton->getFrame().size.height / 2 , 0 , 0 ) );
		pButton->addSubview( pImage );
		pButton->setTag(i);
		pButton->addTarget( this , CAControl_selector(PageHome::OnButtonDown) , CAControlTouchUpInSide );
		getView()->addSubview( pButton );
	}
	
	Refresh();
}
void PageHome::scrollViewWillBeginDragging(CAScrollView* view)
{
	m_nScrollViewBeginDragPosX = view->getContentOffset().x;
}
void PageHome::scrollViewDidEndDragging(CAScrollView* view)
{
	int fScreenWidth = view->getFrame().size.width;
	int x = view->getContentOffset().x;
	if ( x < m_nScrollViewBeginDragPosX - 100 )
	{
		if ( x > 0 )
		{
			x = m_nScrollViewBeginDragPosX;
			x = (x + fScreenWidth/2) / fScreenWidth * fScreenWidth - fScreenWidth;
			view->setContentOffset( CCPoint( x , 0 ) , true );
		}
	}
	else if ( x > m_nScrollViewBeginDragPosX + 100 )
	{
		if ( x <= fScreenWidth * 2 )
		{
			x = m_nScrollViewBeginDragPosX;
			x = (x + fScreenWidth/2) / fScreenWidth * fScreenWidth + fScreenWidth;
			view->setContentOffset( CCPoint( x , 0 ) , true );
		}
	}
	else
	{
		x = x / fScreenWidth * fScreenWidth;
		view->setContentOffset( CCPoint( x , 0 ) , true );
	}
	SetPage( x / fScreenWidth );
}
void PageHome::OnButtonDown( CAControl* pTarget, CCPoint point )
{
	switch( pTarget->getTag() )
	{
	case 0:
		{
			ShowWait( true );
			CCDelayTime *p2 = CCDelayTime::create( 0.5 );
			CCCallFunc *p3 = CCCallFunc::create( this , callfunc_selector(PageHome::TouchBeginFinish) );
			CCSequence *p4 = CCSequence::create( p2 , p3 , NULL );
			this->getView()->runAction( p4 );
		}
		break;
	case 1:
		getNavigationController()->pushViewController( PageManager::Instance()->CreatePageSearch() , true );
		break;
	case 2:
		getNavigationController()->pushViewController( PageManager::Instance()->CreatePageRank() , true );
		break;
	case 3:
		getNavigationController()->pushViewController( PageManager::Instance()->CreatePageSetting() , true );
		break;
	}
}
void PageHome::TouchBeginFinish()
{
	ShowWait( false );
	getNavigationController()->pushViewController( PageManager::Instance()->CreatePageBookStorage() , true );
}
void PageHome::viewDidUnload()
{
	PageBase::viewDidUnload();
}
void PageHome::OnBookIconClick( BookIcon *pBook )
{
	ShowWait( true );
	BookData out;
	if ( !BookDB::Instance()->FindLastReadBook( &out , pBook->GetBookData().GetKeyName() ) )
		out.m_nLastReadMenu = 0;
	BookDB::Instance()->GetBookMenu( &out , this , BookDB_selector(PageHome::OnGetMenuComplete) );
}
void PageHome::OnGetMenuComplete( void *pData )
{
	ShowWait( false );
	if ( pData != NULL )
	{
		BookData m_bookData = *(BookData*)pData;
		if ( m_bookData.m_strBookName != "" )
		{
			PageBase *pPageReader = PageManager::Instance()->CreatePageReader( m_bookData , m_bookData.m_nLastReadMenu );
			getNavigationController()->pushViewController( pPageReader , true );
		}
	}
}
void PageHome::OnRefreshButtonDown( CAControl* pTarget , CCPoint point )
{
	ShowWait( true );
	CCDelayTime *p2 = CCDelayTime::create( 0.5 );
	CCCallFunc *p3 = CCCallFunc::create( this , callfunc_selector(PageHome::Refresh) );
	CCSequence *p4 = CCSequence::create( p2 , p3 , NULL );
	this->getView()->runAction( p4 );
}
void PageHome::Refresh()
{
	ShowWait( false );
	std::vector<BookData> vecBook = BookDB::Instance()->GetLastReadBook();
	for ( int i = 0 ; i < (int)vecBook.size() ; i ++ )
	{
		if ( i >= 9 ) break;
		m_vecIcon[i]->LoadFromBookData( &vecBook[i] );
		m_vecIcon[i]->LoadIconFromUrl( vecBook[i].m_strImageUrl.c_str() );
		m_vecIcon[i]->SetTouchEnable( this );
	}
	for ( int i =(int)vecBook.size() ; i < 9 ; i ++ )
		m_vecIcon[i]->LoadFromBookData( NULL );
}
void PageHome::SetPage( int index )
{
	for ( int i = 0 ; i < 3 ; i ++ )
	{
		m_vecPageIcon[i]->setImage( CAImage::create(i == index ? "HomePageIcon2.png":"HomePageIcon1.png") );
	}
}
void PageHome::keyBackClicked()
{
	int count = getNavigationController()->getViewControllerCount();
	if ( count == 1 )
	{
		CAApplication::getApplication()->end();
#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)
		exit( 0 );
#endif
	}
	else PageBase::keyBackClicked();
}