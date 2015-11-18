#pragma once
#include "hdccommand.h"
class hdcCmdConvertPeriod :
	public hdcCommand
{
public:
	hdcCmdConvertPeriod(void);
	virtual ~hdcCmdConvertPeriod(void);

	//////////////////////////////////////////////////
	//
	//	Get/Set

	void SetPeriod( int value );
	int Period() const;

	void SetOutputPeriod( int value );
	int OutputPeriod() const;

	void SetShiftTime( int value );
	int ShiftTime() const;

	//	先頭行読み飛ばしフラグ
	bool SkipFirstRow() const;
	void SetSkipFirstRow(bool value);

protected:

	//	コマンドを実行する
	virtual Hdc::Result Execute();

	//	コマンドを実行する
	virtual Hdc::Result Execute(const CString& strPath, const CString& strFileName, const CString& strTitle );

private:

	//	時間枠を変換する
	Hdc::Result ConvertTimePeriod( const CString& strPath, const CString& strFileName, const CString& strSymbol ) const;

	//	ティックデータを基に時間枠を変換する
	Hdc::Result ConvertTimePeriodFromTick( const CString& strPath, const CString& strFileName, const CString& strSymbol ) const;

private:

	//	時間枠
	int m_nPeriod;

	//	出力時間枠
	int m_nOutputPeriod;

	//	シフト時間
	int m_nShiftTime;

	//	先頭行読み飛ばしフラグ
	bool m_bSkipFirstRow;
};

inline void hdcCmdConvertPeriod::SetPeriod( int value )
{
	m_nPeriod = value;
}

inline int hdcCmdConvertPeriod::Period() const
{
	return m_nPeriod;
}

inline void hdcCmdConvertPeriod::SetOutputPeriod( int value )
{
	m_nOutputPeriod = value;
}

inline int hdcCmdConvertPeriod::OutputPeriod() const
{
	return m_nOutputPeriod;
}

inline void hdcCmdConvertPeriod::SetShiftTime( int value )
{
	m_nShiftTime = value;
}

inline int hdcCmdConvertPeriod::ShiftTime() const
{
	return m_nShiftTime;
}

inline bool hdcCmdConvertPeriod::SkipFirstRow() const
{
	return m_bSkipFirstRow;
}

inline void hdcCmdConvertPeriod::SetSkipFirstRow(bool value)
{
	m_bSkipFirstRow = value;
}
