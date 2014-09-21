//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "adView.h"
#include "utf8.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
void OpenIE( const char *strUrl )
{
	//ShellExecute( NULL , L"open", utf8_to_unicode( strUrl ).c_str() , NULL, NULL, SW_SHOWNORMAL );
	STARTUPINFO si;  
    GetStartupInfo(&si);  
    PROCESS_INFORMATION pi;  
    si.dwFlags = STARTF_USESHOWWINDOW;  
    si.wShowWindow = SW_HIDE;  
  
    CreateProcess(L"C:\\Program Files\\Internet Explorer\\iexplore.exe"  , (LPWSTR)utf8_to_unicode( strUrl ).c_str()  , NULL, NULL, FALSE, CREATE_NO_WINDOW, 
		NULL, NULL, &si, &pi);
}
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
void OpenIE( const char *strUrl )
{
	const char* class_name = "com/component/impl/WebViewCtrl";
	JniMethodInfo info;
	jobject dthero;
	if (JniHelper::getStaticMethodInfo(info, class_name,
			"getInstance", "()Lcom/component/impl/WebViewCtrl;")) {
		dthero = info.env->CallStaticObjectMethod(info.classID, info.methodID);
	}
	if (dthero == NULL) {
		CCLog("activity null!");
		return;
	}
	if (JniHelper::getMethodInfo(info, class_name,
			"browserOpenURL", "(Ljava/lang/String;)V")) {
		info.env->CallVoidMethod(dthero, info.methodID,
				info.env->NewStringUTF(strUrl));
	}
}
#endif

adView::adView()
{
	m_pIcon = NULL;
}
adView::~adView()
{
}
bool adView::init()
{
	CAView::init();
	m_pIcon = CAImageView::create();
	addSubview( m_pIcon );
	setColor( ccc4( 0 , 0 , 0 , 0 ) );
	return true;
}
void adView::Request()
{
	CCHttpClient* httpClient = CCHttpClient::getInstance();
	CCHttpRequest* httpReq = new CCHttpRequest();
	httpReq->setRequestType(CCHttpRequest::kHttpGet);
	httpReq->setUrl("http://u.9miao.com/page/cpc.php?s=7");
	httpReq->setResponseCallback(this,httpresponse_selector(adView::onRequestFinished));
	httpClient->setTimeoutForConnect(30);
	httpClient->send(httpReq);
	httpReq->release();
}
void adView::onRequestFinished( CCHttpClient* client, CCHttpResponse* response )
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
		m_data = root;
		LoadIcon();
	}
}
void adView::LoadIcon()
{
	const char *url = m_data["img"].asCString();
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
				m_pIcon->initWithImage( pNew );
				m_pIcon->setFrame( CCRect( 0 , 0 , getFrame().size.width , getFrame().size.height ) );
				//setFrame( CCRect( (getSuperview()->getFrame().size.width - pNew->getPixelsWide()) / 2 , getFrame().origin.y , pNew->getPixelsWide() , pNew->getPixelsHigh() ) );
			}
		}
		else
		{
			CCHttpClient* httpClient = CCHttpClient::getInstance();
			CCHttpRequest* httpReq = new CCHttpRequest();
			httpReq->setRequestType(CCHttpRequest::kHttpGet);
			httpReq->setUrl(url);
			httpReq->setResponseCallback(this,httpresponse_selector(adView::onIconImgRequestFinished));
			httpClient->setTimeoutForConnect(30);
			httpClient->send(httpReq);
			httpReq->release();
		}
	}
}
void adView::onIconImgRequestFinished( CCHttpClient* client, CCHttpResponse* response )
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
		m_pIcon->initWithImage( pNew );
		m_pIcon->setFrame( CCRect( 0 , 0 , getFrame().size.width , getFrame().size.height ) );
		//setFrame( CCRect( (getSuperview()->getFrame().size.width - pNew->getPixelsWide()) / 2 , getFrame().origin.y , pNew->getPixelsWide() , pNew->getPixelsHigh() ) );
	}
}
bool adView::ccTouchBegan(CATouch *pTouch, CAEvent *pEvent)
{
#if (CC_TARGET_PLATFORM != CC_PLATFORM_IOS)
	if ( m_data["url"].asCString() != NULL )
		OpenIE( m_data["url"].asCString() );
#endif
	return true;
}