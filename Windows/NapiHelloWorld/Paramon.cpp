#include <stdio.h>
#include <windows.h>
#include <windowsx.h>
#include <fstream>
#include "plugin.h"
#include "XTouch32Ex.h"
#include "Paramon.h"
#include <string>

//ofstream logfile;
HANDLE hDevice;
HINSTANCE hDll;
CPlugin * plugin;

HWND hWindow;
NPObject* window = NULL;
NPObject* paramon = NULL;
NPIdentifier ontouch = NULL;
NPIdentifier onready = NULL;
NPVariant consoleVar;
DWORD lastEvent = 0;
MONITORINFO primaryMonitor;
MONITORINFO secondMonitor;
int screenCount = 0;
bool touchEnabled = true;
string touchName;
bool initialized = false;
bool messagereceived = false;
string toString(LONG i)
{
	char number[21];
	_itoa_s(i, number, 10);
	return number;
}

extern "C" BOOL WINAPI EnumerateTouchscreenCallback(PVOID pContext,LPSTR szSymbolicName,UINT nType )
{
	try
	{
		//char text[] = "Found device:";
		//strcat(text,szSymbolicName);
		//paramonDebug(text);
		touchName=string(szSymbolicName);
		//logfile<<"registered";
		//logfile<<szSymbolicName;
		//logfile.close();
		//CloseDevice( hDevice );
	}
	catch(...)
	{
		paramonDebug(L"touchkit:exception on enum touch");
	}
    return TRUE;
}

BOOL CALLBACK MonitorEnumProc(
  HMONITOR hMonitor,    // handle to display monitor
  HDC hdcMonitor,       // handle to monitor DC
  LPRECT lprcMonitor,   // monitor intersection rectangle
  LPARAM dwData         // data
)
{
	//logfile<<screenCount<<":Bottom:"<<lprcMonitor->bottom<<";Top:"<<lprcMonitor->top<<";Left:"<<lprcMonitor->left<<";Right:"<<lprcMonitor->right<<"\n";
	try
	{
		MONITORINFO info;
		info.cbSize = sizeof(info);
		if(GetMonitorInfo(hMonitor,&info)>0)
		{
			if(info.dwFlags == MONITORINFOF_PRIMARY)
			{
				paramonDebug(L"touchkit:Found primary monitor");
				primaryMonitor = info;
				
			}
			else
			{
				paramonDebug(L"touchkit:Found secondary monitor");
				secondMonitor = info;
				
			}
	//[{name:'Display1',width:1024,height:768},{name:'Display2',width:1024,height:768,position:TOP|BOTTOM|LEFT|RIGHT}]
		}
		screenCount++;
	}
	catch(...)
	{
		paramonDebug(L"touchkit:ecxeption on enum monitors");
	}
    return true;
} 

