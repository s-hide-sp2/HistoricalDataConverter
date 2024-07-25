#include "StdAfx.h"
#include "hdcUtility.h"

static const TCHAR s_szDateSep[] = _T("/.-_");
static const TCHAR s_szTimeSep[] = _T(":");


void hdcUtility::GetTokens( 
	vecString& vecToken,	//(o)�g�[�N��
	const CString& strData,	//(i)�f�[�^
	LPCTSTR lpszSep			//(i)��؂蕶��
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

//	���ԕ�����𕪉������ԃI�u�W�F�N�g��Ԃ�(DD/MM/YY,HH:mm:ss)
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

//	���ԕ�����𕪉������ԃI�u�W�F�N�g��Ԃ�(YYYY/MM/DD,HH:mm:ss)
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
	bool isNoSep = false;

	vecToken.reserve(3);

	if (strDate.GetLength() == 8) {
		vecToken.push_back(strDate.Left(4));
		vecToken.push_back(strDate.Mid(4,2));
		vecToken.push_back(strDate.Right(2));
		isNoSep = true;
	}
	else {
		hdcUtility::GetTokens(vecToken, strDate, s_szDateSep);
	}

	if( vecToken.size() != 3 )
		return Hdc::rFail;

	for each( const auto& token in vecToken )
		nDate[n++] = _ttoi(token);
	
	vecToken.clear();

	if (isNoSep) {
		if (strTime.GetLength() == 6) {
			vecToken.push_back(strTime.Left(2));
			vecToken.push_back(strTime.Mid(2, 2));
			vecToken.push_back(strTime.Right(2));
		}
		else {
			return Hdc::rFail;
		}
	}
	else {
		hdcUtility::GetTokens(vecToken, strTime, s_szTimeSep);
	}

	if( vecToken.size() < 2 )
		return Hdc::rFail;
	
	n = 0;
	for each( const auto& token in vecToken )
		nTime[n++] = _ttoi(token);

	time.SetDateTime( nDate[0], nDate[1], nDate[2], nTime[0], nTime[1], nTime[2] );

	return Hdc::rOk;
}

//	�G���[���b�Z�[�W��Ԃ�
CString hdcUtility::GetErrorMsg( Hdc::Result result )
{
	return _T("");
}

//	���O�o��
bool hdcUtility::WriteLog( 
	LPCTSTR lpszFilePath,	//(i)�t�@�C���p�X
	LPCTSTR lpszLog,		//(i)�o�͓��e
	...						//(i)�t�H�[�}�b�g����
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
