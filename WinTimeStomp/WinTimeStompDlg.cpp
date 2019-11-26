// WinTimeStompDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinTimeStomp.h"
#include "WinTimeStompDlg.h"
#include "TimeStomp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// CWinTimeStompDlg dialog

CWinTimeStompDlg::CWinTimeStompDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWinTimeStompDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	// Initialize the CTimeStomp class. It has some
	//   static data that needs to be initialized.
	CTimeStomp::Init();
}

void CWinTimeStompDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATECREATED, m_DateCreated);
	DDX_Control(pDX, IDC_DATEACCESSED, m_DateAccessed);
	DDX_Control(pDX, IDC_DATECHANGED, m_DateChanged);
}

BEGIN_MESSAGE_MAP(CWinTimeStompDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BROWSE, &CWinTimeStompDlg::OnBnClickedBrowse)
	ON_BN_CLICKED(IDC_GETINFO, &CWinTimeStompDlg::OnBnClickedGetinfo)
	ON_BN_CLICKED(IDC_UPDATEINFO, &CWinTimeStompDlg::OnBnClickedUpdateinfo)
	ON_BN_CLICKED(IDC_RADIO1, &CWinTimeStompDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CWinTimeStompDlg::OnBnClickedRadio2)
END_MESSAGE_MAP()

// CWinTimeStompDlg message handlers

CString savedPath;

BOOL CWinTimeStompDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	m_DateCreated.SetFormat( L"MM-dd-yy hh:mm" );
	m_DateAccessed.SetFormat( L"MM-dd-yy hh:mm" );
	m_DateChanged.SetFormat( L"MM-dd-yy hh:mm" );

	CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO1);
	pButton->SetCheck(TRUE);

	savedPath = "c:\\";

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWinTimeStompDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWinTimeStompDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWinTimeStompDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CWinTimeStompDlg::OnClose()
{
	CTimeStomp::UnInit();

	CDialog::OnClose();
}