void paramonInitialize(HWND hwnd,CPlugin * p)
{
	try
	{
		//if(initialized)return;
		plugin = p;
		hWindow = hwnd;
		//logfile.open("c:\\logs\\touch.log");
		//logfile<<"init\n";
		/*
		paramonDebug(L"touchkit:Loading labrary 'XTouch32EX.dll'...");
		hDll = LoadLibraryA((LPCSTR)"XTouch32EX.dll");
		if(hDll!=NULL)
		{
			//logfile<<"dll loaded\n";
			paramonDebug(L"touchkit:Library 'XTouch32EX.dll' loaded");
			paramonDebug(L"touchkit:Importing functions...");
			EnumerateTouchInstance = (ENUMERATETOUCHINSTANCE)GetProcAddress(hDll,"EnumerateTouchInstance");
			CreateDevice = (CREATEDEVICE)GetProcAddress(hDll,"CreateDevice");
			EnableTouch = (ENABLETOUCH)GetProcAddress(hDll,"EnableTouch");
			CloseDevice = (CLOSEDEVICE)GetProcAddress(hDll,"CloseDevice");
			StartDeviceThread = (STARTDEVICETHREAD)GetProcAddress(hDll,"StartDeviceThread");
			StopDeviceThread = (STOPDEVICETHREAD)GetProcAddress(hDll,"StopDeviceThread");
			SetBranchFormat = (SETBRANCHFORMAT)GetProcAddress(hDll,"SetBranchFormat");
			GetBranchFormat = (GETBRANCHFORMAT)GetProcAddress(hDll,"GetBranchFormat");
			RegisterTPNotifyMessage = (REGISTERTPNOTIFYMESSAGE)GetProcAddress(hDll,"RegisterTPNotifyMessage");
			paramonDebug(L"touchkit:Functions imported");
			paramonDebug(L"touchkit:Enumerating touch devices");
			paramonGetTouchInfo();
			paramonDebug(L"touchkit:Touch devices enumerated");
			*/
			//paramonDebug(L"touchkit:Enumerating monitors");
			paramonGetScreenInfo();
			//paramonDebug(L"touchkit:Monitors enumerated");
			//paramonDebug(L"touchkit:Initialization complete");
			initialized = true;
		/*} 
		else 
		{
			paramonDebug(L"touchkit:XTouch32EX.dll not found");
		}
		*/
		//logfile.close();
		//return dwNumsOfDevices;
	}
	catch (...)
	{
		paramonDebug(L"touchkit:exception on initialize");	
	}
}
void paramonLoadlibrary(WCHAR * path)
{
	wchar_t * buffer = new wchar_t[wcslen(path)+16];


	wcscpy(buffer,path);
	wcscat(buffer,(LPCWSTR)L"\\xTouch32ex.dll");
	paramonDebug(L"touchkit:Loading library 'xTouch32ex.dll'...");
	hDll = LoadLibrary(buffer);
	if(hDll!=NULL)
	{
		//logfile<<"dll loaded\n";
		paramonDebug(L"touchkit:Library 'xTouch32ex.dll' loaded");
		paramonDebug(L"touchkit:Importing functions...");
		EnumerateTouchInstance = (ENUMERATETOUCHINSTANCE)GetProcAddress(hDll,"EnumerateTouchInstance");
		CreateDevice = (CREATEDEVICE)GetProcAddress(hDll,"CreateDevice");
		EnableTouch = (ENABLETOUCH)GetProcAddress(hDll,"EnableTouch");
		CloseDevice = (CLOSEDEVICE)GetProcAddress(hDll,"CloseDevice");
		StartDeviceThread = (STARTDEVICETHREAD)GetProcAddress(hDll,"StartDeviceThread");
		StopDeviceThread = (STOPDEVICETHREAD)GetProcAddress(hDll,"StopDeviceThread");
		SetBranchFormat = (SETBRANCHFORMAT)GetProcAddress(hDll,"SetBranchFormat");
		GetBranchFormat = (GETBRANCHFORMAT)GetProcAddress(hDll,"GetBranchFormat");
		RegisterTPNotifyMessage = (REGISTERTPNOTIFYMESSAGE)GetProcAddress(hDll,"RegisterTPNotifyMessage");
		paramonDebug(L"touchkit:Functions imported");
		paramonDebug(L"touchkit:Enumerating touch devices");
		paramonGetTouchInfo();
		paramonDebug(L"touchkit:Touch devices enumerated");
	} 
	else 
	{
		paramonDebug(L"touchkit:XTouch32EX.dll not found");
	}
}

void paramonDispose()
{
	// Cleanup all allocated objects, otherwise, reference count and
	// memory leaks will happen.
	//NPN_ReleaseObject(window);
	//NPN_ReleaseVariantValue(&consoleVar);
	//logfile<<"dispose";
	if(!initialized) return;
	initialized = false;
	
	//logfile.close();
}

