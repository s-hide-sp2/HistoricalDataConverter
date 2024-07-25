#include "StdAfx.h"
#include "hdcUtility.h"
#include "hdcChart.h"
#include "hdcExcludedPeriodManager.h"

hdcChart::hdcChart(void)
	: m_nPeriod(1)
	, m_nOutputPeriod(1)
	, m_nDecimal(1)
{
}


hdcChart::~hdcChart(void)
{
}

//	�`���[�g�f�[�^�𐶐�����
Hdc::Result hdcChart::Generate(
	LPCTSTR lpszSrcPath,	//(i)���f�[�^�t�@�C���p�X
	int nSrcPeriod,			//(i)���Ԙg
	LPCTSTR lpszOutPath,	//(i)�o�̓f�[�^�t�@�C���p�X
	int nOutputPeriod,		//(i)�o�͎��Ԙg
	bool bSkipFirstRow,		//(i)�擪�s�ǂݔ�΂��t���O
	Hdc::DataForamt dataFormat	//(i)�f�[�^�t�H�[�}�b�g
	)
{
	Hdc::Result result = Hdc::rOk;

	SetPeriod( nSrcPeriod );
	SetOutputPeriod( nOutputPeriod );

	if( nSrcPeriod == 0 )
		return Hdc::rCancel;

	try{
		CStdioFile cfr( lpszSrcPath, CFile::modeRead | CFile::typeText );
		CStdioFile cfw( lpszOutPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText );
		CString strData;
		const TCHAR szSep[] = _T(",\t");
		vecString vecToken;
		hdcTime time, timePrev;
		const int shift = (Hdc::DataForamt::Normal == dataFormat) ? 0 : 1;

		InitOutputLogTerm();

		vecToken.reserve(6);
		
		if( bSkipFirstRow )
			cfr.ReadString( strData );

		while( cfr.ReadString( strData ) && result == Hdc::rOk ){
			bool bNext = false;

			vecToken.clear();
			hdcUtility::GetTokens( vecToken, strData, szSep );
			if( vecToken.size() < 6 ){
				hdcUtility::WriteLog( OutputLogFilePath(), _T("Invalid Data=%s\n"), strData );
				continue;
			}

			result = hdcUtility::GetTimeYYYYMMDD_HHMMSS( time, vecToken[shift], vecToken[shift + 1] );

			assert( result == Hdc::rOk );
			if( IsOutputLog(time) )
				hdcUtility::WriteLog( OutputLogFilePath(), _T("%s\n"), strData );

			time += ShiftTime();
			int nDay = time.GetDayOfWeek();
			if( nDay == Hdc::Sunday || nDay == Hdc::Saturday )
				continue;

			if( time < m_barTemp.Time() && time != m_barTemp.Time() ){
				hdcUtility::WriteLog( OutputLogFilePath(), _T("BarTempTime=%s SkipData=%s\n"), m_barTemp.Time().Format(HDC_FMT_YYYYMMDD_HHMMSS), strData );
				continue;
			}

			//	���o�[�f�[�^����
			result = IsNextBarTime( bNext, time );
			assert( result == Hdc::rOk );
#ifdef _DEBUG
			CString strDebug1 = time.Format( HDC_FMT_YYYYMMDD_HHMMSS );
#endif
			//	�`���[�g�f�[�^�o��
			if( result == Hdc::rOk && bNext ){
				result = Write( cfw, m_barTemp, Hdc::Bid );
				timePrev = m_barTemp.Time();
			}
			
			if( result == Hdc::rOk ){
				double dRates[hdcBar::NUM_OF_BAR_VALUE_KIND];
				int volume = (6 < vecToken.size()) ? _ttoi(vecToken[shift + 6]) : 1;

				for( int n = 0; n < hdcBar::NUM_OF_BAR_VALUE_KIND; n++ )
					dRates[n] = _ttof( vecToken[shift + 2 + n] );

				result = AddBarTemp( bNext, time, dRates, volume);
			}
		}

		//	�Ō�̃f�[�^���o�͂���
		if( result == Hdc::rOk && timePrev != m_barTemp.Time() && timePrev < m_barTemp.Time() ){
			result = Write( cfw, m_barTemp, Hdc::Bid );
		}
	}
	catch(...){
		result = Hdc::rFileOpenError;
	}

	return result;
}

