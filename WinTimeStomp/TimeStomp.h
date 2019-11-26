#pragma once
#include <windows.h>

class CTimeStomp
{
public:
	static BOOL GetFileInfo( const TCHAR *pFilePath );
	static BOOL SetFileInfo( const TCHAR *pFilePath );
	static BOOL Init( void );
	static BOOL CheckInit( void );
	static BOOL UnInit( void );
	static LARGE_INTEGER GetCreationTime( void );
	static LARGE_INTEGER GetLastAccessTime( void );
	static LARGE_INTEGER GetLastWriteTime( void );
	static LARGE_INTEGER GetChangeTime( void );
	static void SetCreationTime( LARGE_INTEGER );
	static void SetLastAccessTime( LARGE_INTEGER );
	static void SetLastWriteTime( LARGE_INTEGER );
	static void SetChangeTime( LARGE_INTEGER );
	static DWORD ConvertLocalTimeToLargeInteger( SYSTEMTIME localsystemtime, LARGE_INTEGER *largeinteger );

};

