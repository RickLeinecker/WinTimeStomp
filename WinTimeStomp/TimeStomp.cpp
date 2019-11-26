#include "StdAfx.h"
#include "TimeStomp.h"
#include "TimeStompTypes.h"

// Prototypes that we'll use for the the DLL functions.
unsigned long (__stdcall * g_NtQueryInformationFile)( HANDLE hFile, IO_STATUS_BLOCK *pIOStatus, void *pFileInformation, unsigned long ulLength, int nFileInformationClass );
unsigned long (__stdcall * g_NtSetInformationFile)( HANDLE hFile, IO_STATUS_BLOCK *pIOStatus, void *pFileInformation, unsigned long ulLength, int nFileInformationClass );

// The module handle for the DLL.
HMODULE g_hModule;
// The structure that contains the information we need.
FILE_BASIC_INFORMATION g_FBI;

// This method initializes the class. It
//   loads the DLL and obtains the function
//   addresses that we'll use later.
BOOL CTimeStomp::Init( void )
{

	// Check if the module is already loaded.
	if( g_hModule != NULL )
	{
		// Bail out if the module is already loaded (indicating success).
		return( TRUE );
	}

	// Load the DLL instance.
	g_hModule = ::LoadLibrary( L"ntdll.dll" );
	// If LoadLibrary() fails, then the handle will be NULL.
	if( g_hModule == NULL )
	{
		// Bail out indicating failure.
		return( FALSE );
	}

	// Get the function address for NtQueryInformationFile
	g_NtQueryInformationFile = (unsigned long (__stdcall *)(HANDLE,IO_STATUS_BLOCK *,void *,unsigned long,int))::GetProcAddress( g_hModule, "NtQueryInformationFile" );
	// If GetProcAddress fails then the pointer will be NULL.
	if( g_NtQueryInformationFile == NULL )
	{
		// Free the DLL instance.
		::FreeLibrary( g_hModule );
		// Set the handle to NULL so we know it's unloaded.
		g_hModule = NULL;
		// Bail out indicating failure.
		return( FALSE );
	}

	// Get the function address for NtQueryInformationFile
	g_NtSetInformationFile = (unsigned long (__stdcall *)(HANDLE,IO_STATUS_BLOCK *,void *,unsigned long,int))::GetProcAddress( g_hModule, "NtSetInformationFile" );
	// If GetProcAddress fails then the pointer will be NULL.
	if( g_NtQueryInformationFile == NULL )
	{
		// Free the DLL instance.
		::FreeLibrary( g_hModule );
		// Set the handle to NULL so we know it's unloaded.
		g_hModule = NULL;
		// Bail out indicating failure.
		return( FALSE );
	}

	// Return indicating success.
	return( TRUE );
}

// This method cleans up by unloading the DLL instance.
BOOL CTimeStomp::UnInit( void )
{

	// Check to see if the DLL handle is already NULL.
	if( g_hModule == NULL )
	{
		// Return indicating success.
		return( TRUE );
	}

	// Free the DLL module.
	::FreeLibrary( g_hModule );
	// Set the handle variable to NULL.
	g_hModule = NULL;

	// Return indicating success.
	return( TRUE );
}

// This method checks for DLL and procedure initialization.
BOOL CTimeStomp::CheckInit( void )
{

	// Do we need to initialize?
	if( g_NtSetInformationFile == NULL ||
		g_NtQueryInformationFile == NULL ||
		g_hModule == NULL )
	{
		// Do it.
		Init();
	}

	// Check to see if everything worked.
	if( g_NtQueryInformationFile == NULL ||
		g_NtSetInformationFile == NULL ||
		g_hModule == NULL )
	{
		// Return failure.
		return( FALSE );
	}

	// Return success.
	return( TRUE );
}

// This method gets information from a file.
BOOL CTimeStomp::GetFileInfo( const TCHAR *pFilePath )
{
	IO_STATUS_BLOCK iostatus;

	// Make sure the DLL instance is loaded and
	//   ready to go.
	if( !CheckInit() )
	{
		// Return failure.
		return( FALSE );
	}

	// Here we open the file.
	HANDLE hFile = CreateFile( pFilePath, FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL );
	// Check to make sure the file opened.
	if( hFile == INVALID_HANDLE_VALUE )
	{
		// Return failure.
		return( FALSE );
	}

	// Call NtQueryInformationFile()
	//   A non-zero value indicates failure.
	if( g_NtQueryInformationFile( hFile, &iostatus, &g_FBI, sizeof( FILE_BASIC_INFORMATION ), FileBasicInformation ) < 0 )
	{
		// Close the file handle.
		CloseHandle( hFile );
		// Return failure.
		return( FALSE );
	}

	// Close the file handle.
	CloseHandle( hFile );

	// Return success.
	return( TRUE );

}

