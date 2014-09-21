//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#ifndef __BOOKICON_H__
#define __BOOKICON_H__
#include "BookDB.h"
class PageBase;
// 图书图标
class BookIcon : public CAView
{
public:
	const static int STYLE_ICON_WITH_NAME = 0;
	const static int STYLE_ICON_IN_LIST = 1;
	const static int STYLE_NOICON_IN_LIST = 2;
	const static int STYLE_ICON_AND_INFO = 3;
protected:
	std::string m_strBookInfo; // 图书简介
	std::string m_strBookWordSize; // 图书文字数量
	std::string m_strBookName; // 图书名称
	std::string m_strBookAuthor; // 图书作者
	std::string m_strLastChapter; // 最后更新章节
	std::string m_strSourceNetName; // 来源网站
	std::string m_strMenuUrl; // 目录URL
	std::string m_strInfoUrl; // 简介URL
	std::string m_strImageUrl; // 图标URL
	BookData m_bookData; // 图书数据
	bool m_bHasBookData; // 是否有图书数据

	CAImageView *m_pIcon; // 图标图片
	CAScale9ImageView *m_pBk; // 背景
	CAImageView *m_pTitleBk;  // 标题背景
	CALabel *m_pBookName; // 图书名称
	CALabel *m_pAuthor; // 作者
	CALabel *m_pWordSize; // 文字数量
	CALabel *m_pFromNetName; // 来源网站
	CALabel *m_pLastChapter; // 最后更新章节
	CALabel *m_pBookInfo; // 图书简介
	int m_nStyle;// 显示类型
	PageBase *m_pCallbackPage;

	void onIconUrlRequestFinished( CCHttpClient* client, CCHttpResponse* response );
	void onIconImgRequestFinished( CCHttpClient* client, CCHttpResponse* response );
	void OnHold( float f );
public:
	BookIcon( int style = STYLE_ICON_WITH_NAME );
	bool init();
	static int GetIconHeight( int style );
	virtual void SetSize( int width , int height );
	virtual bool ccTouchBegan(CATouch *pTouch, CAEvent *pEvent);
	virtual void ccTouchMoved(CATouch *pTouch, CAEvent *pEvent);
    virtual void ccTouchEnded(CATouch *pTouch, CAEvent *pEvent);
	void TouchBeginFinish();
	void TouchBeginFinish2();
	void LoadIconFromUrl( const char *url );
	bool hasBookData();
	BookData GetBookData();
	void LoadFromBookData( BookData *pBookData );
	void SetTouchEnable( PageBase *pCallbackPage );
};

#endif