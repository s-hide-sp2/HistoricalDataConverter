#pragma once
#include "hdccommand.h"
class hdcCmdExtract :
	public hdcCommand
{
public:
	hdcCmdExtract(void);
	virtual ~hdcCmdExtract(void);

	//////////////////////////////////////////////////
	//
	//	Get/Set

	void SetTimeBegin(const hdcTime& value);
	const hdcTime& TimeBegin() const;

	void SetTimeEnd(const hdcTime& value);
	const hdcTime& TimeEnd() const;

	void SetTimeFormat(Hdc::TimeFormat value);
	Hdc::TimeFormat TimeFormat() const;

protected:

	//	コマンドを実行する
	virtual Hdc::Result Execute();

	//	コマンドを実行する
	virtual Hdc::Result Execute(const CString& strPath, const CString& strFileName, const CString& strTitle );

private:

	//	期間文字を返す
	CString GetTermString() const;

private:

	hdcTime m_timeBegin;

	hdcTime m_timeEnd;

	Hdc::TimeFormat m_timeFormat;
};

inline void hdcCmdExtract::SetTimeBegin(const hdcTime& value)
{
	m_timeBegin = value;
}

inline const hdcTime& hdcCmdExtract::TimeBegin() const
{
	return m_timeBegin;
}

inline void hdcCmdExtract::SetTimeEnd(const hdcTime& value)
{
	m_timeEnd = value;
}

inline const hdcTime& hdcCmdExtract::TimeEnd() const
{
	return m_timeEnd;
}

inline void hdcCmdExtract::SetTimeFormat(Hdc::TimeFormat value)
{
	m_timeFormat = value;
}

inline Hdc::TimeFormat hdcCmdExtract::TimeFormat() const
{
	return m_timeFormat;
}

