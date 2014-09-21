//  Copyright (c) 2014 http://9miao.com All rights reserved.
// 9��С˵�Ķ����������� http://www.9miao.com/group-40-59173.html
#ifndef __BOOKDB_H__
#define __BOOKDB_H__
#include <list>
#include "Book9s.h"
//#include "cocos2d.h"
//#include "cocos-ext.h"
//USING_NS_CC_EXT; 
//USING_NS_CC;

typedef void (CAObject::*SEL_BookDBOnComplete)( void *pData );
#define BookDB_selector(_SELECTOR) (SEL_BookDBOnComplete)(&_SELECTOR)
class BookDBHttpRequest;
typedef void (CAObject::*SEL_BookDBOnHttpComplete)( BookDBHttpRequest *pRequest , CSJson::Value *pJson );
#define BookDB_httpselector(_SELECTOR) (SEL_BookDBOnHttpComplete)(&_SELECTOR)

// Http���󷽷�����¼�ص�����
class BookDBHttpRequest : public CCHttpRequest
{
public:
	CAObject *pTarget; // �ص�Ŀ��
	SEL_BookDBOnComplete onComplete; // �ص�����
	SEL_BookDBOnHttpComplete onHttpComplete; // HTTP��ɺ���
	void *pUserData; // �û�����

	BookDBHttpRequest( CAObject *pTarget , SEL_BookDBOnComplete onComplete , SEL_BookDBOnHttpComplete onHttpComplete , void *pUserData )
	{
		this->pTarget = pTarget;
		this->onComplete = onComplete;
		this->onHttpComplete = onHttpComplete;
		this->pUserData = pUserData;
	}
};
// ϵͳ������
class Setting
{
public:
	int m_nTextColor;// ������ɫ
	int m_nBkColor;  // ������ɫ
	int m_nTextSize; // ���ִ�С

	Setting();
	CAColor4B IntToCCC4( int data )
	{
		return ccc4( (data >> 16) & 0xFF , (data >> 8) & 0xFF , data & 0xFF , (data >> 24) & 0xFF );
	}

	// ��json���л�
	void ReadFromJson( CSJson::Value &node )
	{
		m_nTextColor = node["TextColor"].asInt();
		m_nBkColor = node["BkColor"].asInt();
		m_nTextSize = node["TextSize"].asInt();
	}
	// ���л���json
	CSJson::Value WriteToJson()
	{
		CSJson::Value root;
		root["TextColor"] = m_nTextColor;
		root["BkColor"] = m_nBkColor;
		root["TextSize"] = m_nTextSize;
		return root;
	}
	// ���л����ַ���
	std::string GetJsonString()
	{
		CSJson::FastWriter fast_writer;
		return fast_writer.write( WriteToJson() );
	}
	// ���������ɫ����
	int GetTextColorIndex();
	// ����������ɫ����
	void SetTextColorIndex( int idx );
	// ������ִ�С����
	int GetTextSizeIndex();
	// �������ִ�С����
	void SetTextSizeIndex( int idx );
	// ��ñ�����ɫ����
	int GetBkColorIndex();
	// ���ñ�����ɫ����
	void SetBkColorIndex( int idx );
};
// ͼ��Ŀ¼��
class BookMenu
{
public:
	int m_nGuid;// uid
	std::string m_strBookMenu; // Ŀ¼����
	std::string m_strTextUrl;  // ����URL
	bool m_bReaded;// �Ƿ��Ѷ�

	BookMenu()
	{
		m_nGuid = 0;
		m_bReaded = false;
	}
	// ��JSON���л�
	void ReadFromJson( CSJson::Value &node )
	{
		m_strBookMenu = node["chapter_name"].asString();
		m_strTextUrl = node["chapter_url"].asString();
		m_nGuid = atoi( node["menu_guid"].asString().c_str() );
		m_bReaded = node["readed"].asBool();
	}
	// ���л���JSON
	CSJson::Value WriteToJson()
	{
		CSJson::Value root;
		root["chapter_name"] = m_strBookMenu;
		root["chapter_url"] = m_strTextUrl;
		char buf[32]; snprintf( buf , 32 , "%d" , m_nGuid );
		root["menu_guid"] = buf;
		root["readed"] = m_bReaded;
		return root;
	}
};
// ͼ������
class BookData
{
public:
	int m_nBookGuid; // UID
	std::string m_strBookInfo; // ͼ����
	std::string m_strBookWordSize; // ͼ����������
	std::string m_strBookName; // ͼ������
	std::string m_strBookAuthor; // ͼ������
	std::string m_strLastChapter; // �������½�
	std::string m_strLastUpdateDate; // ����������
	std::string m_strSourceNetName; // ��Դ��վ
	std::string m_strMenuUrl; // Ŀ¼URL
	std::string m_strInfoUrl; // ���URL
	std::string m_strImageUrl; // ͼ��URL
	std::vector<BookMenu> m_vecMenu; // ͼ��Ŀ¼
	int m_nLastReadMenu; // ����Ķ��½�

