//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9秒小说阅读器在线讨论 http://www.9miao.com/group-40-59173.html
#include "BookDB.h"
#include "utf8.h"
#include "sqlite3.h"
#include "MenuCell.h"
#include "PageMenu.h"
BookDB *BookDB::m_pInstance = NULL;

Setting::Setting()
{
	m_nTextColor = 0xFF2A2A2A;
	m_nBkColor = 0xFFC6C6C6;
	m_nTextSize = 40;
}
int Setting::GetTextColorIndex()
{
	switch( m_nTextColor )
	{
	case 0xFF2A2A2A: return 0;
	case 0xFFC6C6C6: return 1;
	case 0xFF2e3729: return 2;
	case 0xFF4b4335: return 3;
	}
	return 0;
}
void Setting::SetTextColorIndex( int idx )
{
	switch( idx )
	{
	case 0: m_nTextColor = 0xFF2A2A2A; break;
	case 1: m_nTextColor = 0xFFC6C6C6; break;
	case 2: m_nTextColor = 0xFF2e3729; break;
	case 3: m_nTextColor = 0xFF4b4335; break;
	}
}
int Setting::GetTextSizeIndex()
{
	switch( m_nTextSize )
	{
	case 30: return 0;
	case 40: return 1;
	case 50: return 2;
	case 60: return 3;
	}
	return 1;
}
void Setting::SetTextSizeIndex( int idx )
{
	switch( idx )
	{
	case 0: m_nTextSize = 30; break;
	case 1: m_nTextSize = 40; break;
	case 2: m_nTextSize = 50; break;
	case 3: m_nTextSize = 60; break;
	}
}
int Setting::GetBkColorIndex()
{
	switch( m_nBkColor )
	{
	case 0xFF2A2A2A: return 0;
	case 0xFFC6C6C6: return 1;
	case 0xFFafd8ae: return 2;
	case 0xFFbfad8e: return 3;
	}
	return 0;
}
void Setting::SetBkColorIndex( int idx )
{
	switch( idx )
	{
	case 0: m_nBkColor = 0xFF2A2A2A; break;
	case 1: m_nBkColor = 0xFFC6C6C6; break;
	case 2: m_nBkColor = 0xFFafd8ae; break;
	case 3: m_nBkColor = 0xFFbfad8e; break;
	}
}

