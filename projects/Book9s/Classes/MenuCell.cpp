//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "MenuCell.h"
#include "PageManager.h"

MenuCell::MenuCell( OnCellDel *pOnCellDeleted )
{
	float fScale = CROSSAPP_ADPTATION_RATIO;
	m_pOnCellDeleted = pOnCellDeleted;
	setFrame( CCRect( 0 , 0 , 720 * PageManager::Instance()->GetScale() , 65 * fScale ) );
	m_pMenuName = CALabel::createWithFrame( CCRect( 45 * fScale , 0 , 610 * fScale , 65 * fScale ) );
	m_pMenuName->setTextAlignment( CATextAlignmentLeft );
	m_pMenuName->setFontSize( 28 * fScale );
	m_pMenuName->setVerticalTextAlignmet(CAVerticalTextAlignmentCenter);
	m_pMenuName->setColor( ccc4( 255 , 255 , 255 , 255 ) );
	//m_pMenuName->setNumberOfLine( 1 );
	addSubview( m_pMenuName );

	m_pDownloadingIcon = CAImageView::createWithFrame( CCRect( 670 * PageManager::Instance()->GetScale() , 15 * fScale , 31 * fScale , 31 * fScale ) );
	m_pDownloadingIcon->setVisible( false );
	addSubview( m_pDownloadingIcon );


	m_pNotRead =  CAImageView::createWithFrame( CCRect( 17 * PageManager::Instance()->GetScale() , 26 * fScale , 12 * fScale , 12 * fScale ) );
	addSubview( m_pNotRead );
}
MenuCell::~MenuCell()
{
	m_pOnCellDeleted->OnCellDeleted( this );
}
void MenuCell::LoadFromBookMenuData( BookMenu *pBookMenu )
{
	m_pMenuName->setText( pBookMenu->m_strBookMenu );
}
void MenuCell::UpdateStatus( int download_status , int read_status )
{
	if ( download_status == STATUS_NONE )
	{
		m_pDownloadingIcon->setVisible( false );
		//m_pDownloadFinishIcon->setVisible( false );
		//m_pDownloadFaield->setVisible( false );
	}
	else if ( download_status == STATUS_DOWNLOADING )
	{
		m_pDownloadingIcon->setVisible( true );
		m_pDownloadingIcon->setImage( CAImage::create( "DownloadingIcon.png" ) );
		//m_pDownloadFinishIcon->setVisible( false );
		//m_pDownloadFaield->setVisible( false );
	}
	else if ( download_status == STATUS_DOWNLOAD_FINISH )
	{
		m_pDownloadingIcon->setVisible( true );
		m_pDownloadingIcon->setImage( CAImage::create( "DownloadFinishIcon.png" ) );
		//m_pDownloadFinishIcon->setVisible( true );
		//m_pDownloadFaield->setVisible( false );
	}
	else if ( download_status == STATUS_DOWNLOAD_FAILED )
	{
		m_pDownloadingIcon->setVisible( true );
		m_pDownloadingIcon->setImage( CAImage::create( "DownloadFailed.png" ) );
		//m_pDownloadFinishIcon->setVisible( false );
		//m_pDownloadFaield->setVisible( true );
	}
	if ( read_status == STATUS_NOTREAD )
	{
		m_pNotRead->setImage( CAImage::create( "NotRead.png" ) );
		//m_pReading->setVisible( false );
		//m_pReaded->setVisible( false );
	}
	else if ( read_status == STATUS_READED )
	{
		m_pNotRead->setImage( CAImage::create( "Readed.png" ) );
		//m_pReading->setVisible( false );
		//m_pReaded->setVisible( true );
	}
	else if ( read_status == STATUS_READING )
	{
		m_pNotRead->setImage( CAImage::create( "Reading.png" ) );
		//m_pReading->setVisible( true );
		//m_pReaded->setVisible( false );
	}
}