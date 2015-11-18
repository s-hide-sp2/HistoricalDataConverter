#include "StdAfx.h"
#include "hdcUtility.h"

static const TCHAR s_szDateSep[] = _T("/.-_");
static const TCHAR s_szTimeSep[] = _T(":");


void hdcUtility::GetTokens( 
	vecString& vecToken,	//(o)トークン
	const CString& strData,	//(i)データ
	LPCTSTR lpszSep			//(i)区切り文字
	)
{
	CString strToken;
	int curPos= 0;

	strToken = strData.Tokenize( lpszSep, curPos );
	while( !strToken.IsEmpty() ){
		vecToken.push_back( strToken );
		strToken = strData.Tokenize( lpszSep, curPos );
	}
}

//	時間文字列を分解し時間オブジェクトを返す(DD/MM/YY,HH:mm:ss)
Hdc::Result hdcUtility::GetTimeDDMMYY_HHMMSS( 
	hdcTime& time, 
	const CString& strDateTime
	)
{
	CString strDate, strTime;
	vecString vecToken;
	int nDate[3];
	int nTime[3];
	int n = 0;
	int nSepLoc = strDateTime.Find( _T(' ') );
	//static const TCHAR szDateSep[] = _T("/.");
	//static const TCHAR szTimeSep[] = _T(":");

	strDate = strDateTime.Left( nSepLoc );
	strTime = strDateTime.Mid( nSepLoc+1 );

	vecToken.reserve(3);
	hdcUtility::GetTokens( vecToken, strDate, s_szDateSep );

	if( vecToken.size() != 3 )
		return Hdc::rFail;

	for each( const auto& token in vecToken )
		nDate[n++] = _ttoi(token);
	
	n = 0;
	vecToken.clear();
	vecToken.reserve(3);
	hdcUtility::GetTokens( vecToken, strTime, s_szTimeSep );

	if( vecToken.size() != 3 )
		return Hdc::rFail;

	for each( const auto& token in vecToken )
		nTime[n++] = _ttoi(token);

	time.SetDateTime( 2000 + nDate[2], nDate[1], nDate[0], nTime[0], nTime[1], nTime[2] );

	return Hdc::rOk;
}

//	時間文字列を分解し時間オブジェクトを返す(YYYY/MM/DD,HH:mm:ss)
Hdc::Result hdcUtility::GetTimeYYYYMMDD_HHMMSS( 
	hdcTime& time, 
	const CString& strDate,
	const CString& strTime
	)
{
	vecString vecToken;
	int nDate[3] = {0,0,0};
	int nTime[3] = {0,0,0};
	int n = 0;

	vecToken.reserve(3);
	hdcUtility::GetTokens( vecToken, strDate, s_szDateSep );

	if( vecToken.size() != 3 )
		return Hdc::rFail;

	for each( const auto& token in vecToken )
		nDate[n++] = _ttoi(token);
	
	vecToken.clear();
	hdcUtility::GetTokens( vecToken, strTime, s_szTimeSep );

	if( vecToken.size() < 2 )
		return Hdc::rFail;
	
	n = 0;
	for each( const auto& token in vecToken )
		nTime[n++] = _ttoi(token);

	time.SetDateTime( nDate[0], nDate[1], nDate[2], nTime[0], nTime[1], nTime[2] );

	return Hdc::rOk;
}

//	エラーメッセージを返す
CString hdcUtility::GetErrorMsg( Hdc::Result result )
{
	return _T("");
}

//	ログ出力
bool hdcUtility::WriteLog( 
	LPCTSTR lpszFilePath,	//(i)ファイルパス
	LPCTSTR lpszLog,		//(i)出力内容
	...						//(i)フォーマット引数
	)
{
	bool bResult = true;
	FILE* fp = _tfopen( lpszFilePath, _T("a") );	

	if( fp == NULL )
		return false;
	
	va_list	ap;
	TCHAR* lpszBuff = NULL;
	int nLen = 0;

	va_start(ap, lpszLog);
	nLen = _vsctprintf( lpszLog, ap ) + 1;
	try{
		lpszBuff = new TCHAR[nLen];

		_vstprintf( lpszBuff, nLen, lpszLog, ap );
		_ftprintf( fp, lpszBuff );
		va_end(ap);
		delete [] lpszBuff;
		lpszBuff = nullptr;
	}
	catch(...){
		bResult = false;
	}

	fclose(fp);

	return bResult;
}