/////////////////////////////////////////////////////////////////////
//
// The following code was added to support selection of directories
//
static int CALLBACK BrowseCallbackProc(HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{

	if (uMsg == BFFM_INITIALIZED)
	{
//		std::string tmp = (const char *)lpData;
//		std::cout << "path: " << tmp << std::endl;
		SendMessage(hwnd, BFFM_SETSELECTION, TRUE, lpData);
	}

	return 0;
}

void BrowseFolder(CString& savedPath, CString& selected)
{
	TCHAR path[MAX_PATH];

	BROWSEINFO bi = { 0 };
	bi.lpszTitle = L"Browse for folder...";
	bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;
	bi.lpfn = BrowseCallbackProc;

	TCHAR sp[MAX_PATH];
	for (int i = 0; i < savedPath.GetLength(); i++)
	{
		sp[i] = (TCHAR)savedPath.GetAt(i);
		sp[i + 1] = 0;
	}
	bi.lParam = (LPARAM)sp;

	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

	if (pidl != 0)
	{
		//get the name of the folder and put it in path
		SHGetPathFromIDList(pidl, path);

		//free memory used
		IMalloc * imalloc = 0;
		if (SUCCEEDED(SHGetMalloc(&imalloc)))
		{
			imalloc->Free(pidl);
			imalloc->Release();
		}

		selected = path;
		savedPath = path;
	}

}
//
/////////////////////////////////////////////////////////////////////

void CWinTimeStompDlg::OnBnClickedBrowse()
{
	CButton *pRadio2 = (CButton *)GetDlgItem(IDC_RADIO2);
	if (pRadio2->GetCheck() == BST_CHECKED)
	{
		BrowseFolder(savedPath, savedPath);
		SetDlgItemText(IDC_FILENAME, savedPath);
		return;
	}

	// Allow user to select a file.
	CFileDialog cfg(TRUE, 0, 0, 6, L"All Files (*.*}|*.*||" );

	if( cfg.DoModal() == IDOK )
	{
		// Set the UI items to reflect the choice.
		SetDlgItemText( IDC_FILENAME, cfg.GetPathName() );
		OnBnClickedGetinfo();
	}

}

// This method simply sets the FILETIME members to the
//   correct values from the LARGE_INTEGER.
FILETIME CWinTimeStompDlg::ConvertTime( LARGE_INTEGER li )
{
	FILETIME ft;
	ft.dwHighDateTime = li.HighPart;
	ft.dwLowDateTime = li.LowPart;

	return( ft );
}

// This method converts the time and sets it in the CTimeStomp class
//   as static data.
LARGE_INTEGER CWinTimeStompDlg::ConvertTime( CDateTimeCtrl &dt )
{
	// Get the time from the control into a CTime object.
	CTime tm;
	dt.GetTime( tm );

	// Declare what we need for the conversions.
	LARGE_INTEGER li;
	SYSTEMTIME si;

	// Get the time as system time.
	tm.GetAsSystemTime( si );

	// Do the conversion.
	CTimeStomp::ConvertLocalTimeToLargeInteger( si, &li );

	// Return the LARGE_INTEGER.
	return( li );
}

// This method gets a file name from the UI,
BOOL CWinTimeStompDlg::CheckForFileName( int nResID, TCHAR *pBuffer, int nMaxSize )
{

	// Clear the buffer and get the file name from
	//   the UI.
	memset( pBuffer, 0, nMaxSize );
	GetDlgItemText( nResID, pBuffer, nMaxSize );

	// Use a CString to easily trim the file name left and right.
	CString strFileName = pBuffer;
	strFileName.TrimLeft();
	strFileName.TrimRight();

	// Make sure we have something in the file name.
	if( strFileName.GetLength() == 0 )
	{
		AfxMessageBox( L"You must have a file name or directory selected." );
		return( FALSE );
	}

	// Copy the trimmed CString into the buffer.
	wcscpy( pBuffer, strFileName );

	// Return success.
	return( TRUE );
}

// This gets the time information from a file into the static data
//   of the CTimeStomp class.
void CWinTimeStompDlg::OnBnClickedGetinfo()
{

	CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO1);
	int chk = pButton->GetCheck();
	if (chk == BST_UNCHECKED)
	{
		AfxMessageBox(L"You cannot get information for an entire directory. This only works for single files.");
	}

	// Get the file name. If that method fails then bail out.
	TCHAR szFileName[500];
	if( !CheckForFileName( IDC_FILENAME, szFileName, sizeof( szFileName ) ) )
	{
		return;
	}

	// Let CTimeStomp get the file information.
	CTimeStomp::GetFileInfo( szFileName );

	try
	{
		// Convert the Creation Time into a CTime object.
		CTime tm1(ConvertTime(CTimeStomp::GetCreationTime()));
		// Set the Date Created time in the UI.
		m_DateCreated.SetTime(&tm1);
	}
	catch (CException *ex)
	{
	}


	try
	{
		// Convert the Last Write Time into a CTime object.
		CTime tm2(ConvertTime(CTimeStomp::GetLastWriteTime()));
		// Set the Get Last Write time in the UI.
		m_DateAccessed.SetTime(&tm2);
	}
	catch (CException *ex)
	{
	}

	try
	{
		// Convert the Get Change Time into a CTime object.
		CTime tm3(ConvertTime(CTimeStomp::GetChangeTime()));
		// Set the Change time in the UI.
		m_DateChanged.SetTime(&tm3);
	}
	catch (CException *ex)
	{
	}

}

void addTrailingBackslash(CString& data)
{
	char c = data.GetAt(data.GetLength() - 1);
	if (c != '\\')
	{
		data += "\\";
	}
}

