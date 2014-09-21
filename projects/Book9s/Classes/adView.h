//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#ifndef __ADVIEW_H__
#define __ADVIEW_H__
#include <list>
#include "Book9s.h"
//#include "cocos2d.h"
//#include "cocos-ext.h"
//USING_NS_CC_EXT; 
//USING_NS_CC;

// 广告展示类
class adView : public CAView
{
	CSJson::Value m_data;
	CAImageView *m_pIcon;
	void onIconImgRequestFinished( CCHttpClient* client, CCHttpResponse* response );
	void onRequestFinished( CCHttpClient* client, CCHttpResponse* response );
	// 从url中读取广告图片
	void LoadIcon();
public:
	adView();
	~adView();
	bool init();
	void Request();
	// 点击图片响应
	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
};

#endif