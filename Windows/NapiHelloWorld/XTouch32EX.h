

/*
	__declspec(dllimport) typedef BOOL ( WINAPI *XTOUCH_ENUM_CALLBACK_PROC)(PVOID     pContext, LPSTR    szSymbloicName, UINT nType );


	__declspec(dllimport) DWORD WINAPI EnumerateTouchInstance (HWND hWnd,  PVOID pContext, XTOUCH_ENUM_CALLBACK_PROC pCallback );
	__declspec(dllimport) HANDLE WINAPI CreateDevice(LPSTR szSymbolicName );
	__declspec(dllimport) VOID WINAPI  EnableTouch( HANDLE hFile, BOOL bEnable );
	__declspec(dllimport) BOOL WINAPI CloseDevice( HANDLE hFile );

*/

typedef BOOL ( WINAPI* XTOUCH_ENUM_CALLBACK_PROC)(PVOID     pContext, LPSTR    szSymbloicName, UINT nType );
typedef DWORD (CALLBACK* ENUMERATETOUCHINSTANCE)(HWND hWnd,  PVOID pContext, XTOUCH_ENUM_CALLBACK_PROC pCallback );
typedef HANDLE (CALLBACK* CREATEDEVICE)(LPSTR szSymbolicName );
typedef VOID (CALLBACK* ENABLETOUCH)( HANDLE hFile, BOOL bEnable );
typedef BOOL (CALLBACK* CLOSEDEVICE)( HANDLE hFile );
typedef BOOL (WINAPI* STARTDEVICETHREAD)( HANDLE hFile);
typedef BOOL (WINAPI* STOPDEVICETHREAD)( HANDLE hFile);
typedef VOID (WINAPI* SETBRANCHFORMAT)( HANDLE hFile, ULONG* pInfo );
typedef VOID (WINAPI* GETBRANCHFORMAT)( HANDLE hFile, ULONG* pInfo );
typedef DWORD (WINAPI* REGISTERTPNOTIFYMESSAGE)( HANDLE hFile, HWND hWnd, UINT imsg );

static const unsigned int TOUCH_MESSAGE = 0x9001;
static ENUMERATETOUCHINSTANCE EnumerateTouchInstance;
static CREATEDEVICE CreateDevice;
static ENABLETOUCH EnableTouch;
static CLOSEDEVICE CloseDevice;
static STARTDEVICETHREAD StartDeviceThread;
static STOPDEVICETHREAD StopDeviceThread;
static SETBRANCHFORMAT SetBranchFormat;
static GETBRANCHFORMAT GetBranchFormat;
static REGISTERTPNOTIFYMESSAGE RegisterTPNotifyMessage;

