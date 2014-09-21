//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "PageReader.h"
#include "PageMenu.h"
#include "PageManager.h"
#include "adView.h"

PageReader::PageReader()
{
	CANavigationBarItem *pItem = CANavigationBarItem::create( UTF8("书页") );
	pItem->setShowGoBackButton( false );
	pItem->addLeftButtonItem( CreateBackButton() );
	CABarButtonItem *pBarItem = CABarButtonItem::create( "" , CAImage::create( "MenuButton.png" ) , CAImage::create( "MenuButton.png" ) );
	pBarItem->setTarget( this , CAControl_selector(PageReader::OnMenuButtonDown) );
	pItem->addRightButtonItem( pBarItem );
	setNavigationBarItem( pItem );
	m_pScrollView = NULL;
	m_pFullScreenButton = NULL;
	m_pButton = NULL;
	m_nMenu = 0;
}
void PageReader::reshapeViewRectDidFinish()
{
	if ( m_pScrollView )
	{
		m_pScrollView->setFrame( CAViewController::getView()->getBounds() );
		getView()->setFrame( CAViewController::getView()->getBounds() );
		getWait()->setFrame( CAViewController::getView()->getBounds() );
	}
}
void PageReader::viewDidLoad()
{
	PageBase::viewDidLoad();

	CANavigationController *p = getNavigationController();
	//p->getNavigationBar()->setBackGroundImage( CAImage::create("TitleBlack.png") );
	m_pScrollView = new CAScrollView();
	m_pScrollView->initWithFrame( getView()->getFrame() );
	m_pScrollView->setShowsHorizontalScrollIndicator( false );
	m_pScrollView->setBackGroundColor( ccc4( 0 , 0 , 0 , 0 ) );
	//m_pScrollView->setViewSize( CCSize( getView()->getFrame().size.width * 3 , 480 ) );
	m_pScrollView->setTouchMovedListenHorizontal( false );
	m_pScrollView->setBounceHorizontal( false );
	getView()->addSubview( m_pScrollView );
	//m_pScrollView->setAlpha( 0 );
	m_pScrollView->release();
	m_pMenuName = CALabel::createWithFrame( CCRect( 0 , 0 , 720 * GetScale() , 80 * GetScale() ) );
	m_pMenuName->setTextAlignment( CATextAlignmentCenter );
	m_pMenuName->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
	//m_pMenuName->setText( m_bookData.m_vecMenu[m_nMenu].m_strBookMenu );
	Setting setting = BookDB::Instance()->GetSetting();
	this->getView()->setColor( setting.IntToCCC4( setting.m_nBkColor ) );
	m_pMenuName->setFontSize( setting.m_nTextSize * CROSSAPP_ADPTATION_RATIO );
	m_pMenuName->setColor( setting.IntToCCC4( setting.m_nTextColor ) );
	m_pScrollView->addSubview( m_pMenuName );
	//getNavigationController()->setNavigationBarHidden( true , true );

	ShowWait( true );
	BookDB::Instance()->ReadBook( &m_bookData , m_nMenu , this , BookDB_selector(PageReader::OnComplete) );
}
void PageReader::viewDidUnload()
{
	PageBase::viewDidUnload();
	m_vecControls.clear();
	m_pScrollView = NULL;
}
void PageReader::OnComplete( void *pData )
{
	ShowWait( false );
	if ( pData != NULL && m_pScrollView != NULL )
	{
		Setting setting = BookDB::Instance()->GetSetting();
		this->getView()->setColor( setting.IntToCCC4( setting.m_nBkColor ) );
		m_pMenuName->setFontSize( setting.m_nTextSize * CROSSAPP_ADPTATION_RATIO );
		m_pMenuName->setColor( setting.IntToCCC4( setting.m_nTextColor ) );

		m_bookData.m_nLastReadMenu = m_nMenu;
		if ( m_nMenu >= 0 && m_nMenu < (int)m_bookData.m_vecMenu.size() )
			m_bookData.m_vecMenu[m_nMenu].m_bReaded = true;
		BookDB::Instance()->AddLastReadBook( &m_bookData );
		for ( int i = 0 ; i < m_vecControls.size() ; i ++ )
			m_vecControls[i]->removeFromSuperview();
		m_vecControls.clear();
		m_pScrollView->setContentOffset( CCPoint( 0 , 0 ) , false );

		CSJson::Value *pJson = (CSJson::Value*)pData;
		std::string str = (*pJson)["content_text"].asString();
		std::string s;
		//m_pMenuName->setText( m_bookData.m_vecMenu[m_nMenu].m_strBookMenu );
		//m_pMenuName->setFrame( CCRect( 0 , 20 , 720 , 80 ) );

		CANavigationBarItem *pItem = CANavigationBarItem::create( m_bookData.m_vecMenu[m_nMenu].m_strBookMenu );
		pItem->setShowGoBackButton( false );
		pItem->addLeftButtonItem( CreateBackButton() );
		CABarButtonItem *pBarItem = CABarButtonItem::create( "" , CAImage::create( "MenuButton.png" ) , CAImage::create( "MenuButton.png" ) );
		pBarItem->setTarget( this , CAControl_selector(PageReader::OnMenuButtonDown) );
		pItem->addRightButtonItem( pBarItem );
		setNavigationBarItem( pItem );

		int ypos = 20 * GetScale();
		for ( int i = 0 , n = (int)str.size() ; i <= n ; i ++ )
		{
			if ( str[i] == '\n' || i == n )
			{
				CALabel *pLabel = CALabel::createWithFrame( CCRect( 20 * GetScale() , ypos , getView()->getFrame().size.width - 40 * GetScale() , 0 ) );
				pLabel->setText( s.c_str() );
				pLabel->setColor( setting.IntToCCC4( setting.m_nTextColor ) );
				pLabel->setFontSize( setting.m_nTextSize * CROSSAPP_ADPTATION_RATIO );
				pLabel->setTextAlignment( CATextAlignmentLeft );
				pLabel->setVerticalTextAlignmet( CAVerticalTextAlignmentTop );
				m_pScrollView->addSubview( pLabel );
				ypos += pLabel->getLabelSize().height + 10;
				s.clear();
				m_vecControls.push_back( pLabel );
			}
			else s += str[i];
		}
		// 全屏切换按钮
		m_pFullScreenButton = new FullChangedView();
		m_pFullScreenButton->init();
		m_pFullScreenButton->setColor( ccc4( 0 , 0 , 0 , 0 ) );
		m_vecControls.push_back( m_pFullScreenButton );
		m_pScrollView->addSubview( m_pFullScreenButton );
		((FullChangedView*)m_pFullScreenButton)->setNavigationController( getNavigationController() );
		m_pFullScreenButton->release();
		// 下一章
		if ( m_nMenu < (int)m_bookData.m_vecMenu.size() - 1 )
		{
			m_pButton = CAButton::createWithFrame( CCRect( (720 - 250)/2 * GetScale() , ypos , 250 * GetScale() , 80 * GetScale() ) , CAButtonTypeCustom );
			m_pButton->setBackGroundViewForState( CAControlStateNormal , CAScale9ImageView::createWithImage( CAImage::create( "ButtonNormal.png" ) ) );
			m_pButton->setBackGroundViewForState( CAControlStateHighlighted , CAScale9ImageView::createWithImage( CAImage::create( "ButtonLight.png" ) ) );
			m_pButton->setBackGroundViewForState( CAControlStateDisabled , CAScale9ImageView::createWithImage( CAImage::create( "ButtonDisable.png" ) ) );
			CALabel *pLabel = CALabel::createWithFrame( CCRect( 0 , 0 , m_pButton->getFrame().size.width , m_pButton->getFrame().size.height ) );
			pLabel->setText( UTF8("下一章") );
			pLabel->setColor( ccc4( 255 , 255 , 255 , 255 ) );
			pLabel->setFontSize( 28 );
			pLabel->setTextAlignment( CATextAlignmentCenter );
			pLabel->setVerticalTextAlignmet( CAVerticalTextAlignmentCenter );
			m_pButton->addSubview( pLabel );
			m_pScrollView->addSubview( m_pButton );
			m_pButton->addTarget( this , CAControl_selector(PageReader::OnNextChapter) , CAControlTouchUpInSide );
			ypos += m_pButton->getFrame().size.height + 10;
			m_vecControls.push_back( m_pButton );
		}
		// 广告展示
		adView *pAdView = new adView();
		pAdView->init();
		pAdView->setFrame( CCRect( 0 , ypos , 720 , 100 ) );
		ypos += 100;
		pAdView->Request();
		m_vecControls.push_back( pAdView );
		m_pScrollView->addSubview( pAdView );

		m_pScrollView->setViewSize( CCSize( getView()->getFrame().size.width , ypos + 20 ) );
		m_pFullScreenButton->setFrame( CCRect( 0 , 0 , m_pScrollView->getViewSize().width , m_pScrollView->getViewSize().height ) );
	}
}
void PageReader::OnNextChapter( CAControl* pTarget, CCPoint point )
{
	ShowWait( true );
	CCDelayTime *p2 = CCDelayTime::create( 0.5 );
	CCCallFunc *p3 = CCCallFunc::create( this , callfunc_selector(PageReader::TouchBeginFinish) );
	CCSequence *p4 = CCSequence::create( p2 , p3 , NULL );
	getView()->runAction( p4 );
	m_pButton->setControlStateDisabled();
}
void PageReader::TouchBeginFinish()
{
	m_nMenu++;
	BookDB::Instance()->ReadBook( &m_bookData , m_nMenu , this , BookDB_selector(PageReader::OnComplete) );
}
void PageReader::OnFullScreenTouchBegin( CAControl* pTarget , CCPoint point )
{
	m_bFullScrollDraged = false;
}
void PageReader::OnFullScreenDrag( CAControl* pTarget , CCPoint point )
{
	m_bFullScrollDraged = true;
}
void PageReader::OnFullScreenChange( CAControl* pTarget , CCPoint point )
{
	if ( !m_bFullScrollDraged )
		getNavigationController()->setNavigationBarHidden( !getNavigationController()->isNavigationBarHidden() , true );
}
void PageReader::OnMenuButtonDown( CAControl* pTarget , CCPoint point )
{
	PageBase *pPageMenu = PageManager::Instance()->CreatePageMenu( m_bookData );
	getNavigationController()->replaceViewController( pPageMenu , true );
}