void paramonMoveWindowToScreen(wchar_t * title,wchar_t * display)
{
	// - Google Chrome
	//HWND hwnd = FindWindowEx(HWND_MESSAGE,NULL,NULL,(LPCWSTR)"TOUCHKIT - Mozilla Firefox");


	int len = wcslen(title) + 2;//+18;
	/*
	char * c = new char[10];
	WCHAR * d = new WCHAR[11];
	itoa(len,c,10);
	MultiByteToWideChar(CP_UTF8,0,c,11,(LPWSTR)d,11);
	paramonDebug(d);
	*/
	/*
	WCHAR * buffer = new WCHAR [len];
	for(int i = 0; i < len; i++)
		buffer[0] = 0;
	wcscpy(buffer,title);
	*/
	//wcscat(buffer,L" - Google Chrome");
	HWND hwnd = FindWindow(L"Chrome_WidgetWin_1",(LPCWSTR)title);
	//paramonDebug(L"touchkit:123");
	//HWND hwnd = GetDesktopWindow();
	//if(hwnd == NULL)
	//	paramonDebug(L"touchkit:Error:desctop window not found");
	//hwnd = FindWindowEx(hwnd,NULL,L"Chrome_WidgetWin_0",(LPCWSTR)title);
	//HWND hwnd = GetAncestor(hWindow,2);
	//if(hwnd == NULL)
	//	hwnd = FindWindow(L"Chrome_WidgetWin_1",(LPCWSTR)buffer);
	if(hwnd==NULL)
	{
		paramonDebug(L"touchkit:Error:window not found");
		paramonDebug(title);
	}
	else
	{
		paramonDebug(L"touchkit:window found");
	}
	if(display == L"Primary" || screenCount==1)
	{
		MoveWindow(hwnd, primaryMonitor.rcMonitor.left, primaryMonitor.rcMonitor.top,abs(primaryMonitor.rcMonitor.right - primaryMonitor.rcMonitor.left),abs(primaryMonitor.rcMonitor.top - primaryMonitor.rcMonitor.bottom),false); 
	}
	else 
	{
		ShowWindow(hwnd,SW_RESTORE);
		//MoveWindow(hwnd, secondMonitor.rcMonitor.left, secondMonitor.rcMonitor.top,secondMonitor.rcMonitor.right - secondMonitor.rcMonitor.left,secondMonitor.rcMonitor.bottom-secondMonitor.rcMonitor.top,true);
		SetWindowPos(hwnd,HWND_TOPMOST, secondMonitor.rcMonitor.left,secondMonitor.rcMonitor.top,secondMonitor.rcMonitor.right - secondMonitor.rcMonitor.left,secondMonitor.rcMonitor.bottom-secondMonitor.rcMonitor.top,SWP_NOACTIVATE);
		ShowWindow(hwnd,SW_SHOWMAXIMIZED);

		long style= GetWindowLong(hwnd, GWL_EXSTYLE);
		style |= WS_EX_TOOLWINDOW; 
		ShowWindow(hwnd, SW_HIDE); // hide the window
		SetWindowLong(hwnd, GWL_EXSTYLE, style); // set the style
		ShowWindow(hwnd, SW_SHOW);
	}
	SetForegroundWindow(hwnd);
	//paramonDebug(L"touchkit:post message");
	//SendMessage( hwnd, WM_KEYDOWN, VK_F11,0);
	//PostMessage(hwnd, WM_KEYUP, VK_F11, 3226927105);
	keybd_event( VK_F11,0x45,KEYEVENTF_EXTENDEDKEY | 0,0);
	keybd_event( VK_F11,0x45,KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP,0);
}
string paramonGetTouchInfo()
{
	/*logfile<<"start enumerate touches\n";
	if(hDevice > 0)
	{
		CloseDevice(hDevice);
		hDevice = 0;
	}
	*/
	DWORD count = 0;
	if(EnumerateTouchInstance!=NULL)
	count = EnumerateTouchInstance ( NULL, NULL, EnumerateTouchscreenCallback );
	//logfile<<"end enumerate touches\nstart enumerate displays\n";
	if(count > 0)
	{
		return touchName;
	}
	else
	{
		return "";	
	}
}
string paramonGetScreenInfo()
{
	screenCount = 0;
	if(EnumDisplayMonitors(NULL, NULL, MonitorEnumProc,0))
	{
		//throw std::runtime_error("CDisplayHandlesPool: EnumDisplayMonitors failed");
		//logfile<<"end enumerate displays\n";
		if(screenCount > 0)
		{
			string screenInfo("[");
			screenInfo+="{name:'Primary',width:" + toString(primaryMonitor.rcMonitor.right - primaryMonitor.rcMonitor.left) + ",height:" + toString(primaryMonitor.rcMonitor.bottom-primaryMonitor.rcMonitor.top)+"}";
			if(screenCount==2)
			{
				screenInfo+=",{name:'Secondary',width:" + toString(secondMonitor.rcMonitor.right - secondMonitor.rcMonitor.left) + ",height:" + toString(secondMonitor.rcMonitor.bottom-secondMonitor.rcMonitor.top)+",position:";
				if(secondMonitor.rcMonitor.left < primaryMonitor.rcMonitor.left)
					screenInfo+="'LEFT'";
				else if(secondMonitor.rcMonitor.right > primaryMonitor.rcMonitor.right)
					screenInfo+="'RIGHT'";
				else if(secondMonitor.rcMonitor.top < primaryMonitor.rcMonitor.top)
					screenInfo+="'TOP'";
				else if(secondMonitor.rcMonitor.bottom > primaryMonitor.rcMonitor.bottom)
					screenInfo+="'BOTTOM'";
				screenInfo+="}";
			}
			screenInfo+="]";
			return screenInfo;
		}
		else
		{
			return "";
		}
	}
}
void paramonEnableTouch(bool enable) 
{
	//logfile<<"enable touch:"<<enable<<"\n";
	if(touchEnabled==enable) return;
	touchEnabled = enable;
	
	if(enable)
	{
		EnableTouch(hDevice, enable);
		StopDeviceThread(hDevice);
		CloseDevice( hDevice );
	}
	else
	{
		paramonDebug(L"touchkit:Opening device...");
		hDevice = CreateDevice((LPSTR)touchName.c_str() );
		if(hDevice > 0)
		{
			EnableTouch(hDevice, enable);
			paramonDebug(L"touchkit:Device opened");
			if(StartDeviceThread(hDevice))
			{
				paramonDebug(L"touchkit:Device thread started");
				ULONG format = 0;
				GetBranchFormat(hDevice,&format);
				//logfile<<format<<"\n";
				format= 2147483687;
				//2147483687
				SetBranchFormat(hDevice,&format);
				//logfile<<hWindow<<"\n";
				RegisterTPNotifyMessage(hDevice,hWindow,0x8111);
			}
			else
			{
				paramonDebug(L"touchkit:Error:Device thread not started");
			}
		}
		else
		{
			paramonDebug(L"touchkit:Error:Device not opened.");
		}
	}




	
}

