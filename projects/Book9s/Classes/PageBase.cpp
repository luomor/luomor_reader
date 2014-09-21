//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "PageBase.h"
#include "PageHome.h"
#include "PageManager.h"

PageBase::PageBase() 
{
}

void PageBase::HttpRequest( std::string url , SEL_HttpResponse onComplete )
{
	CCHttpClient* httpClient = CCHttpClient::getInstance();
    CCHttpRequest* httpReq = new CCHttpRequest();
    httpReq->setRequestType(CCHttpRequest::kHttpGet);
    httpReq->setUrl(utf8_to_url_string(url.c_str()).c_str());
    httpReq->setResponseCallback(this,onComplete);
    httpClient->setTimeoutForConnect(30);
    httpClient->send(httpReq);
    httpReq->release();
}

CAButton *g_pBackButtonTest;
void PageBase::viewDidLoad()
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	setKeypadEnabled( true );
#endif

	this->getView()->setColor(CAColor_clear);

	m_pWaitView = CAView::createWithFrame( CAViewController::getView()->getFrame() , ccc4( 0 , 0 , 0 , 128 ) );
	CAViewController::getView()->addSubview( m_pWaitView );
	m_pWaitIcon = CAImageView::createWithImage( CAImage::create( "WaitIcon.png" ) );
	m_pWaitIcon->setFrame( CCRect( ( m_pWaitView->getFrame().size.width - m_pWaitIcon->getFrame().size.width ) / 2 ,
		( m_pWaitView->getFrame().size.height - m_pWaitIcon->getFrame().size.height ) / 2 , 0 , 0 ) );
	m_pWaitView->addSubview( m_pWaitIcon );
	m_pWaitView->setVisible( false );

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
	if ( g_pBackButtonTest == NULL )
	{
		g_pBackButtonTest = CAButton::createWithFrame( CCRect( 0 , 0 , 50 , 50  ) , CAButtonTypeSquareRect );
		g_pBackButtonTest->addTarget( this , CAControl_selector(PageBase::OnTestBackButtonDown) , CAControlTouchUpInSide );
		CAApplication::getApplication()->getRootWindow()->insertSubview( g_pBackButtonTest , 99999 );
	}
#endif
}
void PageBase::OnTestBackButtonDown( CAControl* pTarget, CCPoint point )
{
	keyBackClicked();
}
void PageBase::viewDidUnload()
{

}
CAView *PageBase::getWait()
{
	return m_pWaitView;
}
void PageBase::ShowWait( bool bShow )
{
	/*m_pWaitView->setVisible( bShow );
	if ( bShow )
	{
		CAScheduler::schedule( schedule_selector( PageBase::OnTimer ) , this , 0.1 , false );
	}
	else
	{
		CAScheduler::unschedule( schedule_selector( PageBase::OnTimer ) , this );
	}*/
	PageManager::Instance()->GetWaitLayer()->setVisible( bShow );
}
void PageBase::OnTimer( float time )
{
	m_pWaitIcon->setRotation( m_pWaitIcon->getRotation() + 45 );
}
void PageBase::BeginFrameMove( bool bBegin )
{
	if ( bBegin )
	{
		CAScheduler::schedule( schedule_selector( PageBase::OnFrameMove ) , this , 0.1 , false );
	}
	else
	{
		CAScheduler::unschedule( schedule_selector( PageBase::OnFrameMove ) , this );
	}
}
void PageBase::OnFrameMove( float time )
{
}
void PageBase::keyBackClicked()
{
	if ( getNavigationController() && dynamic_cast<PageHome*>( this ) && PageManager::Instance()->GetWaitLayer()->isVisible() == false )
	{
		if ( getNavigationController()->isNavigationBarHidden() )
			getNavigationController()->setNavigationBarHidden( false , false );
		else
		{
			getNavigationController()->popViewControllerAnimated( true );
		}
	}
}
CABarButtonItem *PageBase::CreateBackButton()
{
	CABarButtonItem *pItem = CABarButtonItem::create( "" , CAImage::create( "BackButton.png" ) , CAImage::create( "BackButton.png" ) );
	pItem->setTarget( this , CAControl_selector(PageBase::OnBackButtonDown) );
	return pItem;
}
void PageBase::OnBackButtonDown( CAControl* pTarget , CCPoint point )
{
	getNavigationController()->popViewControllerAnimated( true );
}
float PageBase::GetScale()
{
	return PageManager::Instance()->GetScale();
}