//	�e�B�b�N�f�[�^�����Ƀ`���[�g�f�[�^�𐶐�����
Hdc::Result hdcChart::GenerateFromTick(
	LPCTSTR lpszSrcPath,	//(i)���f�[�^�t�@�C���p�X
	LPCTSTR lpszOutPathBid,	//(i)�o�̓f�[�^�t�@�C���p�X(Bid)
	LPCTSTR lpszOutPathAsk,	//(i)�o�̓f�[�^�t�@�C���p�X(Ask)
	int nOutputPeriod			//(i)�o�͎��Ԙg
	)
{
	Hdc::Result result = Hdc::rOk;
	COleDateTime time;

	try{
		CStdioFile cfr( lpszSrcPath, CFile::modeRead | CFile::typeText );
		CStdioFile cfwBid( lpszOutPathBid, CFile::modeCreate | CFile::modeWrite | CFile::typeText );
		CStdioFile cfwAsk( lpszOutPathAsk, CFile::modeCreate | CFile::modeWrite | CFile::typeText );
		CString strData;
		const TCHAR szSep[] = _T(",\t");
		vecString vecToken;
		hdcTime timePrev;
#ifdef _DEBUG
		int nDebugCnt = 0;
#endif
		SetPeriod(0);
		InitOutputLogTerm();
		
		while( cfr.ReadString( strData ) && result == Hdc::rOk ){
			hdcTime time;
			double dBid, dAsk;
			bool bNext = false;

			vecToken.clear();
			vecToken.reserve(3);
			hdcUtility::GetTokens( vecToken, strData, szSep );
			assert( vecToken.size() == 3 );

			if( vecToken.size() != 3 ){
				hdcUtility::WriteLog( OutputLogFilePath(), _T("Invalid Data=%s\n"), strData );
				continue;
			}

			result = hdcUtility::GetTimeDDMMYY_HHMMSS( time, vecToken[0] );
			assert( result == Hdc::rOk );
			if( IsOutputLog(time) )
				hdcUtility::WriteLog( OutputLogFilePath(), _T("%s\n"), strData );

			time += ShiftTime();
			int nDay = time.GetDayOfWeek();
			if( nDay == Hdc::Sunday || nDay == Hdc::Saturday )
				continue;

			if( time < m_barTemp.Time() && time != m_barTemp.Time() ){
				hdcUtility::WriteLog( OutputLogFilePath(), _T("BarTempTime=%s SkipData=%s\n"), m_barTemp.Time().Format(HDC_FMT_YYYYMMDD_HHMMSS), strData );
				continue;
			}
			
			dBid = _ttof( vecToken[1] );
			dAsk = _ttof( vecToken[2] );

			//	���o�[�f�[�^����
			result = IsNextBarTime( bNext, time );
			assert( result == Hdc::rOk );
#ifdef _DEBUG
			CString strDebug1 = time.Format( HDC_FMT_YYYYMMDD_HHMMSS );

			if( strDebug1 == _T("2011/12/30 23:50:00") || strDebug1 == _T("2011/12/30 23:55:00") ){
				int n = 0;
				n++;
			}
#endif

			//	�`���[�g�f�[�^�o��
			if( result == Hdc::rOk && bNext ){
				result = Write( cfwBid, m_barTemp, Hdc::Bid );
				if( result == Hdc::rOk )
					result = Write( cfwAsk, m_barTemp, Hdc::Ask );

				timePrev = m_barTemp.Time();
#ifdef _DEBUG
				nDebugCnt++;
		//		if( 3 <= nDebugCnt )
		//			return result;
#endif
			}

			if( result == Hdc::rOk )
				result = AddBarTemp( bNext, time, dBid, dAsk );
			assert( result == Hdc::rOk );
		}

		//	�Ō�̃f�[�^���o�͂���
		if( result == Hdc::rOk && timePrev != m_barTemp.Time() && timePrev < m_barTemp.Time() ){
			result = Write( cfwBid, m_barTemp, Hdc::Bid );
			if( result == Hdc::rOk )
				result = Write( cfwAsk, m_barTemp, Hdc::Ask );
		}
	}
	catch(...){
		result = Hdc::rFileOpenError;
	}

	return result;
}