// This method sets information into a file.
BOOL CTimeStomp::SetFileInfo( const TCHAR *pFilePath )
{
	IO_STATUS_BLOCK iostatus;

	// Make sure the DLL instance is loaded and
	//   ready to go.
	if( !CheckInit() )
	{
		// Return failure.
		return( FALSE );
	}

	// Here we open the file.
	HANDLE hFile = CreateFile( pFilePath, FILE_READ_ATTRIBUTES | FILE_WRITE_ATTRIBUTES, 0, NULL, OPEN_EXISTING, 0, NULL );
	// Check to make sure the file opened.
	if( hFile == INVALID_HANDLE_VALUE )
	{
		// Return failure.
		return( FALSE );
	}

	// Call NtSetInformationFile()
	//   A non-zero value indicates failure.
	if( g_NtSetInformationFile( hFile, &iostatus, &g_FBI, sizeof( FILE_BASIC_INFORMATION ), FileBasicInformation ) < 0 )
	{
		// Close the file handle.
		CloseHandle( hFile );
		// Return failure.
		return( FALSE );
	}

	// Close the file handle.
	CloseHandle( hFile );

	// Return success.
	return( TRUE );

}

// This is more like a property and simply returns
//   the creation time.
LARGE_INTEGER CTimeStomp::GetCreationTime( void )
{

	return( g_FBI.CreationTime );

}

// This is more like a property and simply returns
//   the last access time.
LARGE_INTEGER CTimeStomp::GetLastAccessTime( void )
{

	return( g_FBI.LastAccessTime );

}

// This is more like a property and simply returns
//   the last write time.
LARGE_INTEGER CTimeStomp::GetLastWriteTime( void )
{

	return( g_FBI.LastWriteTime );

}

// This is more like a property and simply returns
//   the change time.
LARGE_INTEGER CTimeStomp::GetChangeTime( void )
{

	return( g_FBI.ChangeTime );

}

// This is more like a property and simply sets
//   the creation time.
void CTimeStomp::SetCreationTime( LARGE_INTEGER li )
{

	g_FBI.CreationTime = li;

}

// This is more like a property and simply sets
//   the last access time.
void CTimeStomp::SetLastAccessTime( LARGE_INTEGER li )
{

	g_FBI.LastAccessTime = li;

}

// This is more like a property and simply sets
//   the last write time.
void CTimeStomp::SetLastWriteTime( LARGE_INTEGER li )
{

	g_FBI.LastWriteTime = li;

}

// This is more like a property and simply sets
//   the change time.
void CTimeStomp::SetChangeTime( LARGE_INTEGER li )
{

	g_FBI.ChangeTime = li;

}

// returns 0 on error, 1 on success. this function converts a SYSTEMTIME structure to a LARGE_INTEGER
DWORD CTimeStomp::ConvertLocalTimeToLargeInteger( SYSTEMTIME localsystemtime, LARGE_INTEGER *largeinteger )
{
	// the local time is stored in the system time structure argument which should be from the user
	// input. the user inputs the times in local time which is then converted to utc system time because
	// ntfs stores all timestamps in utc, which is then converted to a large integer
	
	// MSDN recommends converting SYSTEMTIME to FILETIME via SystemTimeToFileTime() and
	// then copying the values in FILETIME to a ULARGE_INTEGER structure.

	FILETIME filetime;
	FILETIME utcfiletime;
	DWORD result = 0;

	// convert the SYSTEMTIME structure to a FILETIME structure
    if (SystemTimeToFileTime(&localsystemtime, &filetime) == 0)
	{
		return 0;
	}

	// convert the local file time to UTC
	if (LocalFileTimeToFileTime(&filetime, &utcfiletime) == 0)
	{
		return 0;
	}

	/* copying lowpart from a DWORD to DWORD, and copying highpart from a DWORD to a LONG.
	potential data loss of upper values 2^16, but acceptable bc we wouldn't be able to set 
	this high even if we wanted to because NtSetInformationFile() takes a max of what's
	provided in LARGE_INTEGER */
	largeinteger->LowPart = utcfiletime.dwLowDateTime;
	largeinteger->HighPart = utcfiletime.dwHighDateTime;

	return 1;
}
