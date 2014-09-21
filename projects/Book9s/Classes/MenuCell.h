//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#ifndef __MENUCELL_H__
#define __MENUCELL_H__
#include "BookDB.h"

class MenuCell;
class OnCellDel
{
public:
	virtual void OnCellDeleted( MenuCell *p ) = 0;
};
// 目录栏子对象
class MenuCell : public CAView
{
	int m_nStatus;
	CALabel *m_pMenuName;
	std::string m_strUrl;
	CAImageView *m_pDownloadingIcon;
	//CAImageView *m_pDownloadFinishIcon;
	//CAImageView *m_pDownloadFaield;
	CAImageView *m_pNotRead;
	//CAImageView *m_pReading;
	//CAImageView *m_pReaded;
	CSJson::Value m_jsonData;
	CSJson::Value m_contentData;
	OnCellDel *m_pOnCellDeleted;

	void OnDownloadFinish( CCHttpClient* client, CCHttpResponse* response );
	void SetStatus( int status );
public:
	const static int STATUS_NONE = 0;
	const static int STATUS_DOWNLOADING = 1;
	const static int STATUS_DOWNLOAD_FINISH = 2;
	const static int STATUS_DOWNLOAD_FAILED = 3;
	const static int STATUS_NOTREAD = 4;
	const static int STATUS_READED = 5;
	const static int STATUS_READING = 6;

	MenuCell( OnCellDel *pOnCellDeleted );
	~MenuCell();
	void LoadFromBookMenuData( BookMenu *pBookMenu );
	void UpdateStatus( int download_status , int read_status );
};

#endif