void doDir(CString dir, BOOL doRecurse)
{
	CString dirWithStar;
	dirWithStar = dir;
	addTrailingBackslash(dirWithStar);
	dirWithStar += "*.*";

	WIN32_FIND_DATA fileData;
	memset(&fileData, 0, sizeof(WIN32_FIND_DATA));
	HANDLE handle = FindFirstFile(dirWithStar, &fileData);

	while (handle != INVALID_HANDLE_VALUE)
	{

		if( ( fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY ) == 0 &&	// Skip directories
			_tcscmp(fileData.cFileName, _T(".") ) != 0 && // ignore "." and ".."
			_tcscmp(fileData.cFileName, _T("..")) != 0)
		{
			CTimeStomp::SetFileInfo(fileData.cFileName);
		}

		if (!FindNextFile(handle, &fileData))
		{
			break;
		}
	}
	FindClose(handle);

	if (!doRecurse)
	{
		return;
	}

	memset(&fileData, 0, sizeof(WIN32_FIND_DATA));
	handle = FindFirstFile(dirWithStar, &fileData);

	while (handle != INVALID_HANDLE_VALUE)
	{

		if ((fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0 &&	// Look at directories
			_tcscmp(fileData.cFileName, _T(".")) != 0 && // ignore "." and ".."
			_tcscmp(fileData.cFileName, _T("..")) != 0)
		{
			CString nextDir = dir;
			addTrailingBackslash(nextDir);
			nextDir += fileData.cFileName;
			doDir(nextDir, doRecurse);
		}

		if (!FindNextFile(handle, &fileData))
		{
			break;
		}
	}
	FindClose(handle);

}

void CWinTimeStompDlg::OnBnClickedUpdateinfo()
{

	// Get the file name. If that method fails then bail out.
	TCHAR szFileName[500];
	if( !CheckForFileName( IDC_FILENAME, szFileName, sizeof( szFileName ) ) )
	{
		return;
	}

	CTime tm;

	// Set the Date Created time in the CTimeStomp class.
	CTimeStomp::SetCreationTime( ConvertTime( m_DateCreated ) );
	// Set the Last Write time in the CTimeStomp class.
	CTimeStomp::SetLastWriteTime( ConvertTime( m_DateAccessed ) );
	// Set the Change Time time in the CTimeStomp class.
	CTimeStomp::SetChangeTime( ConvertTime( m_DateChanged ) );

	CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO1);

	// Now let CTimeStomp write the information.
	if (pButton->GetCheck() == BST_CHECKED)
	{
		CTimeStomp::SetFileInfo(szFileName);
	}
	else
	{
		pButton = (CButton *)GetDlgItem(IDC_DORECURSE);
		doDir(savedPath,pButton->GetCheck() == BST_CHECKED ? TRUE : FALSE );
	}

	// Alert user to success.
	AfxMessageBox( L"Done!" );
}

void CWinTimeStompDlg::OnBnClickedRadio1()
{

	CButton *pCheck = (CButton *)GetDlgItem(IDC_DORECURSE);
	pCheck->EnableWindow(FALSE);
	CButton *pRadio = (CButton *)GetDlgItem(IDC_RADIO2);
	pRadio->SetCheck(FALSE);
	CWnd *pWnd = GetDlgItem(IDC_FILEORFOLDER);
	pWnd->SetWindowText(L"File Path:");
	CButton *pButton = (CButton *)GetDlgItem(IDC_GETINFO);
	pButton->EnableWindow(TRUE);
	
}

void CWinTimeStompDlg::OnBnClickedRadio2()
{

	CButton *pCheck = (CButton *)GetDlgItem(IDC_DORECURSE);
	pCheck->EnableWindow(TRUE);
	CButton *pRadio = (CButton *)GetDlgItem(IDC_RADIO1);
	pRadio->SetCheck(FALSE);
	CWnd *pWnd = GetDlgItem(IDC_FILEORFOLDER);
	pWnd->SetWindowText(L"Directory:");
	CButton *pButton = (CButton *)GetDlgItem(IDC_GETINFO);
	pButton->EnableWindow(FALSE);

}
