#include "StdAfx.h"
#include "hdcUtility.h"
#include "hdcBar.h"


hdcBar::hdcBar(void)
{
	ClearRate();
	m_volume = 0;
}


hdcBar::~hdcBar(void)
{
}

Hdc::Result hdcBar::Generate( hdcBar& bar, const CString& strData, Hdc::BarKind barKind )
{
	Hdc::Result result = Hdc::rOk;
	vecString vecToken;
	hdcTime time;
	const TCHAR szSep[] = _T(",\t");

	vecToken.reserve(6);
	hdcUtility::GetTokens( vecToken, strData, szSep );

	if( vecToken.size() < 6 ){
		result = Hdc::rFail;
	}
	else{
		result = hdcUtility::GetTimeYYYYMMDD_HHMMSS( time, vecToken[0], vecToken[1] );
	}

	if( Hdc::rOk == result ){
		double dRates[NUM_OF_BAR_VALUE_KIND];
		int volume = (6 < vecToken.size()) ? _ttoi(vecToken[6]) : 1;
		
		for( int n = 0; n < NUM_OF_BAR_VALUE_KIND; n++ ){
			dRates[n] = _ttof( vecToken[n+2] );
		}

		bar.SetTime( time );
		bar.SetRate( dRates, barKind );
		bar.SetVolume(volume);
	}
	
	return result;
}

//	ティックを追加する
void hdcBar::AddTick( 
	double dBid,	//(i)Bid
	double dAsk		//(i)Ask
	)
{
	if( Open() == 0.0 ){
		m_dValue[Hdc::Open + Hdc::Bid] = dBid;
		m_dValue[Hdc::Open + Hdc::Ask] = dAsk;

		m_dValue[Hdc::High + Hdc::Bid] = dBid;
		m_dValue[Hdc::High + Hdc::Ask] = dAsk;

		m_dValue[Hdc::Low + Hdc::Bid] = dBid;
		m_dValue[Hdc::Low + Hdc::Ask] = dAsk;
	}
	else{
		m_dValue[Hdc::High + Hdc::Bid] = max(dBid, m_dValue[Hdc::High + Hdc::Bid]);
		m_dValue[Hdc::High + Hdc::Ask] = max(dAsk, m_dValue[Hdc::High + Hdc::Ask]);

		m_dValue[Hdc::Low + Hdc::Bid] = min(dBid, m_dValue[Hdc::Low + Hdc::Bid]);
		m_dValue[Hdc::Low + Hdc::Ask] = min(dAsk, m_dValue[Hdc::Low + Hdc::Ask]);
	}

	m_dValue[Hdc::Close + Hdc::Bid] = dBid;
	m_dValue[Hdc::Close + Hdc::Ask] = dAsk;
}

//	レートを追加する
void hdcBar::AddRates( const double dRates[], Hdc::BarKind barKind )
{
	if( Open(barKind) == 0.0 ){
		m_dValue[Hdc::Open + barKind] = dRates[Hdc::Open];
		m_dValue[Hdc::High + barKind] = dRates[Hdc::High];
		m_dValue[Hdc::Low + barKind] = dRates[Hdc::Low];
	}
	else{
		m_dValue[Hdc::High + barKind] = max(dRates[Hdc::High], m_dValue[Hdc::High + barKind]);
		m_dValue[Hdc::Low + barKind] = min(dRates[Hdc::Low], m_dValue[Hdc::Low + barKind]);
	}

	m_dValue[Hdc::Close + barKind] = dRates[Hdc::Close];
}

void hdcBar::AddVolume(int volume)
{
	m_volume += volume;
}