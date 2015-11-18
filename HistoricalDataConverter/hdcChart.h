#pragma once

#include "hdcBar.h"

struct hdcTerm{
	hdcTime m_timeBegin;
	hdcTime m_timeEnd;

	hdcTerm(const hdcTime& begin, const hdcTime& end) : m_timeBegin(begin), m_timeEnd(end){}

	bool IsIn( const hdcTime& time ) const
	{
		return ( m_timeBegin <= time && time <= m_timeEnd );
	}
};

class hdcChart
{
public:
	hdcChart(void);
	virtual ~hdcChart(void);

	//	チャートデータを生成する
	Hdc::Result Generate(LPCTSTR lpszSrcPath, int nSrcPeriod, LPCTSTR lpszOutPath, int nOutputPeriod, bool bSkipFirstRow);

	//	ティックデータを元にチャートデータを生成する
	Hdc::Result GenerateFromTick(LPCTSTR lpszSrcPath, LPCTSTR lpszOutPathBid, LPCTSTR lpszOutPathAsk, int nOutputPeriod);

	//	抽出する
	Hdc::Result Extract( LPCTSTR lpszSrcPath, LPCTSTR lpszOutPath, const hdcTime& timeBegin, const hdcTime& timeEnd, Hdc::TimeFormat timeFormat );

	///////////////////////////////////////////////////
	//
	//	Get/Set

	void SetSymbol( LPCTSTR value );
	LPCTSTR Symbol() const;

	void SetPeriod(int value);
	int Period() const;

	void SetOutputPeriod(int value);
	int OutputPeriod() const;

	void SetShiftTime(int nTotalMinutes);
	const hdcTimeSpan& ShiftTime() const;

protected:

	//	次バー日時判定
	Hdc::Result IsNextBarTime( bool& bNext, const hdcTime& time ) const;

	//	作業用バーにデータを追加する
	Hdc::Result AddBarTemp( bool bNextBarTime, const hdcTime& time, double dBid, double dAsk);

	//	作業用バーにデータを追加する
	Hdc::Result AddBarTemp( bool bNextBarTime, const hdcTime& time, const double dRates[] );

	//	バーを追加する
	void AddBar( const hdcBar& bar );

	//	ヘッダー出力
	Hdc::Result WriteHeader( CStdioFile& cf );

	//	バーデータ出力
	Hdc::Result Write( CStdioFile& cf, const hdcBar& bar, Hdc::BarKind barKind );

	//	欠損データを埋める
	//Hdc::Result FillLackData( hdcTime& timeNext, const hdcTime& timeCur, CStdioFile& cfBid, CStdioFile& cfAsk );

	//	プライスの小数桁数を返す
	int Decimal() const;

	//	時間枠の時間間隔を返す
	hdcTimeSpan GetPeriodSpan(int nPeriod) const;

	//	バーオープン日時を返す
	hdcTime GetOpenTime(const hdcTime& time, int nPeriod) const;

	//	次バーのオープン日時を返す
	bool GetNextOpenTime(hdcTime& timeNextOpen) const;

	//	ログ出力期間初期化
	void InitOutputLogTerm();

	//	ログ出力判定
	bool IsOutputLog( const hdcTime& time ) const;

	//	ログ出力ファイルパスを返す
	CString OutputLogFilePath() const;

private:
	//	通貨ペア
	CString m_strSymbol;

	//	時間枠
	int m_nPeriod;

	//	チャートデータ生成時間枠
	int m_nOutputPeriod;

	//	シフト時間
	hdcTimeSpan m_spanShift;

	//	プライスの小数桁数
	int m_nDecimal;

	//	バーマップ
	mapBar m_mapBar;

	//	チャートデータ生成用作業バー
	hdcBar m_barTemp;

	//	ログ出力期間
	lstTerm m_termOutputLog;
};

inline LPCTSTR hdcChart::Symbol() const
{
	return m_strSymbol;
}

inline void hdcChart::SetPeriod(int value)
{
	m_nPeriod = value;
}

inline int hdcChart::Period() const
{
	return m_nPeriod;
}

inline void hdcChart::SetOutputPeriod(int value)
{
	m_nOutputPeriod = value;
}

inline int hdcChart::OutputPeriod() const
{
	return m_nOutputPeriod;
}

//	バーを追加する
inline void hdcChart::AddBar( const hdcBar& bar )
{
	m_mapBar[bar.Time()] = bar;
}

//	プライスの小数桁数を返す
inline int hdcChart::Decimal() const
{
	return m_nDecimal;
}

inline const hdcTimeSpan& hdcChart::ShiftTime() const
{
	return m_spanShift;
}

//	ログ出力ファイルパスを返す
inline CString hdcChart::OutputLogFilePath() const
{
	CString str;

	str.Format( _T("L:\\SVNProjects\\HistoricalDataConverter\\Log\\%s-%d.log"), Symbol(), Period() );
	return str;
}