void paramonOnReady(string touch,string screen)
{
	try
	{
		/*
		NPVariant t,s;
		STRINGZ_TO_NPVARIANT(touch.c_str(), t);
		STRINGZ_TO_NPVARIANT(screen.c_str(),s);
		NPVariant args[] = { t,s };
		NPVariant voidResponse;
		if(NPN_Invoke(plugin->m_pNPInstance, paramon, onready, args,sizeof(args) / sizeof(args[0]),&voidResponse))
			logfile<<"onready success\n";
		NPN_ReleaseVariantValue(&voidResponse);
		*/

		// Get window object.
		char * ct;
		char * cs;

		ct = new char [touch.size()+1];
		strcpy (ct, touch.c_str());
		cs = new char [screen.size()+1];
		strcpy (cs, screen.c_str());

		NPObject* window = NULL;
		NPN_GetValue(plugin->m_pNPInstance, NPNVWindowNPObject, &window);

		// Get console object.
		NPVariant consoleVar;
		NPIdentifier id = NPN_GetStringIdentifier("Paramon");
		NPN_GetProperty(plugin->m_pNPInstance, window, id, &consoleVar);
		NPObject* console = NPVARIANT_TO_OBJECT(consoleVar);

		// Get the debug object.
		id = NPN_GetStringIdentifier("onReady");

		// Invoke the call with the message!
		NPVariant t,s;
		STRINGZ_TO_NPVARIANT(ct,t);
		STRINGZ_TO_NPVARIANT(cs,s);
		NPVariant args[] = { t,s };
		NPVariant voidResponse;
		NPN_Invoke(plugin->m_pNPInstance, console, id, args,sizeof(args) / sizeof(args[0]),&voidResponse);

		// Cleanup all allocated objects, otherwise, reference count and
		// memory leaks will happen.
		NPN_ReleaseObject(window);
		NPN_ReleaseVariantValue(&consoleVar);
		NPN_ReleaseVariantValue(&voidResponse);

		//logfile<<"onready:"<<touch<<screen<<"\n";
	}
	catch(...)
	{
		//logfile<<"exception onready\n";
	}
}

