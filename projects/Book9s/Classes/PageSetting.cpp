//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "PageSetting.h"
#include "PageManager.h"

PageSetting::PageSetting()
{
	m_pFontSize = NULL;
	m_pFontColor = NULL;
	m_pBkColor = NULL;
	CANavigationBarItem *pItem = CANavigationBarItem::create( "设置" );
	setNavigationBarItem( pItem );
	pItem->setShowGoBackButton( false );
	pItem->addLeftButtonItem( CreateBackButton() );
}
void PageSetting::viewDidLoad()
{
	PageBase::viewDidLoad();

	CAImageView *pBk = CAImageView::createWithImage(CAImage::create("Bk.png"));
	pBk->setFrame( getView()->getFrame() );
	getView()->addSubview( pBk );

	CAImageView *pBk2 = CAImageView::createWithImage(CAImage::create("SettingBk.png"));
	pBk2->setFrame( CCRect( 0 , 56 * GetScale() , pBk2->getFrame().size.width * GetScale() , pBk2->getFrame().size.height * GetScale() ) );
	getView()->addSubview( pBk2 );

	Setting setting = BookDB::Instance()->GetSetting();

	m_pFontSize = CASegmentedControl::createWithFrame( CCRect( 167 * GetScale() , 0 * GetScale() , 511 * GetScale() , 150 * GetScale() ) , 4 );
	for ( int i = 0 ; i < 4 ; i ++ )
	{
		m_pFontSize->setTitleAtIndex( "" , i , CAControlStateAll );
		m_pFontSize->setBackgroundImageAtIndex( NULL , i , CAControlStateAll );
	}
	m_pFontSize->setImageAtIndex( CAImage::create( "FontSize1.png" ) , 0 , CAControlStateNormal );
	m_pFontSize->setImageAtIndex( CAImage::create( "FontSize1Light.png" ) , 0 , CAControlStateSelected );
	m_pFontSize->setImageAtIndex( CAImage::create( "FontSize2.png" ) , 1 , CAControlStateNormal );
	m_pFontSize->setImageAtIndex( CAImage::create( "FontSize2Light.png" ) , 1 , CAControlStateSelected );
	m_pFontSize->setImageAtIndex( CAImage::create( "FontSize3.png" ) , 2 , CAControlStateNormal );
	m_pFontSize->setImageAtIndex( CAImage::create( "FontSize3Light.png" ) , 2 , CAControlStateSelected );
	m_pFontSize->setImageAtIndex( CAImage::create( "FontSize4.png" ) , 3 , CAControlStateNormal );
	m_pFontSize->setImageAtIndex( CAImage::create( "FontSize4Light.png" ) , 3 , CAControlStateSelected );
	m_pFontSize->addTarget( this , CAControl_selector(PageSetting::OnUpdateSetting) );
	m_pFontSize->setSelectedAtIndex( setting.GetTextSizeIndex() );
	getView()->addSubview( m_pFontSize );

	m_pFontColor = CASegmentedControl::createWithFrame( CCRect( 170 * GetScale() , 157 * GetScale() , 491 * GetScale() , 150 * GetScale() ) , 4 );
	for ( int i = 0 ; i < 4 ; i ++ )
	{
		m_pFontColor->setTitleAtIndex( "" , i , CAControlStateAll );
		m_pFontColor->setBackgroundImageAtIndex( NULL , i , CAControlStateAll );
	}
	m_pFontColor->setImageAtIndex( CAImage::create( "Color1.png" ) , 0 , CAControlStateNormal );
	m_pFontColor->setImageAtIndex( CAImage::create( "Color1Light.png" ) , 0 , CAControlStateSelected );
	m_pFontColor->setImageAtIndex( CAImage::create( "Color2.png" ) , 1 , CAControlStateNormal );
	m_pFontColor->setImageAtIndex( CAImage::create( "Color2Light.png" ) , 1 , CAControlStateSelected );
	m_pFontColor->setImageAtIndex( CAImage::create( "Color3.png" ) , 2 , CAControlStateNormal );
	m_pFontColor->setImageAtIndex( CAImage::create( "Color3Light.png" ) , 2 , CAControlStateSelected );
	m_pFontColor->setImageAtIndex( CAImage::create( "Color5.png" ) , 3 , CAControlStateNormal );
	m_pFontColor->setImageAtIndex( CAImage::create( "Color5Light.png" ) , 3 , CAControlStateSelected );
	m_pFontColor->addTarget( this , CAControl_selector(PageSetting::OnUpdateSetting) );
	m_pFontColor->setSelectedAtIndex( setting.GetTextColorIndex() );
	getView()->addSubview( m_pFontColor );

	m_pBkColor = CASegmentedControl::createWithFrame( CCRect( 170 * GetScale() , 349 * GetScale() , 491 * GetScale() , 150 * GetScale() ) , 4 );
	for ( int i = 0 ; i < 4 ; i ++ )
	{
		m_pBkColor->setTitleAtIndex( "" , i , CAControlStateAll );
		m_pBkColor->setBackgroundImageAtIndex( NULL , i , CAControlStateAll );
	}
	m_pBkColor->setImageAtIndex( CAImage::create( "Color1.png" ) , 0 , CAControlStateNormal );
	m_pBkColor->setImageAtIndex( CAImage::create( "Color1Light.png" ) , 0 , CAControlStateSelected );
	m_pBkColor->setImageAtIndex( CAImage::create( "Color2.png" ) , 1 , CAControlStateNormal );
	m_pBkColor->setImageAtIndex( CAImage::create( "Color2Light.png" ) , 1 , CAControlStateSelected );
	m_pBkColor->setImageAtIndex( CAImage::create( "Color4.png" ) , 2 , CAControlStateNormal );
	m_pBkColor->setImageAtIndex( CAImage::create( "Color4Light.png" ) , 2 , CAControlStateSelected );
	m_pBkColor->setImageAtIndex( CAImage::create( "Color6.png" ) , 3 , CAControlStateNormal );
	m_pBkColor->setImageAtIndex( CAImage::create( "Color6Light.png" ) , 3 , CAControlStateSelected );
	m_pBkColor->addTarget( this , CAControl_selector(PageSetting::OnUpdateSetting) );
	m_pBkColor->setSelectedAtIndex( setting.GetBkColorIndex() );
	getView()->addSubview( m_pBkColor );
}
void PageSetting::viewDidUnload()
{
	PageBase::viewDidUnload();
}
void PageSetting::OnUpdateSetting( CAControl* pTarget , CCPoint point )
{
	if ( m_pFontSize != NULL && m_pFontColor != NULL && m_pBkColor != NULL )
	{
		Setting setting = BookDB::Instance()->GetSetting();
		setting.SetTextSizeIndex( m_pFontSize->getselectedIndex() );
		setting.SetTextColorIndex( m_pFontColor->getselectedIndex() );
		setting.SetBkColorIndex( m_pBkColor->getselectedIndex() );
		BookDB::Instance()->SetSetting( setting );
	}
}