//	���o�[��������
Hdc::Result hdcChart::IsNextBarTime(
	bool& bNext,		//(o)���o�[�t���O
	const hdcTime& time	//(i)����
	) const
{
	Hdc::Result result = Hdc::rOk;
	hdcTime timeNextOpen;

	bNext = false;

	if( GetNextOpenTime(timeNextOpen) ){
#ifdef _DEBUG
	CString strDebug1 = time.Format( HDC_FMT_YYYYMMDD_HHMMSS );
	CString strDebug2 = timeNextOpen.Format( HDC_FMT_YYYYMMDD_HHMMSS );
#endif
		//	���o�[�̃e�B�b�N����
		if( timeNextOpen <= time ){
			bNext = true;
		}
	}

	return result;
}

//	��Ɨp�o�[�}�b�v�Ƀf�[�^��ǉ�����
Hdc::Result hdcChart::AddBarTemp(
	bool bNextBarTime,		//(i)���o�[�����t���O
	const hdcTime& time,	//(i)����
	double dBid,			//(i)Bid
	double dAsk				//(i)Ask
	)
{
	Hdc::Result result = Hdc::rOk;
	mapBar::const_reverse_iterator it = m_mapBar.rbegin();
#ifdef _DEBUG
	CString strDebug = time.Format( HDC_FMT_YYYYMMDD_HHMMSS );
#endif
	if( it != m_mapBar.rend() ){
		//	���o�[�̃e�B�b�N����
		if( bNextBarTime ){
			hdcTime timeNextOpen = GetOpenTime( time, OutputPeriod() );
#ifdef _DEBUG
			CString strDebug2 = timeNextOpen.Format( HDC_FMT_YYYYMMDD_HHMMSS );
#endif
			m_mapBar.clear();
			m_barTemp.ClearRate();
			m_barTemp.SetTime( timeNextOpen );
			AddBar( m_barTemp );
		}
	}
	else{
		hdcTime timeNew = GetOpenTime( time, OutputPeriod() );

#ifdef _DEBUG
		CString strDebug = timeNew.Format( HDC_FMT_YYYYMMDD_HHMMSS );
#endif
		m_barTemp.SetTime( timeNew );
		AddBar( m_barTemp );
	}

	if( result == Hdc::rOk )
		m_barTemp.AddTick(dBid, dAsk);

	return result;
}

//	��Ɨp�o�[�Ƀf�[�^��ǉ�����
Hdc::Result hdcChart::AddBarTemp( 
	bool bNextBarTime,		//(i)���o�[�t���O
	const hdcTime& time,	//(i)����
	const double dRates[],	//(i)���[�g
	int volume				//(i)�o����
)
{
	Hdc::Result result = Hdc::rOk;
	mapBar::const_reverse_iterator it = m_mapBar.rbegin();
#ifdef _DEBUG
	CString strDebug = time.Format( HDC_FMT_YYYYMMDD_HHMMSS );
#endif
	if( it != m_mapBar.rend() ){
		//	���o�[�̃e�B�b�N����
		if( bNextBarTime ){
			hdcTime timeNextOpen = GetOpenTime( time, OutputPeriod() );
#ifdef _DEBUG
			CString strDebug2 = timeNextOpen.Format( HDC_FMT_YYYYMMDD_HHMMSS );
#endif
			m_mapBar.clear();
			m_barTemp.ClearRate();
			m_barTemp.SetTime( timeNextOpen );
			AddBar( m_barTemp );
		}
	}
	else{
		hdcTime timeNew = GetOpenTime( time, OutputPeriod() );

#ifdef _DEBUG
		CString strDebug = timeNew.Format( HDC_FMT_YYYYMMDD_HHMMSS );
#endif
		m_barTemp.SetTime( timeNew );
		AddBar( m_barTemp );
	}

	if( result == Hdc::rOk ){
		m_barTemp.AddRates(dRates);
		m_barTemp.AddVolume(volume);
	}

	return result;
}

