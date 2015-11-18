#pragma once
class hdcUtility
{
public:
	//	文字をトークンに分解
	static void GetTokens( vecString& vecToken, const CString& strData, LPCTSTR lpszSep );

	//	時間文字列を分解し時間オブジェクトを返す(DD/MM/YY,HH:mm:ss)
	static Hdc::Result GetTimeDDMMYY_HHMMSS( hdcTime& time, const CString& strDateTime );

	//	時間文字列を分解し時間オブジェクトを返す(YYYY/MM/DD,HH:mm:ss)
	static Hdc::Result GetTimeYYYYMMDD_HHMMSS( hdcTime& time, const CString& strDate, const CString& strTime );

	//	エラーメッセージを返す
	static CString GetErrorMsg( Hdc::Result result );

	//	ログ出力
	static bool WriteLog( LPCTSTR lpszFilePath, LPCTSTR lpszLog, ...);

};

