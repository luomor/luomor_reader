//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "ViewController.h"
#include "PageManager.h"
#include "PageSearch.h"
#include "PageReader.h"
#include "PageMenu.h"
#include "PageRank.h"
#include "PageSetting.h"
#include "PageBookInfo.h"


class MessageBoxView : public CAView
{
public:
	MessageBoxView()
	{
	}
	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
	{
		setVisible( false );
		return true;
	}
};
class WaitView : public CAView
{
	CAImageView *m_pWaitIcon;
public:
	WaitView()
	{
	}
	bool initWithFrame( const CCRect &frame , const CAColor4B &color )
	{
		CAView::initWithFrame( frame , color );

		m_pWaitIcon = CAImageView::createWithImage( CAImage::create( "WaitIcon.png" ) );
		m_pWaitIcon->setFrame( CCRect( ( getFrame().size.width - m_pWaitIcon->getFrame().size.width * PageManager::Instance()->GetScale() ) / 2 ,
			( getFrame().size.height - m_pWaitIcon->getFrame().size.height * PageManager::Instance()->GetScale() ) / 2 ,
			m_pWaitIcon->getFrame().size.width * PageManager::Instance()->GetScale() ,
			m_pWaitIcon->getFrame().size.height * PageManager::Instance()->GetScale() ) );
		addSubview( m_pWaitIcon );
		CAScheduler::schedule( schedule_selector( WaitView::OnTimer ) , this , 1/60.0f , false );
		return true;
	}
	void OnTimer( float time )
	{
		m_pWaitIcon->setRotation( m_pWaitIcon->getRotation() + 8 );
	}
};
CAWindow* ViewController::createWindow()
{
	CAWindow* _window = CAWindow::create();

	CANavigationController *p = new CANavigationController();
	p->getNavigationBar()->setBackGroundImage( CAImage::create("Title.png") );
	p->initWithRootViewController( PageManager::Instance()->CreatePageHome() );
	PageManager::Instance()->SetNavigationController( p );
	float w = CAApplication::getApplication()->getWinSize().width;
	PageManager::Instance()->SetScale( w / 720.0f );// 美术的设计尺寸是720宽

	_window->setRootViewController(p);
	p->release();

	CAView *pMessageBox = new MessageBoxView();
	pMessageBox->initWithFrame( _window->getFrame() , ccc4( 0 , 0 , 0 , 127 ) );
	_window->addSubview( pMessageBox );
	pMessageBox->setVisible( false );
	PageManager::Instance()->SetMessageBoxLayer( pMessageBox );
	pMessageBox->release();

	CAView *pWaitView = new WaitView();
	pWaitView->initWithFrame( _window->getFrame() , ccc4( 0 , 0 , 0 , 127 ) );
	_window->addSubview( pWaitView );
	pWaitView->setVisible( false );
	PageManager::Instance()->SetWaitLayer( pWaitView );
	pWaitView->release();
	pWaitView->setVisible( false );
	return _window;
}