//	�o�[�f�[�^�o��
Hdc::Result hdcChart::Write( 
	CStdioFile& cf,			//(o)�t�@�C���o��
	const hdcBar& bar,		//(i)�o�[�f�[�^
	Hdc::BarKind barKind	//(i)�o�[���
	)
{
	Hdc::Result result = Hdc::rOk;
	CString str;
	int nDecimal = Decimal();

	str.Format(_T("%s,%s,%.*lf,%.*lf,%.*lf,%.*lf,%d\n"), 
		bar.Time().Format(_T("%Y.%m.%d")),
		bar.Time().Format(_T("%H:%M:%S")),
		nDecimal,
		bar.Open( barKind ),
		nDecimal,
		bar.High( barKind ),
		nDecimal,
		bar.Low( barKind ),
		nDecimal,
		bar.Close( barKind ),
		bar.Volume());

	cf.WriteString( str );

	return result;
}
/*
//	�����f�[�^�𖄂߂�
Hdc::Result hdcChart::FillLackData( 
	hdcTime& timeNextOpen,	//(i/o)���o�[�I�[�v������
	const hdcTime& timeCur,	//(i)�J�����g�s����
	CStdioFile& cfBid,		//(i)Bid�o��
	CStdioFile& cfAsk		//(i)Ask�o��
	)
{
	Hdc::Result result = Hdc::rOk;
	hdcBar bar = m_barTemp;
	double dBid[hdcBar::NUM_OF_BAR_VALUE_KIND];
	double dAsk[hdcBar::NUM_OF_BAR_VALUE_KIND];
	bool bFlg = false;
	
	//	���߂̃o�[�̏I�l�ɃZ�b�g
	for( int n = 0; n < hdcBar::NUM_OF_BAR_VALUE_KIND; n++ ){
		dBid[n] = bar.Close(Hdc::Bid);
		dAsk[n] = bar.Close(Hdc::Ask);
	}
	bar.SetRate( dBid, Hdc::Bid );
	bar.SetRate( dAsk, Hdc::Ask );

	hdcTime time = timeNextOpen;
	hdcTimeSpan span = GetPeriodSpan( OutputPeriod() );

	while( time < timeCur && result == Hdc::rOk ){
		bar.SetTime( time );

		result = Write( cfBid, bar, Hdc::Bid );
		if( result == Hdc::rOk )
			result = Write( cfAsk, bar, Hdc::Ask );
		
		time += span;
		bFlg = true;
	}

	if( bFlg )
		timeNextOpen = GetOpenTime( timeCur, OutputPeriod() );
	
	return result;
}
*/

void hdcChart::SetSymbol( LPCTSTR value )
{
	m_strSymbol = value;

	CString strCur = m_strSymbol.Mid(3,3);

	strCur.MakeUpper();

	if( strCur == _T("JPY") )
		m_nDecimal = 3;
	else
		m_nDecimal = 5;
}

void hdcChart::SetShiftTime(
	int nTotalMinutes	//(i)�g�[�^����
	)
{
	int nHour = nTotalMinutes / 60;
	int nMin = nTotalMinutes - nHour*60;

	m_spanShift.SetDateTimeSpan( 0, nHour, nMin, 0 );
}