	BookData()
	{
		m_nBookGuid = 0;
		m_nLastReadMenu = 0;
	}
	// ��JSON���л�
	void ReadFromJson( CSJson::Value &node , bool bWithMenu = true )
	{
		m_strBookInfo = node["book_intro"].asString(); // ���
		m_strBookWordSize = node["book_words"].asString(); // ����
		m_strBookName = node["book_name"].asString(); // ����
		m_strBookAuthor = node["book_author"].asString(); // ����
		m_strLastChapter = node["latest_chapter"].asString(); // �½�
		m_strLastUpdateDate = node["update_date"].asString();
		m_strImageUrl = node["img_url"].asString();
		m_strInfoUrl = node["intro_url"].asString();
		m_strMenuUrl = node["cat_url"].asString();
		m_strSourceNetName = node["book_source_site"].asString(); // ��Դ��վ��
		m_nLastReadMenu = node["last_read_menu"].asInt();
		m_vecMenu.resize( node["menu_list"].size() );
		if ( bWithMenu )
		{
			for ( int i = 0 ; i < (int)m_vecMenu.size() ; i ++ )
				m_vecMenu[i].ReadFromJson( node["menu_list"][i] );
		}
	}
	// ���л���JSON
	CSJson::Value WriteToJson()
	{
		CSJson::Value root;
		root["book_intro"] = m_strBookInfo;
		root["book_words"] = m_strBookWordSize;
		char buf[32]; snprintf( buf , 32 , "%d" , m_nBookGuid );
		root["book_guid"] = buf;
		root["book_name"] = m_strBookName;
		root["book_author"] = m_strBookAuthor;
		root["latest_chapter"] = m_strLastChapter;
		root["update_date"] = m_strLastUpdateDate;
		root["img_url"] = m_strImageUrl;
		root["intro_url"] = m_strInfoUrl;
		root["cat_url"] = m_strMenuUrl;
		root["book_source_site"] = m_strSourceNetName;
		root["last_read_menu"] = m_nLastReadMenu;
		for ( int i = 0 ; i < m_vecMenu.size() ; i ++ )
		{
			root["menu_list"].append( m_vecMenu[i].WriteToJson() );
		}
		return root;
	}
	// �����ȫ����
	std::string GetKeyName()
	{
		return m_strBookName + m_strBookAuthor + m_strSourceNetName;
	}
	// ���Ŀ¼ȫ����
	std::string GetKeyNameWithMenuID()
	{
		char buf[32];
		snprintf( buf , 32 , "%d" , m_nLastReadMenu );
		return m_strBookName + m_strBookAuthor + m_strSourceNetName + buf;
	}
	// ���л����ַ���
	std::string GetJsonString()
	{
		CSJson::FastWriter fast_writer;
		return fast_writer.write( WriteToJson() );
	}
	// ���ַ������л�
	void FromJsonString( std::string str , bool bWithMenu = true )
	{
		CSJson::Reader reader;
		CSJson::Value root;
		if (reader.parse(str, root))
			ReadFromJson( root , bWithMenu );
	}
};
// ���ݿ��α�
class sqlite3;
class SqliteCursor
{
	int m_nPos;
	std::list<std::vector<std::string>*> m_listvecData;
	std::vector<std::vector<std::string>*> m_vecvecData;
	std::vector<std::string> m_vecTitle;
public:
	SqliteCursor()
	{
		m_nPos = 0;
	}
	~SqliteCursor()
	{
		for ( std::list<std::vector<std::string>*>::iterator it = m_listvecData.begin() ; it != m_listvecData.end() ; it ++ )
			delete (*it);
	}
	void AddRowData( int n_column, char ** column_value, char ** column_name )
	{
		if ( m_listvecData.size() == 0 )
		{
			m_vecTitle.resize( n_column );
			for ( int i = 0 ; i < n_column ; i ++ ) m_vecTitle[i] = column_name[i];
		}
		std::vector<std::string>* vec = new std::vector<std::string>();
		vec->resize( n_column );
		for ( int i = 0 ; i < n_column ; i ++ ) if ( column_value[i] != NULL ) (*vec)[i] = column_value[i];
		m_listvecData.push_back( vec );
	}
	void MakeData()
	{
		if ( m_vecvecData.size() != m_listvecData.size() )
		{
			m_vecvecData.resize( m_listvecData.size() );
			int i = 0;
			for ( std::list<std::vector<std::string>*>::iterator it = m_listvecData.begin() ; it != m_listvecData.end() ; it ++ , i ++ )
				m_vecvecData[i] = (*it);
		}
	}
	int GetRowCount()
	{
		return (int)m_listvecData.size();
	}
	int GetColumnCount()
	{
		return (int)m_vecTitle.size();
	}
	std::vector<std::string> &GetCurRow()
	{
		return (*m_vecvecData[m_nPos]);
	}
	const char* GetData( int column )
	{
		MakeData();
		std::vector<std::string> &row = GetCurRow();
		if ( column < 0 || column >= (int)row.size() ) return "";
		return row[column].c_str();
	}
	const char* GetColumData( int column )
	{
		if ( column < 0 || column >= (int)m_vecTitle.size() ) return "";
		return m_vecTitle[column].c_str();
	}
	int GetPos()
	{
		return m_nPos;
	}
	bool SetPos( int pos )
	{
		if ( GetRowCount() == 0 ) return false;
		MakeData();
		if ( pos < 0 ) pos = 0;
		else if ( pos > GetRowCount() - 1 ) pos = GetRowCount() - 1;
		m_nPos = pos;
		return true;
	}
};
// ͼ�����ݿ�
class BookDB : public CAObject
{
	static BookDB *m_pInstance;
	BookData m_currentGetBookMenu;
	std::map<int , CCHttpClient*> m_mapHttpClient;
	std::map<std::string , int> m_mapDownloading;
	int m_nDownloadingMaxNum;
	BookData m_bookDataForGetReadStatus;
	std::string m_strBookDataForGetReadStatusKey;

