//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "BookIcon.h"
#include "utf8.h"
#include "PageManager.h"
#include "PageBase.h"

BookIcon::BookIcon( int style )
{
	m_nStyle = style;
	m_pIcon = NULL;
	m_pBk = NULL;
	m_pTitleBk = NULL;
	m_pBookName = NULL;
	m_pAuthor = NULL;
	m_pWordSize = NULL;
	m_pFromNetName = NULL;
	m_pLastChapter = NULL;
	m_pBookInfo = NULL;
	m_pCallbackPage = NULL;
	m_bHasBookData = false;
}
bool BookIcon::init()
{
	CAView::init();
	setColor( ccc4( 0 , 0 , 0 , 0 ) );
	//setDisplayRange( false );
	float fScale = PageManager::Instance()->GetScale();
	if ( m_nStyle == BookIcon::STYLE_ICON_WITH_NAME )
	{
		m_pIcon = CAImageView::createWithImage( CAImage::create( "BookIcon.png" ) );
		m_pIcon->setFrame( CCRect( 0 , 0 , m_pIcon->getFrame().size.width * fScale , m_pIcon->getFrame().size.height * fScale ) );
		addSubview( m_pIcon );

		m_pTitleBk = CAImageView::createWithImage( CAImage::create( "BookTitleBk.png" ) );
		m_pTitleBk->setFrame( CCRect( 0 , m_pIcon->getFrame().size.height - 75 * fScale , m_pIcon->getFrame().size.width , 75 * fScale ) );
		m_pTitleBk->setVisible( false );
		addSubview( m_pTitleBk );

		m_pBookName = CALabel::createWithFrame( CCRect( 4 , m_pIcon->getFrame().size.height - 75 * fScale , m_pIcon->getFrame().size.width - 4 * 2 , 0 ) );
		m_pBookName->setTextAlignment( CATextAlignmentLeft );
		m_pBookName->setFontSize( 28 * fScale );
		m_pBookName->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
		m_pBookName->setColor( ccc4( 34 , 168 , 109 , 255 ) );
		m_pBookName->setNumberOfLine( 1 );
		addSubview( m_pBookName );

		m_pAuthor = CALabel::createWithFrame( CCRect( 4 , 28 * fScale + m_pBookName->getFrame().origin.y , m_pIcon->getFrame().size.width - 4 * 2 , 0 ) );
		m_pAuthor->setTextAlignment( CATextAlignmentLeft );
		m_pAuthor->setFontSize( 20 * fScale );
		m_pAuthor->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		addSubview( m_pAuthor );

		m_pBk = CAScale9ImageView::createWithImage( CAImage::create( "BookBk.png" ) );
		m_pBk->setFrame( m_pIcon->getFrame() );
		addSubview( m_pBk );
		setFrame( m_pIcon->getFrame() );
	}
	else if ( m_nStyle == BookIcon::STYLE_NOICON_IN_LIST )
	{
		setFrame( CCRect( 0 , 0 , 720 * fScale , GetIconHeight( m_nStyle ) ) );

		m_pBookName = CALabel::createWithFrame( CCRect( 14 * fScale , 18 * fScale , getFrame().size.width - 14 * 2 * fScale , 45 * fScale ) );
		m_pBookName->setTextAlignment( CATextAlignmentLeft );
		m_pBookName->setFontSize( 32 * fScale );
		m_pBookName->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
		m_pBookName->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		m_pBookName->setNumberOfLine( 1 );
		addSubview( m_pBookName );

		CALabel *pLabel = CALabel::createWithFrame( CCRect( 330 * fScale , 34 * fScale , 720 * fScale , 45 * fScale ) );
		pLabel->setTextAlignment( CATextAlignmentLeft );
		pLabel->setFontSize( 24 * fScale );
		pLabel->setColor( ccc4( 34 , 168 , 109 , 255 ) );
		pLabel->setText( UTF8("作者：") );
		addSubview( pLabel );

		m_pAuthor = CALabel::createWithFrame( CCRect( 394 * fScale , 34 * fScale , getFrame().size.width , getFrame().size.height ) );
		m_pAuthor->setTextAlignment( CATextAlignmentLeft );
		m_pAuthor->setFontSize( 24 * fScale );
		m_pAuthor->setColor( ccc4( 34 , 168 , 109 , 255 ) );
		addSubview( m_pAuthor );

		pLabel = CALabel::createWithFrame( CCRect( 573 * fScale , 34 * fScale , 720 * fScale , 45 * fScale ) );
		pLabel->setTextAlignment( CATextAlignmentLeft );
		pLabel->setFontSize( 24 * fScale );
		pLabel->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		pLabel->setText( UTF8("字数：") );
		addSubview( pLabel );

		m_pWordSize = CALabel::createWithFrame( CCRect( 637 * fScale , 34 * fScale , getFrame().size.width , getFrame().size.height ) );
		m_pWordSize->setTextAlignment( CATextAlignmentLeft );
		m_pWordSize->setFontSize( 24 * fScale );
		m_pWordSize->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		addSubview( m_pWordSize );

		m_pLastChapter = CALabel::createWithFrame( CCRect( 14 * fScale , 78 * fScale , 702 * fScale , 28 * fScale ) );
		m_pLastChapter->setTextAlignment( CATextAlignmentLeft );
		m_pLastChapter->setFontSize( 24 * fScale );
		m_pLastChapter->setColor( ccc4( 89 , 104 , 109 , 255 ) );
		addSubview( m_pLastChapter );

		if ( m_pBk != NULL )
			addSubview( m_pBk );
	}
	else if ( m_nStyle == BookIcon::STYLE_ICON_IN_LIST )
	{
		setFrame( CCRect( 0 , 0 , 720 * fScale , GetIconHeight( m_nStyle ) ) );

		m_pIcon = CAImageView::createWithImage( CAImage::create( "BookIcon.png" ) );
		m_pIcon->setFrame( CCRect( 18 * fScale , 18 * fScale , 94 * fScale , 133 * fScale ) );
		addSubview( m_pIcon );

		m_pBookName = CALabel::createWithFrame( CCRect( 150 * fScale , 18 * fScale , getFrame().size.width - 150 * 2 , 40 ) );
		m_pBookName->setTextAlignment( CATextAlignmentLeft );
		m_pBookName->setFontSize( 28 );
		m_pBookName->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
		m_pBookName->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		m_pBookName->setNumberOfLine( 1 );
		addSubview( m_pBookName );

		m_pAuthor = CALabel::createWithFrame( CCRect( 150 * fScale , 72 * fScale , getFrame().size.width , getFrame().size.height ) );
		m_pAuthor->setTextAlignment( CATextAlignmentLeft );
		m_pAuthor->setFontSize( 20 );
		m_pAuthor->setColor( ccc4( 34 , 168 , 109 , 255 ) );
		addSubview( m_pAuthor );

		m_pBookInfo = CALabel::createWithFrame( CCRect( 150 * fScale , 104 * fScale , 530 * fScale , 50 * fScale ) );
		m_pBookInfo->setTextAlignment( CATextAlignmentLeft );
		m_pBookInfo->setFontSize( 20 );
		m_pBookInfo->setColor( ccc4( 89 , 104 , 109 , 255 ) );
		addSubview( m_pBookInfo );

		m_pBk = CAScale9ImageView::createWithImage( CAImage::create( "BookBk.png" ) );
		m_pBk->setFrame( m_pIcon->getFrame() );
		addSubview( m_pBk );
	}
	else if ( m_nStyle == BookIcon::STYLE_ICON_AND_INFO )
	{
		setFrame( CCRect( 0 , 0 , 720 * fScale , GetIconHeight( m_nStyle ) ) );

		m_pIcon = CAImageView::createWithImage( CAImage::create( "BookIcon.png" ) );
		m_pIcon->setFrame( CCRect( 25 , 25 , m_pIcon->getFrame().size.width * fScale , m_pIcon->getFrame().size.height * fScale ) );
		addSubview( m_pIcon );

		m_pBk = CAScale9ImageView::createWithImage( CAImage::create( "BookBk.png" ) );
		m_pBk->setFrame( m_pIcon->getFrame() );
		addSubview( m_pBk );

		m_pBookName = CALabel::createWithFrame( CCRect( 250 * fScale , 25 * fScale , getFrame().size.width - 250 * 2 * fScale , 60 * fScale ) );
		m_pBookName->setTextAlignment( CATextAlignmentLeft );
		m_pBookName->setFontSize( 40 * fScale );
		m_pBookName->setVerticalTextAlignmet(CAVerticalTextAlignmentTop);
		m_pBookName->setColor( ccc4( 34 , 168 , 109 , 255 ) );
		m_pBookName->setNumberOfLine( 1 );
		addSubview( m_pBookName );

		CALabel *pLabel = CALabel::createWithFrame( CCRect( 250 * fScale , 90 * fScale , 720 * fScale , 45 * fScale ) );
		pLabel->setTextAlignment( CATextAlignmentLeft );
		pLabel->setFontSize( 28 * fScale );
		pLabel->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		pLabel->setText( UTF8("作者：") );
		addSubview( pLabel );

		m_pAuthor = CALabel::createWithFrame( CCRect( 350 * fScale , 90 * fScale , 720 * fScale , 45 * fScale ) );
		m_pAuthor->setTextAlignment( CATextAlignmentLeft );
		m_pAuthor->setFontSize( 28 * fScale );
		m_pAuthor->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		addSubview( m_pAuthor );

		pLabel = CALabel::createWithFrame( CCRect( 250 * fScale , 150 * fScale , 720 * fScale , 45 * fScale ) );
		pLabel->setTextAlignment( CATextAlignmentLeft );
		pLabel->setFontSize( 28 * fScale );
		pLabel->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		pLabel->setText( UTF8("字数：") );
		addSubview( pLabel );

		m_pWordSize = CALabel::createWithFrame( CCRect( 350 * fScale , 150 * fScale , 720 * fScale , 45 * fScale ) );
		m_pWordSize->setTextAlignment( CATextAlignmentLeft );
		m_pWordSize->setFontSize( 28 * fScale );
		m_pWordSize->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		addSubview( m_pWordSize );

		pLabel = CALabel::createWithFrame( CCRect( 250 * fScale , 210 * fScale , 720 * fScale , 45 * fScale ) );
		pLabel->setTextAlignment( CATextAlignmentLeft );
		pLabel->setFontSize( 28 * fScale );
		pLabel->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		pLabel->setText( UTF8("来源：") );
		addSubview( pLabel );

		m_pFromNetName = CALabel::createWithFrame( CCRect( 350 * fScale , 210 * fScale , 720 * fScale , 45 * fScale ) );
		m_pFromNetName->setTextAlignment( CATextAlignmentLeft );
		m_pFromNetName->setFontSize( 28 * fScale );
		m_pFromNetName->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		addSubview( m_pFromNetName );

		pLabel = CALabel::createWithFrame( CCRect( 250 * fScale , 270 * fScale , 720 * fScale , 45 * fScale ) );
		pLabel->setTextAlignment( CATextAlignmentLeft );
		pLabel->setFontSize( 28 * fScale );
		pLabel->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		pLabel->setText( UTF8("末章：") );
		addSubview( pLabel );

		m_pLastChapter = CALabel::createWithFrame( CCRect( 350 * fScale , 270 * fScale , 720 * fScale , 45 * fScale ) );
		m_pLastChapter->setTextAlignment( CATextAlignmentLeft );
		m_pLastChapter->setFontSize( 28 * fScale );
		m_pLastChapter->setColor( ccc4( 255 , 255 , 255 , 255 ) );
		addSubview( m_pLastChapter );
	}
	return true;
}
int BookIcon::GetIconHeight( int style )
{
	switch( style )
	{
	case BookIcon::STYLE_ICON_WITH_NAME: return 267 * PageManager::Instance()->GetScale();
	case BookIcon::STYLE_NOICON_IN_LIST: return 110 * PageManager::Instance()->GetScale();
	case BookIcon::STYLE_ICON_IN_LIST: return 170 * PageManager::Instance()->GetScale();
	case BookIcon::STYLE_ICON_AND_INFO: return 315 * PageManager::Instance()->GetScale();
	}
	return 0;
}
void BookIcon::SetSize( int width , int height )
{
	/*dSprite::SetSize( width , height );
	m_pBk->SetSize( width , height );
	m_pIcon->SetSize( width , height );
	m_pTitleBk->SetSize( width , 40 );
	m_pTitleBk->SetPos( 0 , (height - m_pTitleBk->GetHeight()) / 2 );
	m_pBookName->SetMergin( 5 , 3 );
	m_pAuthor->SetMergin( 5 , m_pBookName->GetHeight() + 3 );*/
}
bool BookIcon::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
	if ( m_pCallbackPage == NULL || m_bHasBookData == false ) return false;
	CCPoint point = this->convertTouchToNodeSpace(pTouch);
    if ( !this->isVisible() || !getBounds().containsPoint(point) ) return false;
	
	CAScheduler::schedule( schedule_selector( BookIcon::OnHold ) , this , 0 , 0 , 1 );
	return true;
}
void BookIcon::ccTouchMoved(CATouch *pTouch, CAEvent *pEvent)
{
	CAScheduler::unschedule( schedule_selector( BookIcon::OnHold ) , this );
}
void BookIcon::ccTouchEnded(CATouch *pTouch, CAEvent *pEvent)
{
	if ( CAScheduler::isScheduled( schedule_selector( BookIcon::OnHold ) , this ) )
	{
		CAScheduler::unschedule( schedule_selector( BookIcon::OnHold ) , this );
		TouchBeginFinish();
	}
}
void BookIcon::OnHold( float f )
{
	m_pCallbackPage->OnBookIconHold( this );
}
void BookIcon::TouchBeginFinish()
{
	m_pCallbackPage->ShowWait( true );
	CCScaleTo *p1 = CCScaleTo::create( 0.1 , 0.9 , 0.9 );
	CCScaleTo *p2 = CCScaleTo::create( 0.1 , 1 , 1 );
	CCCallFunc *p3 = CCCallFunc::create( this , callfunc_selector(BookIcon::TouchBeginFinish2) );
	CCSequence *p4 = CCSequence::create( p1 , p2 , p3 , NULL );
	this->runAction( p4 );
}
void BookIcon::TouchBeginFinish2()
{
	m_pCallbackPage->ShowWait( false );
	this->setScale( 1 );
	m_pCallbackPage->OnBookIconClick( this );
}
void BookIcon::SetTouchEnable( PageBase *pCallbackPage )
{
	m_pCallbackPage = pCallbackPage;
}
void BookIcon::onIconUrlRequestFinished( CCHttpClient* client, CCHttpResponse* response )
{
	if (!response->isSucceed())
		return;
 
    std::vector<char> *data = response->getResponseData();
    int data_length =  data->size();
    std::string res;
    for (int i = 0;i<data_length;++i)
    {
        res+=(*data)[i];
    }
    res+='\0';
	CSJson::Reader reader;
    CSJson::Value root;
    if (reader.parse(res.c_str(), root))
    {
		m_bHasBookData = true;
		std::string img_url = root[0]["img_url"].asString();
		if ( img_url != "" ) LoadIconFromUrl( img_url.c_str() );
	}
}
void BookIcon::LoadFromBookData( BookData *pBookData )
{
	if ( pBookData == NULL )
	{
		m_bHasBookData = false;
		if ( m_pBookInfo != NULL ) m_pBookInfo->setText( "" );
		if ( m_pWordSize != NULL ) m_pWordSize->setText( "" );
		if ( m_pAuthor != NULL ) m_pAuthor->setText( "" );
		if ( m_pLastChapter != NULL ) m_pLastChapter->setText( "" );
		if ( m_pFromNetName != NULL ) m_pFromNetName->setText( "" );
		if ( m_pBookName != NULL ) m_pBookName->setText( "" );
		if ( m_pTitleBk != NULL ) m_pTitleBk->setVisible( false );
		m_pIcon->setImage( CAImage::create( "BookIcon.png" ) );
	}
	else
	{
		m_bookData = *pBookData;
		m_bHasBookData = true;
		if ( m_pBookName != NULL )
		{
			std::wstring str = utf8_to_unicode( pBookData->m_strBookName.c_str() );
			if ( m_nStyle == BookIcon::STYLE_ICON_WITH_NAME )
			{
				if ( str.size() > 6 ) str.resize( 6 );
			}
			m_pBookName->setText( unicode_to_utf8( str.c_str() ) );
			if ( m_pTitleBk != NULL ) m_pTitleBk->setVisible( true );
		}
		if ( m_pBookInfo != NULL ) m_pBookInfo->setText( pBookData->m_strBookInfo.c_str() );
		if ( m_pWordSize != NULL ) m_pWordSize->setText( pBookData->m_strBookWordSize.c_str() );
		if ( m_pAuthor != NULL ) m_pAuthor->setText( pBookData->m_strBookAuthor.c_str() );
		if ( m_pLastChapter != NULL ) m_pLastChapter->setText( pBookData->m_strLastChapter.c_str() );
		if ( m_pFromNetName != NULL ) m_pFromNetName->setText( pBookData->m_strSourceNetName.c_str() );
	}
}
void BookIcon::LoadIconFromUrl( const char *url )
{
	if ( url != NULL && url[0] != 0 )
	{
		std::string strUrl = url;
		if ( strncmp( strUrl.c_str() , "http://" , 7 ) == 0 )
			strUrl = &strUrl[7];
		for ( int i = 0 ; i < strUrl.size() ; i ++ )
		{
			if ( strUrl[i] == '/' || strUrl[i] == '?' || strUrl[i] == '&' ) strUrl[i] = '_';
		}
		std::string ret = CCFileUtils::sharedFileUtils()->getWritablePath();
		ret += strUrl;
		FILE *pFile = fopen( ret.c_str() , "rb" );
		if ( pFile != NULL )
		{
			fclose( pFile );
			CAImage *pNew = CAImage::create( ret.c_str() );
			if ( pNew != NULL && m_pIcon != NULL )
			{
				CCRect rc = m_pIcon->getFrame();
				m_pIcon->initWithImage( pNew );
				m_pIcon->setFrame( rc );
			}
		}
		else
		{
			CCHttpClient* httpClient = CCHttpClient::getInstance();
			CCHttpRequest* httpReq = new CCHttpRequest();
			httpReq->setRequestType(CCHttpRequest::kHttpGet);
			httpReq->setUrl(url);
			httpReq->setResponseCallback(this,httpresponse_selector(BookIcon::onIconImgRequestFinished));
			httpClient->setTimeoutForConnect(30);
			httpClient->send(httpReq);
			httpReq->release();
		}
	}
}
void BookIcon::onIconImgRequestFinished( CCHttpClient* client, CCHttpResponse* response )
{
	if (!response->isSucceed())
		return;
 
    std::vector<char> *data = response->getResponseData();
    int data_length =  data->size();
	std::string strUrl = response->getHttpRequest()->getUrl();
	if ( strncmp( strUrl.c_str() , "http://" , 7 ) == 0 )
		strUrl = &strUrl[7];
	for ( int i = 0 ; i < strUrl.size() ; i ++ )
	{
		if ( strUrl[i] == '/' || strUrl[i] == '?' || strUrl[i] == '&' ) strUrl[i] = '_';
	}
	std::string ret = CCFileUtils::sharedFileUtils()->getWritablePath();
	ret += strUrl;
	FILE *pWrite = fopen( ret.c_str() , "wb" );
	fwrite( &(*data)[0] , data_length , 1 , pWrite );
	fclose( pWrite );

	CAImage *pNew = CAImage::create( ret.c_str() );
	if ( pNew != NULL )
	{
		CCRect rc = m_pIcon->getFrame();
		m_pIcon->initWithImage( pNew );
		m_pIcon->setFrame( rc );
	}
}
BookData BookIcon::GetBookData()
{
	return m_bookData;
}