BookDB::BookDB()
{
	m_nDownloadingMaxNum = 0;
}
BookDB::~BookDB()
{
}
void BookDB::Release()
{
	m_pInstance->release();
	m_pInstance = NULL;
}
BookDB* BookDB::Instance()
{
	if ( m_pInstance == NULL ) m_pInstance = new BookDB();
	return m_pInstance;
}
std::string BookDB::GetUrlHeader()
{
	return "http://203.195.184.223:1101/";
}
void BookDB::HttpRequest( std::string url , CAObject *pTarget , SEL_BookDBOnComplete onComplete , SEL_BookDBOnHttpComplete onHttpComplete , void *pUserData , int nThread )
{
	//if ( m_mapHttpClient.find( nThread ) == m_mapHttpClient.end() )
	//	m_mapHttpClient[nThread] = new CCHttpClient();
	if ( pTarget != NULL ) pTarget->retain();
	//CCHttpClient* httpClient = m_mapHttpClient[nThread];
	CCHttpClient *httpClient = CCHttpClient::getInstance( nThread );
	CCHttpRequest* httpReq = new BookDBHttpRequest( pTarget , onComplete , onHttpComplete , pUserData );
    httpReq->setRequestType(CCHttpRequest::kHttpGet);
    httpReq->setUrl(utf8_to_url_string(url.c_str()).c_str());
	httpReq->setResponseCallback(this,httpresponse_selector(BookDB::HttpFinished));
    httpClient->setTimeoutForConnect(30);
    httpClient->send(httpReq);
    httpReq->release();
}
void BookDB::HttpFinished( CCHttpClient* client, CCHttpResponse* response )
{
	BookDBHttpRequest *pRequest = (BookDBHttpRequest*)response->getHttpRequest();
    if (!response->isSucceed())
	{
		if ( pRequest->pTarget != NULL )
		{
			(pRequest->pTarget->*pRequest->onHttpComplete)(pRequest, NULL);
			pRequest->pTarget->release();
		}
		return;
	}
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
		(this->*pRequest->onHttpComplete)(pRequest, &root);
		if ( pRequest->pTarget != NULL )
			pRequest->pTarget->release();
		return;
	}
	else
	{
		(this->*pRequest->onHttpComplete)(pRequest, NULL);
		if ( pRequest->pTarget != NULL )
			pRequest->pTarget->release();
		return;
	}
}
void BookDB::OpenRank( CAObject *pTarget , SEL_BookDBOnComplete onComplete )
{
	HttpRequest( GetUrlHeader() + "rank" , pTarget , onComplete , BookDB_httpselector(BookDB::OpenRankFinished) );
}
void BookDB::OpenRankFinished( BookDBHttpRequest *pRequest , CSJson::Value *pJson )
{
	if ( pJson == NULL ) (pRequest->pTarget->*pRequest->onComplete)( NULL );
	else
	{
		std::vector<BookData> vecBookData;
		vecBookData.resize( pJson->size() );
		for ( int i = 0 ; i < pJson->size() ; i ++ )
			vecBookData[i].ReadFromJson( (*pJson)[i] );
		(pRequest->pTarget->*pRequest->onComplete)( &vecBookData );
	}
}
void BookDB::Search( std::string strSearchKey , std::string strSearchType , CAObject *pTarget , SEL_BookDBOnComplete onComplete )
{
	HttpRequest( GetUrlHeader() + "search?" + strSearchType + "=" + strSearchKey , pTarget , onComplete , BookDB_httpselector(BookDB::SearchFinished) );
}
void BookDB::SearchFinished( BookDBHttpRequest *pRequest , CSJson::Value *pJson )
{
	if ( pJson == NULL ) (pRequest->pTarget->*pRequest->onComplete)( NULL );
	else
	{
		std::vector<BookData> vecBookData;
		vecBookData.resize( pJson->size() );
		for ( int i = 0 ; i < pJson->size() ; i ++ )
			vecBookData[i].ReadFromJson( (*pJson)[i] );
		(pRequest->pTarget->*pRequest->onComplete)( &vecBookData );
	}
}
void BookDB::GetBookInfo( BookData *pBook , CAObject *pTarget , SEL_BookDBOnComplete onComplete )
{
	HttpRequest( GetUrlHeader() + "intro?url=" + pBook->m_strInfoUrl , pTarget , onComplete , BookDB_httpselector(BookDB::GetBookInfoFinished) );
}
void BookDB::GetBookInfoFinished( BookDBHttpRequest *pRequest , CSJson::Value *pJson )
{
	if ( pJson == NULL ) (pRequest->pTarget->*pRequest->onComplete)( NULL );
	else
	{
		BookData bookData;
		bookData.ReadFromJson( (*pJson)[0] );
		(pRequest->pTarget->*pRequest->onComplete)( &bookData );
	}
}
int BookDB::GetBookMenuDownloadStatus( BookData *pBook , int menu )
{
	int ret = MenuCell::STATUS_NONE;
	int nLastReadMenu = pBook->m_nLastReadMenu;
	pBook->m_nLastReadMenu = menu;
	std::string test2 = pBook->GetKeyNameWithMenuID().c_str();
	if ( FindStorageBookData(pBook , NULL ) )
		ret = MenuCell::STATUS_DOWNLOAD_FINISH;
	else if ( m_mapDownloading.find(test2) != m_mapDownloading.end() && m_mapDownloading[test2] == 1 )
		ret = MenuCell::STATUS_DOWNLOADING;
	else if ( m_mapDownloading.find(test2) != m_mapDownloading.end() && m_mapDownloading[test2] == -1 )
		ret = MenuCell::STATUS_DOWNLOAD_FAILED;
	else
		ret = MenuCell::STATUS_NONE;
	pBook->m_nLastReadMenu = nLastReadMenu;
	return ret;
}
int BookDB::GetBookMenuReadStatus( BookData *pBook , int menu )
{
	int ret = MenuCell::STATUS_NOTREAD;
	std::string test2 = pBook->GetKeyNameWithMenuID().c_str();
	if ( menu == pBook->m_nLastReadMenu )
		ret = MenuCell::STATUS_READING;
	else
	{
		if ( m_strBookDataForGetReadStatusKey != pBook->GetKeyName() )
		{
			m_strBookDataForGetReadStatusKey = pBook->GetKeyName();
			FindLastReadBook( &m_bookDataForGetReadStatus , pBook->GetKeyName() );
		}
		if ( menu >= 0 && menu < (int)m_bookDataForGetReadStatus.m_vecMenu.size() )
		{
			if ( m_bookDataForGetReadStatus.m_vecMenu[menu].m_bReaded )
				ret = MenuCell::STATUS_READED;
			else
				ret = MenuCell::STATUS_NOTREAD;
		}
	}
	return ret;
}
void BookDB::GetBookMenu( BookData *pBook , CAObject *pTarget , SEL_BookDBOnComplete onComplete )
{
	// 先从本地查找
	BookData bookData;
	if ( FindStorage( pBook->GetKeyName() , &bookData ) )
	{
		bookData.m_nLastReadMenu = pBook->m_nLastReadMenu;
		(pTarget->*onComplete)( &bookData );
		return;
	}
	m_currentGetBookMenu = *pBook;
	HttpRequest( GetUrlHeader() + "cat?url=" + pBook->m_strMenuUrl , pTarget , onComplete , BookDB_httpselector(BookDB::GetBookMenuFinished) );
}
void BookDB::GetBookMenuFinished( BookDBHttpRequest *pRequest , CSJson::Value *pJson )
{
	if ( pJson == NULL ) (pRequest->pTarget->*pRequest->onComplete)( NULL );
	else
	{
		BookData bookData = m_currentGetBookMenu;
		bookData.m_vecMenu.resize( pJson->size() );
		for ( int i = 0 ; i < pJson->size() ; i ++ )
		{
			bookData.m_vecMenu[i].ReadFromJson( (*pJson)[i] );
		}
		(pRequest->pTarget->*pRequest->onComplete)( &bookData );
	}
}
void BookDB::DownloadBook( BookData *pBook , int start_menu , int end_menu , CAObject *pTarget , SEL_BookDBOnComplete onComplete )
{
	sqlite3 *pSql = NULL;
	std::string tab_name = "book_storage_menu";
	sqlite3_open( (CCFileUtils::sharedFileUtils()->getWritablePath() + tab_name + ".db" ).c_str() , &pSql );
	if ( pSql != NULL )
	{
		// 创建表
		SqliteQurey( pSql , "create table if not exists " + tab_name + " (id text primary key, data text)" );
		// 添加新行
		SqliteQurey( pSql , "insert into " + tab_name + " values(" + TransToSqlString( pBook->GetKeyName() ) + "," + TransToSqlString( pBook->GetJsonString() ) + ")" );
		sqlite3_close( pSql );
	}
	for ( int i = start_menu ; i < end_menu ; i ++ )
	{
		int status = GetBookMenuDownloadStatus( pBook , i );
		if ( status == MenuCell::STATUS_NONE || status == MenuCell::STATUS_DOWNLOAD_FAILED )
		{
			BookData *pBookData = new BookData();
			pBookData->m_strBookName = pBook->m_strBookName;
			pBookData->m_strBookAuthor = pBook->m_strBookAuthor;
			pBookData->m_strSourceNetName = pBook->m_strSourceNetName;
			pBookData->m_nLastReadMenu = i;
			m_mapDownloading[ pBookData->GetKeyNameWithMenuID() ] = 1;
			HttpRequest( GetUrlHeader() + "content?url=" + pBook->m_vecMenu[i].m_strTextUrl , pTarget , onComplete , BookDB_httpselector(BookDB::DownloadBookFinished) , pBookData , 1 );
		}
	}
	m_nDownloadingMaxNum = (int)m_mapDownloading.size();
}
void BookDB::DownloadBookFinished( BookDBHttpRequest *pRequest , CSJson::Value *pJson )
{
	BookData *pBookData = (BookData*)pRequest->pUserData;
	if ( !pJson )
	{
		m_mapDownloading[ pBookData->GetKeyNameWithMenuID() ] = -1;
		delete pBookData;
		return;
	}
	sqlite3 *pSql = NULL;
	std::string tab_name = "book_storage_data";
	sqlite3_open( (CCFileUtils::sharedFileUtils()->getWritablePath() + tab_name + ".db" ).c_str() , &pSql );
	if ( pSql != NULL )
	{
		// 创建表
		SqliteQurey( pSql , "create table if not exists " + tab_name + " (id text primary key, data text)" );
		// 如果当前已存在这本名，则删除
		SqliteCursor *p = SqliteQurey( pSql , "select * from " + tab_name + " where id = " + TransToSqlString( pBookData->GetKeyNameWithMenuID() ) );
		if ( p != NULL && p->GetRowCount() > 0 )
		{
			SqliteQurey( pSql , "delete from " + tab_name + " where id = " + TransToSqlString( pBookData->GetKeyNameWithMenuID() ) );
			delete p;
		}
		// 添加新书
		CSJson::FastWriter fast_writer;
		SqliteQurey( pSql , "insert into " + tab_name + " (id, data) values(" + TransToSqlString( pBookData->GetKeyNameWithMenuID() ) + "," + TransToSqlString( fast_writer.write( *pJson ) ) + ")" );
		sqlite3_close( pSql );
	}
	//if ( pRequest->pTarget != NULL && pRequest->onComplete != NULL )
	//	(pRequest->pTarget->*pRequest->onComplete)( &pBookData->m_nLastReadMenu );
	if ( PageMenu::GetCurrentPageMenu() != NULL )
		PageMenu::GetCurrentPageMenu()->OnUpdateStatus( pBookData );
	if ( m_mapDownloading.find( pBookData->GetKeyNameWithMenuID() ) != m_mapDownloading.end() )
		m_mapDownloading.erase( m_mapDownloading.find( pBookData->GetKeyNameWithMenuID() ) );
	delete pBookData;
}
bool BookDB::FindStorage( std::string strBookKey , BookData *pOut )
{
	bool ret = false;
	sqlite3 *pSql = NULL;
	std::string tab_name = "book_storage_menu";
	sqlite3_open( (CCFileUtils::sharedFileUtils()->getWritablePath() + tab_name + ".db" ).c_str() , &pSql );
	if ( pSql != NULL )
	{
		SqliteCursor *p = SqliteQurey( pSql , "select * from " + tab_name + " where id = " + TransToSqlString( strBookKey ) );
		if ( p != NULL )
		{
			if ( p->GetRowCount() > 0 )
			{
				pOut->FromJsonString( p->GetData( 1 ) );
				ret = true;
			}
		}
		delete p;
		sqlite3_close( pSql );
	}
	return ret;
}
std::vector<BookData> BookDB::GetStorage()
{
	std::list<BookData> list;
	std::vector<BookData> vec;
	sqlite3 *pSql = NULL;
	std::string tab_name = "book_storage_menu";
	sqlite3_open( (CCFileUtils::sharedFileUtils()->getWritablePath() + tab_name + ".db" ).c_str() , &pSql );
	if ( pSql != NULL )
	{
		SqliteCursor *p = SqliteQurey( pSql , "select * from " + tab_name );
		if ( p != NULL )
		{
			for ( int i = p->GetRowCount() - 1 ; i >= 0 ; i -- )
			{
				p->SetPos( i );
				BookData bookData;
				bookData.FromJsonString( p->GetData( 1 ) , false );
				list.push_back( bookData );
			}
			delete p;
		}
		sqlite3_close( pSql );
	}
	vec.resize( list.size() );
	int k = 0;
	for ( std::list<BookData>::iterator it = list.begin() ; it != list.end() ; it ++ )
		vec[k++] = *it;
	return vec;
}
void BookDB::ReadBook( BookData *pBook , int menu , CAObject *pTarget , SEL_BookDBOnComplete onComplete )
{
	// 先从本地数据库查找
	BookData bookData;
	if ( FindStorage( pBook->GetKeyName() , &bookData ) )
	{
		pBook->m_nLastReadMenu = menu;
		CSJson::Value root;
		if ( FindStorageBookData( pBook , &root ) )
		{
			(pTarget->*onComplete)( &root );
			return;
		}
	}
	BookData *pBook2 = new BookData();
	*pBook2 = *pBook;
	DownloadBook( pBook , menu , menu + 1 , NULL , NULL );
	HttpRequest( GetUrlHeader() + "content?url=" + pBook->m_vecMenu[menu].m_strTextUrl , pTarget , onComplete , BookDB_httpselector(BookDB::ReadBookFinished) );
}
void BookDB::ReadBookFinished( BookDBHttpRequest *pRequest , CSJson::Value *pJson )
{
	if ( pJson == NULL ) (pRequest->pTarget->*pRequest->onComplete)( NULL );
	else
	{
		(pRequest->pTarget->*pRequest->onComplete)( pJson );
	}
}
void BookDB::DeleteBook( BookData *pBook )
{
	sqlite3 *pSql = NULL;
	std::string tab_name = "book_storage_menu";
	sqlite3_open( (CCFileUtils::sharedFileUtils()->getWritablePath() + tab_name + ".db" ).c_str() , &pSql );
	if ( pSql != NULL )
	{
		// 如果当前已存在这本名，则删除
		SqliteCursor *p = SqliteQurey( pSql , "select * from " + tab_name + " where id = " + TransToSqlString( pBook->GetKeyName() ) );
		if ( p != NULL && p->GetRowCount() > 0 )
		{
			SqliteQurey( pSql , "delete from " + tab_name + " where id = " + TransToSqlString( pBook->GetKeyName() ) );
			delete p;
		}
		sqlite3_close( pSql );
	}
	tab_name = "book_storage_data";
	sqlite3_open( (CCFileUtils::sharedFileUtils()->getWritablePath() + tab_name + ".db" ).c_str() , &pSql );
	if ( pSql != NULL )
	{
		// 创建表
		SqliteQurey( pSql , "create table if not exists " + tab_name + " (id text primary key, data text)" );
		// 如果当前已存在这本名，则删除
		for ( int i = 0 ; i < (int)pBook->m_vecMenu.size() ; i ++ )
		{
			pBook->m_nLastReadMenu = i;
			SqliteCursor *p = SqliteQurey( pSql , "select * from " + tab_name + " where id = " + TransToSqlString( pBook->GetKeyNameWithMenuID() ) );
			if ( p != NULL && p->GetRowCount() > 0 )
			{
				SqliteQurey( pSql , "delete from " + tab_name + " where id = " + TransToSqlString( pBook->GetKeyNameWithMenuID() ) );
				delete p;
			}
		}
	}
	tab_name = "last_read";
	sqlite3_open( (CCFileUtils::sharedFileUtils()->getWritablePath() + tab_name + ".db").c_str() , &pSql );
	if ( pSql != NULL )
	{
		// 创建表
		SqliteQurey( pSql , "create table if not exists " + tab_name + " (id text primary key, data text)" );
		// 如果当前已存在这本名，则删除
		SqliteCursor *p = SqliteQurey( pSql , "select * from " + tab_name + " where id = " + TransToSqlString( pBook->GetKeyName() ) );
		if ( p != NULL && p->GetRowCount() > 0 )
		{
			SqliteQurey( pSql , "delete from " + tab_name + " where id = " + TransToSqlString( pBook->GetKeyName() ) );
			delete p;
		}
	}
}
bool BookDB::FindStorageBookData( BookData *pBook , CSJson::Value *pOut )
{
	sqlite3 *pSql = NULL;
	std::string tab_name = "book_storage_data";
	sqlite3_open( (CCFileUtils::sharedFileUtils()->getWritablePath() + tab_name + ".db" ).c_str() , &pSql );
	if ( pSql != NULL )
	{
		SqliteCursor *p = SqliteQurey( pSql , "select * from " + tab_name + " where id = " + TransToSqlString( pBook->GetKeyNameWithMenuID() ) );
		if ( p != NULL )
		{
			if ( p->GetRowCount() > 0 )
			{
				if ( pOut == NULL )
				{
					delete p;
					sqlite3_close( pSql );
					return true;
				}
				else
				{
					CSJson::Reader reader;
					CSJson::Value root;
					if (reader.parse(p->GetData( 1 ), root))
					{
						*pOut = root;
						delete p;
						sqlite3_close( pSql );
						return true;
					}
				}
			}
		}
		delete p;
		sqlite3_close( pSql );
	}
	return false;
}
std::vector<BookData> BookDB::GetLastReadBook()
{
	std::vector<BookData> vec;
	sqlite3 *pSql = NULL;
	std::string tab_name = "last_read";
	sqlite3_open( (CCFileUtils::sharedFileUtils()->getWritablePath() + tab_name + ".db" ).c_str() , &pSql );
	if ( pSql != NULL )
	{
		SqliteCursor *p = SqliteQurey( pSql , "select * from " + tab_name );
		if ( p != NULL )
		{
			for ( int i = p->GetRowCount() - 1 ; i >= 0 ; i -- )
			{
				p->SetPos( i );
				BookData bookData;
				bookData.FromJsonString( p->GetData( 1 ) );
				vec.push_back( bookData );
			}
			delete p;
		}
		sqlite3_close( pSql );
	}
	return vec;
}
bool BookDB::FindLastReadBook( BookData *pOut , std::string strBookKey )
{
	bool ret = false;
	std::vector<BookData> vec;
	sqlite3 *pSql = NULL;
	std::string tab_name = "last_read";
	sqlite3_open( (CCFileUtils::sharedFileUtils()->getWritablePath() + tab_name + ".db" ).c_str() , &pSql );
	if ( pSql != NULL )
	{
		SqliteCursor *p = SqliteQurey( pSql , "select * from " + tab_name + " where id = " + TransToSqlString( strBookKey ) );
		if ( p != NULL )
		{
			if ( p->GetRowCount() > 0 )
			{
				pOut->FromJsonString( p->GetData( 1 ) );
				ret = true;
			}
		}
		delete p;
		sqlite3_close( pSql );
	}
	return ret;
}
void BookDB::AddLastReadBook( BookData *pData )
{
	m_strBookDataForGetReadStatusKey.clear();
	sqlite3 *pSql = NULL;
	BookData book = *pData;
	//book.m_vecMenu.clear();
	pData = &book;
	std::string tab_name = "last_read";
	sqlite3_open( (CCFileUtils::sharedFileUtils()->getWritablePath() + tab_name + ".db").c_str() , &pSql );
	if ( pSql != NULL )
	{
		// 创建表
		SqliteQurey( pSql , "create table if not exists " + tab_name + " (id text primary key, data text)" );
		// 如果当前已存在这本名，则删除
		SqliteCursor *p = SqliteQurey( pSql , "select * from " + tab_name + " where id = " + TransToSqlString( pData->GetKeyName() ) );
		if ( p != NULL && p->GetRowCount() > 0 )
		{
			BookData book2;
			book2.FromJsonString( p->GetData( 1 ) );
			for ( int i = 0 ; i < (int)book2.m_vecMenu.size() ; i ++ )
				if ( book2.m_vecMenu[i].m_bReaded ) book.m_vecMenu[i].m_bReaded = true;
			SqliteQurey( pSql , "delete from " + tab_name + " where id = " + TransToSqlString( pData->GetKeyName() ) );
			delete p;
		}
		// 添加新行
		SqliteQurey( pSql , "insert into " + tab_name + " (id, data) values(" + TransToSqlString( pData->GetKeyName() ) + "," + TransToSqlString( pData->GetJsonString() ) + ")" );
		sqlite3_close( pSql );
	}
}
std::string BookDB::TransToSqlString( std::string str )
{
	std::string ret;
	for ( int i = 0 ; str[i] ; i ++ )
	{
		if ( str[i] == '"' ) ret += "\"";
		ret += str[i];
	}
	return "\"" + ret + "\"";
}
SqliteCursor *BookDB::SqliteQurey( sqlite3 *pSql , std::string sql )
{
	char *errMsg = NULL;
	SqliteCursor *pCursor = new SqliteCursor();
	int result = sqlite3_exec( pSql, sql.c_str() , QueryResult, pCursor , &errMsg );  
	if ( result != SQLITE_OK && errMsg != NULL )
	{
		delete pCursor;
		return NULL;
	}
	if ( pCursor->GetRowCount() == 0 )
	{
		delete pCursor;
		return NULL;
	}
	return pCursor;
}
Setting BookDB::GetSetting()
{
	sqlite3 *pSql = NULL;
	std::string tab_name = "setting";
	sqlite3_open( (CCFileUtils::sharedFileUtils()->getWritablePath() + tab_name + ".db").c_str() , &pSql );
	if ( pSql != NULL )
	{
		// 创建表
		SqliteQurey( pSql , "create table if not exists " + tab_name + " (id text primary key, data text)" );
		SqliteCursor *p = SqliteQurey( pSql , "select * from " + tab_name + " where id = " + TransToSqlString( "1" ) );
		if ( p != NULL && p->GetRowCount() > 0 )
		{
			Setting data;
			CSJson::Reader reader;
			CSJson::Value root;
			if (reader.parse(p->GetData( 1 ), root))
				data.ReadFromJson( root );
			delete p;
			return data;
		}
	}
	return Setting();
}
void BookDB::SetSetting( Setting data )
{
	sqlite3 *pSql = NULL;
	std::string tab_name = "setting";
	sqlite3_open( (CCFileUtils::sharedFileUtils()->getWritablePath() + tab_name + ".db").c_str() , &pSql );
	if ( pSql != NULL )
	{
		// 创建表
		SqliteQurey( pSql , "create table if not exists " + tab_name + " (id text primary key, data text)" );
		SqliteCursor *p = SqliteQurey( pSql , "select * from " + tab_name + " where id = " + TransToSqlString( "1" ) );
		if ( p != NULL && p->GetRowCount() > 0 )
		{
			SqliteQurey( pSql , "delete from " + tab_name + " where id = " + TransToSqlString( "1" ) );
			delete p;
		}
		// 添加新行
		SqliteQurey( pSql , "insert into " + tab_name + " (id, data) values(" + TransToSqlString( "1" ) + "," + TransToSqlString( data.GetJsonString() ) + ")" );
		sqlite3_close( pSql );
	}
}