	void HttpRequest( std::string url , CAObject *pTarget , SEL_BookDBOnComplete onComplete , SEL_BookDBOnHttpComplete onHttpComplete , void *pUserData = NULL , int nThread = 0 );
	void HttpFinished( CCHttpClient* client, CCHttpResponse* response );
	SqliteCursor *SqliteQurey( sqlite3 *pSql , std::string sql );
	static int QueryResult( void * para, int n_column, char ** column_value, char ** column_name )  
	{
		((SqliteCursor*)para)->AddRowData(n_column , column_value , column_name );
		return 0;
	}
	std::string TransToSqlString( std::string str );
public:
	BookDB();
	~BookDB();
	static BookDB* Instance();
	static void Release();
	std::string GetUrlHeader();
	// ������
	void OpenRank( CAObject *pTarget , SEL_BookDBOnComplete onComplete );
	void OpenRankFinished( BookDBHttpRequest *pRequest , CSJson::Value *pJson );

	// ������
	void Search( std::string strSearchKey , std::string strSearchType , CAObject *pTarget , SEL_BookDBOnComplete onComplete );
	void SearchFinished( BookDBHttpRequest *pRequest , CSJson::Value *pJson );

	// ��ͼ����
	void GetBookInfo( BookData *pBook , CAObject *pTarget , SEL_BookDBOnComplete onComplete );
	void GetBookInfoFinished( BookDBHttpRequest *pRequest , CSJson::Value *pJson );

	// ��Ŀ¼
	void GetBookMenu( BookData *pBook , CAObject *pTarget , SEL_BookDBOnComplete onComplete );
	int GetBookMenuDownloadStatus( BookData *pBook , int menu );
	int GetBookMenuReadStatus( BookData *pBook , int menu );
	void GetBookMenuFinished( BookDBHttpRequest *pRequest , CSJson::Value *pJson );

	// ����ͼ��
	void DownloadBook( BookData *pBook , int start_menu , int end_menu , CAObject *pTarget , SEL_BookDBOnComplete onComplete );
	void DownloadBookFinished( BookDBHttpRequest *pRequest , CSJson::Value *pJson );

	// �Ķ�ͼ��
	void ReadBook( BookData *pBook , int menu , CAObject *pTarget , SEL_BookDBOnComplete onComplete );
	void ReadBookFinished( BookDBHttpRequest *pRequest , CSJson::Value *pJson );

	// ɾ��ͼ��
	void DeleteBook( BookData *pBook );

	// ����Ķ�����ͼ��
	std::vector<BookData> GetLastReadBook();
	bool FindLastReadBook( BookData *pOut , std::string strBookKey );
	void AddLastReadBook( BookData *pData );
	bool FindStorage( std::string strBookKey , BookData *pOut );
	bool FindStorageBookData( BookData *pBook , CSJson::Value *pOut );
	std::vector<BookData> GetStorage();
	int GetDownloadingNum(){ return (int)m_mapDownloading.size(); }
	int GetDownloadingMaxNum(){ return m_nDownloadingMaxNum; }

	// ��ϵͳ����
	Setting GetSetting();
	void SetSetting( Setting data );
};

#endif