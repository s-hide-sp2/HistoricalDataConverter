
// HistoricalDataConverterDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "HistoricalDataConverter.h"
#include "HistoricalDataConverterDlg.h"
#include "afxdialogex.h"
#include "hdcCmdConvertPeriod.h"
#include "hdcCmdExtract.h"
#include "hdcCmdFillLackData.h"
#include "hdcCmdOutLackData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

// ����
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHistoricalDataConverterDlg �_�C�A���O




CHistoricalDataConverterDlg::CHistoricalDataConverterDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHistoricalDataConverterDlg::IDD, pParent)
	, m_strDataFolder(_T("C:\\data\\HistoricalData\\Debug"))
	, m_strOutputFolder(_T("C:\\data\\HistoricalData\\Debug\\Out"))
	, m_nPeriod(1)
	//, m_nShiftTime(420)
	, m_nShiftTime(0)
	, m_nOutputPeriod(1)
	, m_timeBegin(COleDateTime::GetCurrentTime())
	, m_timeEnd(COleDateTime::GetCurrentTime())
	, m_nTimeFormat(0)
	, m_bSkipFirstLow(TRUE)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHistoricalDataConverterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_DATA_FOLDER, m_strDataFolder);
	DDV_MaxChars(pDX, m_strDataFolder, 255);
	DDX_Text(pDX, IDC_EDT_OUTPUT_FOLDER, m_strOutputFolder);
	DDV_MaxChars(pDX, m_strOutputFolder, 255);
	DDX_Text(pDX, IDC_EDT_PERIOD, m_nPeriod);
	DDV_MinMaxInt(pDX, m_nPeriod, 0, 1440);
	DDX_Text(pDX, IDC_EDT_SHIFT_TIME, m_nShiftTime);
	DDV_MinMaxInt(pDX, m_nShiftTime, 0, 1440);
	DDX_Text(pDX, IDC_EDT_OUTPUT_PERIOD, m_nOutputPeriod);
	DDV_MinMaxInt(pDX, m_nOutputPeriod, 1, 1440);
	DDX_DateTimeCtrl(pDX, IDC_DTP_BEGIN, m_timeBegin);
	DDX_DateTimeCtrl(pDX, IDC_DTP_END, m_timeEnd);
	//  DDX_Text(pDX, IDC_CMB_TIME_FORMAT, m_nTimeFormat);
	//DDX_CBString(pDX, IDC_CMB_TIME_FORMAT, m_nTimeFormat);
	DDX_CBIndex(pDX, IDC_CMB_TIME_FORMAT, m_nTimeFormat);
	DDX_Check(pDX, IDC_CHK_SKIP_FIRST_LOW, m_bSkipFirstLow);
	DDX_Control(pDX, IDC_CMB_DATA_FORMAT, m_cmbDataFormat);
}

BEGIN_MESSAGE_MAP(CHistoricalDataConverterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_CNV_PERIOD, &CHistoricalDataConverterDlg::OnBnClickedBtnCnvPeriod)
	ON_BN_CLICKED(IDC_BTN_EXTRACT, &CHistoricalDataConverterDlg::OnBnClickedBtnExtract)
	ON_BN_CLICKED(IDC_BTN_FILL_LACK_DATA, &CHistoricalDataConverterDlg::OnBnClickedBtnFillLackData)
	ON_BN_CLICKED(IDC_BTN_OUT_LACK_DATA, &CHistoricalDataConverterDlg::OnBnClickedBtnOutLackData)
END_MESSAGE_MAP()


// CHistoricalDataConverterDlg ���b�Z�[�W �n���h���[