//	���Ԙg�̎��ԊԊu��Ԃ�
hdcTimeSpan hdcChart::GetPeriodSpan(
	int nPeriod	//(i)���Ԙg
	) const
{
	int nDay = nPeriod / 1440;
	int nHour = ( nPeriod - nDay*1440) / 60;
	int nMin = nPeriod - ( nDay*1440 + nHour*60 );
	hdcTimeSpan span;

	span.SetDateTimeSpan( nDay, nHour, nMin, 0 );

	return span;
}

//	�o�[�I�[�v��������Ԃ�
hdcTime hdcChart::GetOpenTime(
	const hdcTime& time,
	int nPeriod
	) const
{
	hdcTime timeOpen;

	if( 0 == nPeriod ){
		timeOpen = time;
	}
	else if( nPeriod < 60 ){
		int nMin = time.GetMinute();
		int nCoef = nMin / nPeriod;
		int nOpenMin = nCoef * nPeriod;

		timeOpen.SetDateTime( time.GetYear(), time.GetMonth(), time.GetDay(), time.GetHour(), nOpenMin, 0 );
	}
	else{
		int nHour = time.GetHour();
		int nPeriodHour = nPeriod / 60;
		int nCoef = nHour / nPeriodHour;
		int nOpenHour = nCoef * nPeriodHour;

		timeOpen.SetDateTime( time.GetYear(), time.GetMonth(), time.GetDay(), nOpenHour, 0, 0 );
	}

	return timeOpen;
}

//	���o�[�̃I�[�v��������Ԃ�
bool hdcChart::GetNextOpenTime(
	hdcTime& timeNextOpen	//(o)���o�[�I�[�v���^�C��
	) const
{
	bool bResult = true;
	mapBar::const_reverse_iterator it = m_mapBar.rbegin();

	if( it == m_mapBar.rend() ){
		bResult = false;
	}
	else{
		const hdcTime& timeRecent = it->first;
		
		timeNextOpen = timeRecent + GetPeriodSpan( OutputPeriod() );
	}

	return bResult;
}

//	���O�o�͊��ԏ�����
void hdcChart::InitOutputLogTerm()
{
	m_termOutputLog.clear();
#ifdef _DEBUG
	m_termOutputLog.push_back( hdcTerm( hdcTime(2011,6,29,23,49,0), hdcTime(2011,7,1,0,15,0) ) );
	m_termOutputLog.push_back( hdcTerm( hdcTime(2011,10,30,0,0,0), hdcTime(2011,10,31,0,30,0) ) );
#endif
}

//	���O�o�͔���
bool hdcChart::IsOutputLog( const hdcTime& time ) const
{
	for each( const auto& term in m_termOutputLog ){
		if( term.IsIn(time) )
			return true;
	}

	return false;
}

//	���o����
Hdc::Result hdcChart::Extract( 
	LPCTSTR lpszSrcPath, 
	LPCTSTR lpszOutPath, 
	const hdcTime& timeBegin, 
	const hdcTime& timeEnd, 
	Hdc::TimeFormat timeFormat 
	)
{
	Hdc::Result result = Hdc::rOk;

	try{
		CStdioFile cfr( lpszSrcPath, CFile::modeRead | CFile::typeText );
		CStdioFile cfw( lpszOutPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText );
		CString strData;
		vecString vecToken;
		const TCHAR szSep[] = _T(",\t");

		vecToken.reserve(6);

		while( cfr.ReadString( strData ) ){
			hdcTime time;

			vecToken.clear();
			hdcUtility::GetTokens( vecToken, strData, szSep );
			if( vecToken.size() < 2 )
				continue;

			switch( timeFormat ){
			case Hdc::DDMMYY_HHMMSS:
				result = hdcUtility::GetTimeDDMMYY_HHMMSS( time, vecToken[0] );
				break;
			case Hdc::YYYYMMDD_HHMMSS:
				result = hdcUtility::GetTimeYYYYMMDD_HHMMSS( time, vecToken[0], vecToken[1] );
				break;
			}

			if( result != Hdc::rOk ){
				hdcUtility::WriteLog( OutputLogFilePath(), _T("%s\n"), strData );
			}
			else{
				if( time != timeBegin && time < timeBegin )
					continue;
				else if( time != timeBegin && timeEnd < time )
					break;
				else{
					cfw.WriteString( strData );
					cfw.WriteString( _T("\n") );
				}
			}
		}
	}
	catch(...){
		result = Hdc::rFileOpenError;
	}

	return result;
}