void paramonDebug(wchar_t * message)
{
		int len = wcslen(message)+1;
		char * cmessage = new char[len];
		WideCharToMultiByte(CP_UTF8,0,message,len,cmessage,len,NULL,NULL);
		NPObject* window = NULL;
		NPN_GetValue(plugin->m_pNPInstance, NPNVWindowNPObject, &window);
		
		// Get console object.
		NPVariant consoleVar;
		NPIdentifier id = NPN_GetStringIdentifier("console");
		NPN_GetProperty(plugin->m_pNPInstance, window, id, &consoleVar);
		NPObject* console = NPVARIANT_TO_OBJECT(consoleVar);

		// Get the debug object.
		id = NPN_GetStringIdentifier("debug");

		// Invoke the call with the message!
		NPVariant mess;
		STRINGZ_TO_NPVARIANT(cmessage,mess);
		NPVariant args[] = { mess };
		NPVariant voidResponse;
		NPN_Invoke(plugin->m_pNPInstance, console, id, args,sizeof(args) / sizeof(args[0]),&voidResponse);

		// Cleanup all allocated objects, otherwise, reference count and
		// memory leaks will happen.
		NPN_ReleaseObject(window);
		NPN_ReleaseVariantValue(&consoleVar);
		NPN_ReleaseVariantValue(&voidResponse);
}

void paramonOnTouch(WORD wx,WORD wy,DWORD type)
{
	// Invoke the call with the message!
	try
	{
		if(!messagereceived)
		{
			paramonDebug(L"touchkit:received first message from touch");
			messagereceived = true;
		}
		if(type==1 && lastEvent == 0)
		{
			/*
			logfile<<"ontouch"<<type<<","<<lastEvent<<"\n";
			NPVariant x,y;
			INT32_TO_NPVARIANT(wx, x);
			INT32_TO_NPVARIANT(wy, y);
			NPVariant args[] = { x,y };
			NPVariant voidResponse;
			if(NPN_Invoke(plugin->m_pNPInstance, paramon, ontouch, args,sizeof(args) / sizeof(args[0]), &voidResponse))
				logfile<<"ontouch success";
			NPN_ReleaseVariantValue(&voidResponse);
			*/
			
			// Get window object.
			NPObject* window = NULL;
			NPN_GetValue(plugin->m_pNPInstance, NPNVWindowNPObject, &window);

			// Get console object.
			NPVariant consoleVar;
			NPIdentifier id = NPN_GetStringIdentifier("TouchKit");
			NPN_GetProperty(plugin->m_pNPInstance, window, id, &consoleVar);
			NPObject* console = NPVARIANT_TO_OBJECT(consoleVar);

			// Get the debug object.
			id = NPN_GetStringIdentifier("onRawTouch");

			// Invoke the call with the message!
			NPVariant x,y;
			INT32_TO_NPVARIANT(wx, x);
			INT32_TO_NPVARIANT(wy, y);
			NPVariant args[] = { x,y };
			NPVariant voidResponse;
			NPN_Invoke(plugin->m_pNPInstance, console, id, args,
					 sizeof(args) / sizeof(args[0]),
					 &voidResponse);

			// Cleanup all allocated objects, otherwise, reference count and
			// memory leaks will happen.
			NPN_ReleaseObject(window);
			NPN_ReleaseVariantValue(&consoleVar);
			NPN_ReleaseVariantValue(&voidResponse);
		}
		lastEvent = type;
	}
	catch(...)
	{
		paramonDebug(L"touchkit:exception on touch");
	}
}