#pragma once
class hdcBar
{
public:
	enum{
		NUM_OF_BAR_VALUE_KIND = 4
	};

	hdcBar(void);
	virtual ~hdcBar(void);

	//	レートを返す
	double Rate( Hdc::RateKind rateKind, Hdc::BarKind barKind = Hdc::Bid ) const;
	double Open( Hdc::BarKind barKind = Hdc::Bid ) const;
	double High( Hdc::BarKind barKind = Hdc::Bid ) const;
	double Low( Hdc::BarKind barKind = Hdc::Bid ) const;
	double Close( Hdc::BarKind barKind = Hdc::Bid ) const;

	void SetTime( const hdcTime& value );
	const hdcTime& Time() const;

	//	レートをセットする
	void SetRate( const double dRate[], Hdc::BarKind barKind );
	void SetRate( const double dRate[] );

	//	レートを削除する
	void ClearRate();
	
	//	ティックを追加する
	void AddTick( double dBid, double dAsk );

	//	レートを追加する
	void AddRates( const double dRates[], Hdc::BarKind barKind = Hdc::Bid );

private:

	hdcTime m_time;

	double m_dValue[NUM_OF_BAR_VALUE_KIND*2];
};

//	レートを返す
inline double hdcBar::Rate( Hdc::RateKind rateKind, Hdc::BarKind barKind ) const
{
	return ( m_dValue[rateKind+barKind] );
}

inline double hdcBar::Open( Hdc::BarKind barKind ) const
{
	return Rate( Hdc::Open, barKind );
}

inline double hdcBar::High( Hdc::BarKind barKind ) const
{
	return Rate( Hdc::High, barKind );
}

inline double hdcBar::Low( Hdc::BarKind barKind ) const
{
	return Rate( Hdc::Low, barKind );
}

inline double hdcBar::Close( Hdc::BarKind barKind ) const
{
	return Rate( Hdc::Close, barKind );
}

inline void hdcBar::SetRate( const double dRate[] )
{
	for( int n = 0; n < NUM_OF_BAR_VALUE_KIND*2; n++ )
		m_dValue[n] = dRate[n];
}

inline void hdcBar::SetRate( const double dRate[], Hdc::BarKind barKind )
{
	for( int n = 0; n < NUM_OF_BAR_VALUE_KIND; n++ )
		m_dValue[barKind + n] = dRate[n];
}

inline void hdcBar::SetTime( const hdcTime& value )
{
	m_time = value;
}

inline const hdcTime& hdcBar::Time() const
{
	return m_time;
}

//	レートを削除する
inline void hdcBar::ClearRate()
{
	memset( m_dValue, 0, sizeof(m_dValue) );
}