//	�����f�[�^�𖄂߂�
Hdc::Result hdcChart::FillLackData( 
	LPCTSTR lpszSrcPath, 
	int nSrcPeriod, 
	LPCTSTR lpszOutPath, 
	bool bSkipFirstRow 
	)
{
	Hdc::Result result = Hdc::rOk;

	SetPeriod( nSrcPeriod );
	SetOutputPeriod( nSrcPeriod );

	if( nSrcPeriod == 0 )
		return Hdc::rCancel;

	try{
		CStdioFile cfr( lpszSrcPath, CFile::modeRead | CFile::typeText );
		CStdioFile cfw( lpszOutPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText );
		CString strData;
		BOOL bRead = TRUE;

		InitOutputLogTerm();
				
		if( bSkipFirstRow )
			bRead = cfr.ReadString( strData );

		hdcBar barA, barB;
		mapBar lackBars;

		for( bool isFirst = true; isFirst || bRead; isFirst = false ){
			bRead = cfr.ReadString( strData );
			
			if( bRead ){
				result = hdcBar::Generate( barA, strData, Hdc::Bid );
			
				if( Hdc::rOk == result && !isFirst ){
					GetLackData(lackBars, barA, barB );
				}

				for each( const auto& pair in lackBars ){
					const auto& bar = pair.second;
					
					result = Write( cfw, bar, Hdc::Bid );
					assert( Hdc::rOk == result );
				}

				if( Hdc::rOk == result ){
					result = Write( cfw, barA, Hdc::Bid );
				}

				barB = barA;
			}

			lackBars.clear();
		}
	}
	catch(...){
		result = Hdc::rFileOpenError;
	}

	return result;
}

