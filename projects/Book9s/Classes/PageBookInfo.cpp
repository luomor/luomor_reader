//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "PageBookInfo.h"
#include "PageManager.h"
#include "PageMenu.h"
#include "PageReader.h"

PageBookInfo::PageBookInfo()
{ 
	CANavigationBarItem *pItem = CANavigationBarItem::create( "图书信息" );
	setNavigationBarItem( pItem );
	pItem->setShowGoBackButton( false );
	pItem->addLeftButtonItem( CreateBackButton() );
}
void PageBookInfo::viewDidLoad()
{
	PageBase::viewDidLoad();
	CAImageView *pBk = CAImageView::createWithImage(CAImage::create("Bk.png"));
	pBk->setFrame( getView()->getFrame() );
	getView()->addSubview( pBk );

	m_pIcon = new BookIcon( BookIcon::STYLE_ICON_AND_INFO );
	m_pIcon->init();
	getView()->addSubview( m_pIcon );
	m_pIcon->LoadFromBookData( &m_bookData );
	m_pIcon->release();

	{
		CAButton *pButton = CAButton::createWithFrame( CCRect( 63 * GetScale() , 340 * GetScale() , 255 * GetScale() , 80 * GetScale() ) , CAButtonTypeCustom );
		pButton->setBackGroundViewForState( CAControlStateNormal , CAScale9ImageView::createWithImage( CAImage::create( "ButtonNormal.png" ) ) );
		pButton->setBackGroundViewForState( CAControlStateHighlighted , CAScale9ImageView::createWithImage( CAImage::create( "ButtonLight.png" ) ) );
		pButton->setBackGroundViewForState( CAControlStateDisabled , CAScale9ImageView::createWithImage( CAImage::create( "ButtonDisable.png" ) ) );
		CALabel *pLabel = CALabel::createWithFrame( CCRect( 0 , 0 , pButton->getFrame().size.width , pButton->getFrame().size.height ) );
		pLabel->setText( "阅读本书" );
		pLabel->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		pLabel->setFontSize( 28 * GetScale() );
		pLabel->setTextAlignment( CATextAlignmentCenter );
		pLabel->setVerticalTextAlignmet( CAVerticalTextAlignmentCenter );
		pButton->addSubview( pLabel );
		getView()->addSubview( pButton );
		pButton->addTarget( this , CAControl_selector(PageBookInfo::OnButton1Down) , CAControlTouchUpInSide );
	}
	{
		CAButton *pButton = CAButton::createWithFrame( CCRect( 402 * GetScale() , 340 * GetScale() , 255 * GetScale() , 80 * GetScale() ) , CAButtonTypeCustom );
		pButton->setBackGroundViewForState( CAControlStateNormal , CAScale9ImageView::createWithImage( CAImage::create( "ButtonNormal.png" ) ) );
		pButton->setBackGroundViewForState( CAControlStateHighlighted , CAScale9ImageView::createWithImage( CAImage::create( "ButtonLight.png" ) ) );
		pButton->setBackGroundViewForState( CAControlStateDisabled , CAScale9ImageView::createWithImage( CAImage::create( "ButtonDisable.png" ) ) );
		CALabel *pLabel = CALabel::createWithFrame( CCRect( 0 , 0 , pButton->getFrame().size.width , pButton->getFrame().size.height ) );
		pLabel->setText( "浏览目录" );
		pLabel->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		pLabel->setFontSize( 28 * GetScale() );
		pLabel->setTextAlignment( CATextAlignmentCenter );
		pLabel->setVerticalTextAlignmet( CAVerticalTextAlignmentCenter );
		pButton->addSubview( pLabel );
		getView()->addSubview( pButton );
		pButton->addTarget( this , CAControl_selector(PageBookInfo::OnButton2Down) , CAControlTouchUpInSide );
	}
	m_pBookInfo = CALabel::createWithFrame( CCRect( 24 * GetScale() , 468 * GetScale() , 660 * GetScale() , 646 * GetScale() ) );
	m_pBookInfo->setTextAlignment( CATextAlignmentLeft );
	m_pBookInfo->setFontSize( 28 * GetScale() );
	m_pBookInfo->setColor( ccc4( 255 , 255 , 255 , 255 ) );
	getView()->addSubview( m_pBookInfo );
	
	ShowWait( true );
	BookDB::Instance()->GetBookInfo( &m_bookData , this , BookDB_selector(PageBookInfo::OnComplete) );
}
void PageBookInfo::viewDidUnload()
{
	PageBase::viewDidUnload();
}
void PageBookInfo::OnButton1Down( CAControl* pTarget, CCPoint point )
{
	ShowWait( true );
	BookData out;
	if ( BookDB::Instance()->FindLastReadBook( &out , m_bookData.GetKeyName() ) )
		m_bookData.m_nLastReadMenu = out.m_nLastReadMenu;
	else
		m_bookData.m_nLastReadMenu = 0;
	BookDB::Instance()->GetBookMenu( &m_bookData , this , BookDB_selector(PageBookInfo::OnGetMenuComplete) );
}
void PageBookInfo::OnGetMenuComplete( void *pData )
{
	ShowWait( false );
	if ( pData != NULL )
	{
		m_bookData = *(BookData*)pData;
		PageBase *pReader = PageManager::Instance()->CreatePageReader( m_bookData , m_bookData.m_nLastReadMenu );
		getNavigationController()->pushViewController( pReader , true );
	}
}
void PageBookInfo::OnButton2Down( CAControl* pTarget, CCPoint point )
{
	PageBase *pPageMenu = PageManager::Instance()->CreatePageMenu( m_bookData );
	getNavigationController()->pushViewController( pPageMenu , true );
}
void PageBookInfo::OnComplete( void *pData )
{
	ShowWait( false );
	if ( pData != NULL )
	{
		m_bookData = *((BookData*)pData);
		m_pBookInfo->setText( m_bookData.m_strBookInfo );
		m_pIcon->LoadIconFromUrl( m_bookData.m_strImageUrl.c_str() );
	}
}