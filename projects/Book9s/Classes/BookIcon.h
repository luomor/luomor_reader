//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9��С˵�Ķ����������� http://www.9miao.com/group-40-59173.html
#ifndef __BOOKICON_H__
#define __BOOKICON_H__
#include "BookDB.h"
class PageBase;
// ͼ��ͼ��
class BookIcon : public CAView
{
public:
	const static int STYLE_ICON_WITH_NAME = 0;
	const static int STYLE_ICON_IN_LIST = 1;
	const static int STYLE_NOICON_IN_LIST = 2;
	const static int STYLE_ICON_AND_INFO = 3;
protected:
	std::string m_strBookInfo; // ͼ����
	std::string m_strBookWordSize; // ͼ����������
	std::string m_strBookName; // ͼ������
	std::string m_strBookAuthor; // ͼ������
	std::string m_strLastChapter; // �������½�
	std::string m_strSourceNetName; // ��Դ��վ
	std::string m_strMenuUrl; // Ŀ¼URL
	std::string m_strInfoUrl; // ���URL
	std::string m_strImageUrl; // ͼ��URL
	BookData m_bookData; // ͼ������
	bool m_bHasBookData; // �Ƿ���ͼ������

	CAImageView *m_pIcon; // ͼ��ͼƬ
	CAScale9ImageView *m_pBk; // ����
	CAImageView *m_pTitleBk;  // ���ⱳ��
	CALabel *m_pBookName; // ͼ������
	CALabel *m_pAuthor; // ����
	CALabel *m_pWordSize; // ��������
	CALabel *m_pFromNetName; // ��Դ��վ
	CALabel *m_pLastChapter; // �������½�
	CALabel *m_pBookInfo; // ͼ����
	int m_nStyle;// ��ʾ����
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