BOOL CHistoricalDataConverterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// "�o�[�W�������..." ���j���[���V�X�e�� ���j���[�ɒǉ����܂��B

	// IDM_ABOUTBOX �́A�V�X�e�� �R�}���h�͈͓̔��ɂȂ���΂Ȃ�܂���B
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���̃_�C�A���O�̃A�C�R����ݒ肵�܂��B�A�v���P�[�V�����̃��C�� �E�B���h�E���_�C�A���O�łȂ��ꍇ�A
	//  Framework �́A���̐ݒ�������I�ɍs���܂��B
	SetIcon(m_hIcon, TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(m_hIcon, FALSE);		// �������A�C�R���̐ݒ�

	// TODO: �������������ɒǉ����܂��B
	m_cmbDataFormat.SetCurSel(0);

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

void CHistoricalDataConverterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �_�C�A���O�ɍŏ����{�^����ǉ�����ꍇ�A�A�C�R����`�悷�邽�߂�
//  ���̃R�[�h���K�v�ł��B�h�L�������g/�r���[ ���f�����g�� MFC �A�v���P�[�V�����̏ꍇ�A
//  ����́AFramework �ɂ���Ď����I�ɐݒ肳��܂��B

void CHistoricalDataConverterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �`��̃f�o�C�X �R���e�L�X�g

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N���C�A���g�̎l�p�`�̈���̒���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �A�C�R���̕`��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ���[�U�[���ŏ��������E�B���h�E���h���b�O���Ă���Ƃ��ɕ\������J�[�\�����擾���邽�߂ɁA
//  �V�X�e�������̊֐����Ăяo���܂��B
HCURSOR CHistoricalDataConverterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CHistoricalDataConverterDlg::OnBnClickedBtnCnvPeriod()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	if( UpdateData(TRUE) ){
		hdcCmdConvertPeriod cmd;
		Hdc::DataForamt dataFormat = static_cast<Hdc::DataForamt>(m_cmbDataFormat.GetCurSel());

		cmd.SetDataFolder( m_strDataFolder );
		cmd.SetOutputFolder( m_strOutputFolder );
		cmd.SetPeriod( m_nPeriod );
		cmd.SetOutputPeriod( m_nOutputPeriod );
		cmd.SetShiftTime( m_nShiftTime );
		cmd.SetSkipFirstRow(m_bSkipFirstLow);
		cmd.SetDataFormat(dataFormat);

		hdcCommand::Execute( cmd );
	}
}



void CHistoricalDataConverterDlg::OnBnClickedBtnExtract()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B

	if( UpdateData(TRUE) ){
		hdcCmdExtract cmd;

		m_timeBegin.SetDate( m_timeBegin.GetYear(), m_timeBegin.GetMonth(), m_timeBegin.GetDay() );
		m_timeEnd.SetDateTime( m_timeEnd.GetYear(), m_timeEnd.GetMonth(), m_timeEnd.GetDay(), 23, 59, 59 );

		cmd.SetDataFolder( m_strDataFolder );
		cmd.SetOutputFolder( m_strOutputFolder );
		cmd.SetTimeBegin( m_timeBegin );
		cmd.SetTimeEnd( m_timeEnd );
		cmd.SetTimeFormat( static_cast<Hdc::TimeFormat>(m_nTimeFormat) );

		hdcCommand::Execute( cmd );
	}
	
}


void CHistoricalDataConverterDlg::OnBnClickedBtnFillLackData()
{
	if( UpdateData(TRUE) ){
		hdcCmdFillLackData cmd;

		cmd.SetDataFolder( m_strDataFolder );
		cmd.SetOutputFolder( m_strOutputFolder );
		cmd.SetPeriod( m_nPeriod );
		cmd.SetSkipFirstRow( m_bSkipFirstLow );

		hdcCommand::Execute( cmd );
	}

}


void CHistoricalDataConverterDlg::OnBnClickedBtnOutLackData()
{
	// TODO: �����ɃR���g���[���ʒm�n���h���[ �R�[�h��ǉ����܂��B
	if (UpdateData(TRUE)) {
		hdcCmdOutLackData cmd;

		cmd.SetDataFolder(m_strDataFolder);
		cmd.SetOutputFolder(m_strOutputFolder);
		cmd.SetPeriod(m_nPeriod);
		cmd.SetSkipFirstRow(m_bSkipFirstLow);

		hdcCommand::Execute(cmd);
	}
}