//	�����f�[�^���o�͂���
void hdcChart::GetLackData( mapBar& bars, const hdcBar& barAfter, const hdcBar& barBefore )
{
	const auto& exMan = hdcExcludedPeriodManager::instance();

	if (exMan.isIn(barAfter.Time()) || exMan.isIn(barBefore.Time())) {
		hdcUtility::WriteLog(OutputLogFilePath()
			, _T("ExcludedPeriod %s-%s\n")
			, barBefore.Time().Format(_T("%D"))
			, barAfter.Time().Format(_T("%D")) );
		return;
	}

	Hdc::Result result = Hdc::rOk;
	const auto spanBeforeAfter = barAfter.Time() - barBefore.Time();
	const auto& timeBefore = barBefore.Time();
	const int day = Period()/HDC_PERIOD_D1;
	const int hour = Period()/HDC_PERIOD_H1;
	const int minutes = (Period() < HDC_PERIOD_H1) ? Period() : 0;
	const int second = 0;
	COleDateTimeSpan span( day, hour, minutes, second );
	const auto timeNext = barBefore.Time() + span;

	if (spanBeforeAfter.GetTotalMinutes() == Period())
		return;
	else if (Hdc::Saturday == timeNext.GetDayOfWeek())
		return;
	
#ifdef _DEBUG
		auto debug1 = barBefore.Time().Format(_T("%D/%H:%M"));
		auto debug2 = barAfter.Time().Format(_T("%D/%H:%M"));
#endif
	const int nLackBarCount = static_cast<int>(spanBeforeAfter.GetTotalMinutes() / Period()) - 1;
	const double dInit = static_cast<const double>(nLackBarCount + 1);
	const double dStep[] ={ ( barAfter.Open()- barBefore.Open() ) / dInit,
							( barAfter.High()- barBefore.High() ) / dInit,
							( barAfter.Low()- barBefore.Low() ) / dInit,
							( barAfter.Close()- barBefore.Close() ) / dInit };
	double dRates[] = { barBefore.Open(), barBefore.High(), barBefore.Low(), barBefore.Close() };
	hdcBar bar;

	//	�����o�[���R���e�i�ɒǉ�
	for( hdcTime time = barBefore.Time() + span; time < barAfter.Time(); time += span ){
		dRates[Hdc::Open] += dStep[Hdc::Open];
		dRates[Hdc::High] += dStep[Hdc::High];
		dRates[Hdc::Low] += dStep[Hdc::Low];
		dRates[Hdc::Close] += dStep[Hdc::Close];
		bar.SetTime(time);
		bar.SetRate(dRates);
		bar.SetVolume(barAfter.Volume());
		bars[time] = bar;
	}

	//	�}�[�P�b�g�����Ă���o�[�����O����
	auto it = bars.begin();
	auto end = bars.end();

	while( it != end ){
		const auto& time = it->first;

		if (Hdc::Saturday == time.GetDayOfWeek() || Hdc::Sunday == time.GetDayOfWeek()) {
			bars.erase(it++);
		}	
		else{
			it++;
		}
	}
}

//	�����f�[�^���o�͂���
Hdc::Result hdcChart::OutputLackData(LPCTSTR lpszSrcPath, int nSrcPeriod, LPCTSTR lpszOutPath, bool bSkipFirstRow)
{
	Hdc::Result result = Hdc::rOk;

	SetPeriod(nSrcPeriod);
	SetOutputPeriod(nSrcPeriod);

	if (nSrcPeriod == 0)
		return Hdc::rCancel;

	try {
		CStdioFile cfr(lpszSrcPath, CFile::modeRead | CFile::typeText);
		CStdioFile cfw(lpszOutPath, CFile::modeCreate | CFile::modeWrite | CFile::typeText);
		CString strData;
		BOOL bRead = TRUE;

		InitOutputLogTerm();

		if (bSkipFirstRow)
			bRead = cfr.ReadString(strData);

		hdcBar barA, barB;
		mapBar lackBars;

		for (bool isFirst = true; isFirst || bRead; isFirst = false) {
			bRead = cfr.ReadString(strData);

			if (bRead) {
				result = hdcBar::Generate(barA, strData, Hdc::Bid);

				//	�����f�[�^���o�͂���
				if (Hdc::rOk == result && !isFirst) {
					GetLackData(lackBars, barA, barB);
				}

				for each(const auto& pair in lackBars) {
					const auto& bar = pair.second;

					result = Write(cfw, bar, Hdc::Bid);
					assert(Hdc::rOk == result);
				}

				barB = barA;
			}

			lackBars.clear();
		}
	}
	catch (...) {
		result = Hdc::rFileOpenError;
	}

	return result;
}

//	���O�o�̓t�@�C���p�X��Ԃ�
CString hdcChart::OutputLogFilePath() const
{
	TCHAR szPathResult[MAX_PATH];
	TCHAR szPath[MAX_PATH];
	TCHAR szDrive[_MAX_DRIVE];
	TCHAR szDir[_MAX_DIR];
	TCHAR szFName[_MAX_FNAME];

	::GetModuleFileName(NULL, szPath, MAX_PATH);
	_tsplitpath(szPath, szDrive, szDir, NULL, NULL);
	_stprintf(szFName, _T("%s-%d"), Symbol(), Period());
	_tmakepath(szPathResult, szDrive, szDir, szFName, _T(".log"));

	return szPathResult;
}

