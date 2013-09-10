/* -*- Mode: C++; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: NPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Netscape Public License
 * Version 1.1 (the "License"); you may not use this file except in
 * compliance with the License. You may obtain a copy of the License at
 * http://www.mozilla.org/NPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is mozilla.org code.
 *
 * The Initial Developer of the Original Code is 
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or 
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the NPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the NPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */

//////////////////////////////////////////////////
//
// CPlugin class implementation
//
#ifdef XP_WIN
#include <stdio.h>
//#include <stdafx.h>
#include <windows.h>
#include <windowsx.h>
#include "Paramon.h"
#include "XTouch32Ex.h"


using namespace std;
//#pragma comment(lib,"c:\\logs\\Paramon\\NapiHelloWorld\\Debug\\XTouch32EX.lib")
//#include "XTouch32EX.h"
//#pragma comment(lib,"touch.lib")
//ofstream log;

#endif

#ifdef XP_MAC
#include <TextEdit.h>
#endif

#ifdef XP_UNIX
#include <string.h>
#endif

#include "plugin.h"
#include "npfunctions.h"
/*
static NPIdentifier sFoo_id;
static NPIdentifier sBar_id;
static NPIdentifier sDocument_id;
static NPIdentifier sBody_id;
static NPIdentifier sCreateElement_id;
static NPIdentifier sCreateTextNode_id;
static NPIdentifier sAppendChild_id;
static NPIdentifier sPluginType_id;
*/
static NPIdentifier getTouchInfoId;
static NPIdentifier getScreenInfoId;
static NPIdentifier enableTouchId;
static NPIdentifier moveWindowToScreenId;
static NPIdentifier setWindowOnTopId;
static NPIdentifier loadLibraryId;

static NPObject *sWindowObj;
static int Counter = 0;
// Helper class that can be used to map calls to the NPObject hooks
// into virtual methods on instances of classes that derive from this
// class.
class ScriptablePluginObjectBase : public NPObject
{
public:
  ScriptablePluginObjectBase(NPP npp)
    : mNpp(npp)
  {
  }

  virtual ~ScriptablePluginObjectBase()
  {
  }

  // Virtual NPObject hooks called through this base class. Override
  // as you see fit.
  virtual void Invalidate();
  virtual bool HasMethod(NPIdentifier name);
  virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
  virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
  virtual bool HasProperty(NPIdentifier name);
  virtual bool GetProperty(NPIdentifier name, NPVariant *result);
  virtual bool SetProperty(NPIdentifier name, const NPVariant *value);
  virtual bool RemoveProperty(NPIdentifier name);
  virtual bool Enumerate(NPIdentifier **identifier, uint32_t *count);
  virtual bool Construct(const NPVariant *args, uint32_t argCount,
                         NPVariant *result);

public:
  static void _Deallocate(NPObject *npobj);
  static void _Invalidate(NPObject *npobj);
  static bool _HasMethod(NPObject *npobj, NPIdentifier name);
  static bool _Invoke(NPObject *npobj, NPIdentifier name,
                      const NPVariant *args, uint32_t argCount,
                      NPVariant *result);
  static bool _InvokeDefault(NPObject *npobj, const NPVariant *args,
                             uint32_t argCount, NPVariant *result);
  static bool _HasProperty(NPObject * npobj, NPIdentifier name);
  static bool _GetProperty(NPObject *npobj, NPIdentifier name,
                           NPVariant *result);
  static bool _SetProperty(NPObject *npobj, NPIdentifier name,
                           const NPVariant *value);
  static bool _RemoveProperty(NPObject *npobj, NPIdentifier name);
  static bool _Enumerate(NPObject *npobj, NPIdentifier **identifier,
                         uint32_t *count);
  static bool _Construct(NPObject *npobj, const NPVariant *args,
                         uint32_t argCount, NPVariant *result);

protected:
  NPP mNpp;
};

#define DECLARE_NPOBJECT_CLASS_WITH_BASE(_class, ctor)                        \
static NPClass s##_class##_NPClass = {                                        \
  NP_CLASS_STRUCT_VERSION_CTOR,                                               \
  ctor,                                                                       \
  ScriptablePluginObjectBase::_Deallocate,                                    \
  ScriptablePluginObjectBase::_Invalidate,                                    \
  ScriptablePluginObjectBase::_HasMethod,                                     \
  ScriptablePluginObjectBase::_Invoke,                                        \
  ScriptablePluginObjectBase::_InvokeDefault,                                 \
  ScriptablePluginObjectBase::_HasProperty,                                   \
  ScriptablePluginObjectBase::_GetProperty,                                   \
  ScriptablePluginObjectBase::_SetProperty,                                   \
  ScriptablePluginObjectBase::_RemoveProperty,                                \
  ScriptablePluginObjectBase::_Enumerate,                                     \
  ScriptablePluginObjectBase::_Construct                                      \
}

#define GET_NPOBJECT_CLASS(_class) &s##_class##_NPClass

void
ScriptablePluginObjectBase::Invalidate()
{
}

bool
ScriptablePluginObjectBase::HasMethod(NPIdentifier name)
{
  return false;
}

bool
ScriptablePluginObjectBase::Invoke(NPIdentifier name, const NPVariant *args,
                                   uint32_t argCount, NPVariant *result)
{
  return false;
}

bool
ScriptablePluginObjectBase::InvokeDefault(const NPVariant *args,
                                          uint32_t argCount, NPVariant *result)
{
  return false;
}

bool
ScriptablePluginObjectBase::HasProperty(NPIdentifier name)
{
  return false;
}

bool
ScriptablePluginObjectBase::GetProperty(NPIdentifier name, NPVariant *result)
{
  return false;
}

bool ScriptablePluginObjectBase::SetProperty(NPIdentifier name,const NPVariant *value)
{
/*
	if (name == sBar_id) {
    printf ("bar set\n");

    return true;
  }
*/
  return false;
}

bool
ScriptablePluginObjectBase::RemoveProperty(NPIdentifier name)
{
  return false;
}

bool
ScriptablePluginObjectBase::Enumerate(NPIdentifier **identifier,
                                      uint32_t *count)
{
  return false;
}

bool
ScriptablePluginObjectBase::Construct(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  return false;
}

// static
void
ScriptablePluginObjectBase::_Deallocate(NPObject *npobj)
{
  // Call the virtual destructor.
  delete (ScriptablePluginObjectBase *)npobj;
}

// static
void
ScriptablePluginObjectBase::_Invalidate(NPObject *npobj)
{
  ((ScriptablePluginObjectBase *)npobj)->Invalidate();
}

// static
bool
ScriptablePluginObjectBase::_HasMethod(NPObject *npobj, NPIdentifier name)
{
  return ((ScriptablePluginObjectBase *)npobj)->HasMethod(name);
}

// static
bool
ScriptablePluginObjectBase::_Invoke(NPObject *npobj, NPIdentifier name,
                                    const NPVariant *args, uint32_t argCount,
                                    NPVariant *result)
{
  return ((ScriptablePluginObjectBase *)npobj)->Invoke(name, args, argCount,
                                                       result);
}

// static
bool
ScriptablePluginObjectBase::_InvokeDefault(NPObject *npobj,
                                           const NPVariant *args,
                                           uint32_t argCount,
                                           NPVariant *result)
{
  return ((ScriptablePluginObjectBase *)npobj)->InvokeDefault(args, argCount,
                                                              result);
}

// static
bool
ScriptablePluginObjectBase::_HasProperty(NPObject * npobj, NPIdentifier name)
{
  return ((ScriptablePluginObjectBase *)npobj)->HasProperty(name);
}

// static
bool
ScriptablePluginObjectBase::_GetProperty(NPObject *npobj, NPIdentifier name,
                                         NPVariant *result)
{
  return ((ScriptablePluginObjectBase *)npobj)->GetProperty(name, result);
}

// static
bool
ScriptablePluginObjectBase::_SetProperty(NPObject *npobj, NPIdentifier name,
                                         const NPVariant *value)
{
  return ((ScriptablePluginObjectBase *)npobj)->SetProperty(name, value);
}

// static
bool
ScriptablePluginObjectBase::_RemoveProperty(NPObject *npobj, NPIdentifier name)
{
  return ((ScriptablePluginObjectBase *)npobj)->RemoveProperty(name);
}

// static
bool
ScriptablePluginObjectBase::_Enumerate(NPObject *npobj,
                                       NPIdentifier **identifier,
                                       uint32_t *count)
{
  return ((ScriptablePluginObjectBase *)npobj)->Enumerate(identifier, count);
}

// static
bool
ScriptablePluginObjectBase::_Construct(NPObject *npobj, const NPVariant *args,
                                       uint32_t argCount, NPVariant *result)
{
  return ((ScriptablePluginObjectBase *)npobj)->Construct(args, argCount,
                                                          result);
}


class ConstructablePluginObject : public ScriptablePluginObjectBase
{
public:
  ConstructablePluginObject(NPP npp)
    : ScriptablePluginObjectBase(npp)
  {
  }

  virtual bool Construct(const NPVariant *args, uint32_t argCount,
                         NPVariant *result);
};

static NPObject * AllocateConstructablePluginObject(NPP npp, NPClass *aClass)
{
  return new ConstructablePluginObject(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ConstructablePluginObject,
                                 AllocateConstructablePluginObject);

bool ConstructablePluginObject::Construct(const NPVariant *args, uint32_t argCount,NPVariant *result)
{
  printf("Creating new ConstructablePluginObject!\n");

  NPObject *myobj =    NPN_CreateObject(mNpp, GET_NPOBJECT_CLASS(ConstructablePluginObject));
  if (!myobj)
    return false;

  OBJECT_TO_NPVARIANT(myobj, *result);

  return true;
}

class ScriptablePluginObject : public ScriptablePluginObjectBase
{
public:
  ScriptablePluginObject(NPP npp)
    : ScriptablePluginObjectBase(npp)
  {
  }

  virtual bool HasMethod(NPIdentifier name);
  virtual bool HasProperty(NPIdentifier name);
  virtual bool GetProperty(NPIdentifier name, NPVariant *result);
  virtual bool Invoke(NPIdentifier name, const NPVariant *args,
                      uint32_t argCount, NPVariant *result);
  virtual bool InvokeDefault(const NPVariant *args, uint32_t argCount,
                             NPVariant *result);
};

static NPObject *
AllocateScriptablePluginObject(NPP npp, NPClass *aClass)
{
  return new ScriptablePluginObject(npp);
}

DECLARE_NPOBJECT_CLASS_WITH_BASE(ScriptablePluginObject,
                                 AllocateScriptablePluginObject);


bool ScriptablePluginObject::HasMethod(NPIdentifier name)
{
	return name == getTouchInfoId || name == getScreenInfoId || name == enableTouchId || name == moveWindowToScreenId || name == setWindowOnTopId || name == loadLibraryId;
}


bool ScriptablePluginObject::HasProperty(NPIdentifier name)
{
  return false;
  //(name == sBar_id || name == sPluginType_id);
}

bool ScriptablePluginObject::GetProperty(NPIdentifier name, NPVariant *result)
{
/*
	VOID_TO_NPVARIANT(*result);

  if (name == sBar_id) {
    static int a = 17;

    INT32_TO_NPVARIANT(a, *result);

    a += 5;

    return true;
  }

  if (name == sPluginType_id) {
    NPObject *myobj =
      NPN_CreateObject(mNpp, GET_NPOBJECT_CLASS(ConstructablePluginObject));
    if (!myobj) {
      return false;
    }

    OBJECT_TO_NPVARIANT(myobj, *result);

    return true;
  }
*/
  return false;
}

bool ScriptablePluginObject::Invoke(NPIdentifier name, const NPVariant *args,uint32_t argCount, NPVariant *result)
{
/*
  if (name == sFoo_id) {
    printf ("foo called!\n");

    NPVariant docv;
    NPN_GetProperty(mNpp, sWindowObj, sDocument_id, &docv);

    NPObject *doc = NPVARIANT_TO_OBJECT(docv);

    NPVariant strv;
    STRINGZ_TO_NPVARIANT("div", strv);

    NPVariant divv;
    NPN_Invoke(mNpp, doc, sCreateElement_id, &strv, 1, &divv);

    STRINGZ_TO_NPVARIANT("I'm created by a plugin!", strv);

    NPVariant textv;
    NPN_Invoke(mNpp, doc, sCreateTextNode_id, &strv, 1, &textv);

    NPVariant v;
    NPN_Invoke(mNpp, NPVARIANT_TO_OBJECT(divv), sAppendChild_id, &textv, 1,
               &v);
    NPN_ReleaseVariantValue(&v);

    NPN_ReleaseVariantValue(&textv);

    NPVariant bodyv;
    NPN_GetProperty(mNpp, doc, sBody_id, &bodyv);

    NPN_Invoke(mNpp, NPVARIANT_TO_OBJECT(bodyv), sAppendChild_id, &divv, 1,
               &v);
    NPN_ReleaseVariantValue(&v);

    NPN_ReleaseVariantValue(&divv);
    NPN_ReleaseVariantValue(&bodyv);

    NPN_ReleaseVariantValue(&docv);

    const char* outString = "foo return val";
    char* npOutString = (char *)NPN_MemAlloc(strlen(outString) + 1);
    if (!npOutString)
      return false;
    strcpy(npOutString, outString);
    STRINGZ_TO_NPVARIANT(npOutString, *result);

    return true;
  }
  */


	/*
static NPIdentifier getTouchInfoId;
static NPIdentifier getScreenInfoId;
static NPIdentifier enableTouchId;
static NPIdentifier moveWindowToScreenId;
static NPIdentifier setWindowOnTopId;
*/
	char * outString;
	if(name == getTouchInfoId)
	{
		string info = paramonGetTouchInfo();
		outString = (char *)NPN_MemAlloc(info.size() + 1);
		strcpy (outString, info.c_str());
		STRINGZ_TO_NPVARIANT(outString, *result);
		return true;
	}
	else if(name == getScreenInfoId)
	{
		string info = paramonGetScreenInfo();
		outString = (char *)NPN_MemAlloc(info.size() + 1);
		strcpy (outString, info.c_str());
		STRINGZ_TO_NPVARIANT(outString, *result);
		return true;		
	}
	else if(name == enableTouchId)
	{
		if(argCount > 0)
		{
			paramonEnableTouch(NPVARIANT_TO_BOOLEAN(args[0]));
			return true;
		}
	}
	else if(name == moveWindowToScreenId)
	{
		//MessageBox(NULL, (LPCWSTR)"Hello",NULL,0);
		if(argCount==2)
		{
			//char * title = new char [NPVARIANT_TO_STRING(args[0]).UTF8Length+1];
			//MultiByteToWideChar(CP_UTF8,0,NPVARIANT_TO_STRING(args[0]).UTF8Characters,NPVARIANT_TO_STRING(args[0]).UTF8Length+1,(LPWSTR)title,NPVARIANT_TO_STRING(args[0]).UTF8Length+1);
			int len = NPVARIANT_TO_STRING(args[0]).UTF8Length;
			WCHAR * title = new WCHAR [len+1];
			title[len] = L'\0';
			MultiByteToWideChar(CP_UTF8,0,NPVARIANT_TO_STRING(args[0]).UTF8Characters,len,(LPWSTR)title,len);
			len = NPVARIANT_TO_STRING(args[1]).UTF8Length;
			WCHAR * display = new WCHAR [len+1];
			display[len]=L'\0';
			MultiByteToWideChar(CP_UTF8,0,NPVARIANT_TO_STRING(args[1]).UTF8Characters,len,(LPWSTR)display,len);
			paramonMoveWindowToScreen(title,display);
			return true;
		}
	}
	else if(name == loadLibraryId)
	{
		//MessageBox(NULL, (LPCWSTR)"Hello",NULL,0);
		if(argCount==1)
		{
			//char * title = new char [NPVARIANT_TO_STRING(args[0]).UTF8Length+1];
			//MultiByteToWideChar(CP_UTF8,0,NPVARIANT_TO_STRING(args[0]).UTF8Characters,NPVARIANT_TO_STRING(args[0]).UTF8Length+1,(LPWSTR)title,NPVARIANT_TO_STRING(args[0]).UTF8Length+1);
			int len = NPVARIANT_TO_STRING(args[0]).UTF8Length;
			WCHAR * path = new WCHAR [len+1];
			path[len] = L'\0';
			//char * test = new char[20];
			//itoa(NPVARIANT_TO_STRING(args[0]).UTF8Length,test,10);
			//paramonDebug(test);
			MultiByteToWideChar(CP_UTF8,0,NPVARIANT_TO_STRING(args[0]).UTF8Characters,len,(LPWSTR)path,len);
			//paramonDebug(path);
			paramonLoadlibrary(path);
			return true;
		}
	}
	else if(name == setWindowOnTopId)
	{
	
	
	}
	return false;
}

bool
ScriptablePluginObject::InvokeDefault(const NPVariant *args, uint32_t argCount,
                                      NPVariant *result)
{
  printf ("ScriptablePluginObject default method called!\n");

  const char* outString = "default method return val";
  char* npOutString = (char *)NPN_MemAlloc(strlen(outString) + 1);
  if (!npOutString)
    return false;
  strcpy(npOutString, outString);
  STRINGZ_TO_NPVARIANT(npOutString, *result);

  return true;
}


CPlugin::CPlugin(NPP pNPInstance) :
  m_pNPInstance(pNPInstance),
  m_pNPStream(NULL),
  m_bInitialized(false),
  m_pScriptableObject(NULL)
{
#ifdef XP_WIN
  m_hWnd = NULL;
#endif
	
	try
	{
		NPN_GetValue(m_pNPInstance, NPNVWindowNPObject, &sWindowObj);
		NPIdentifier n = NPN_GetStringIdentifier("foof");
/*
	  sFoo_id = NPN_GetStringIdentifier("foo");
	  sBar_id = NPN_GetStringIdentifier("bar");
	  sDocument_id = NPN_GetStringIdentifier("document");
	  sBody_id = NPN_GetStringIdentifier("body");
	  sCreateElement_id = NPN_GetStringIdentifier("createElement");
	  sCreateTextNode_id = NPN_GetStringIdentifier("createTextNode");
	  sAppendChild_id = NPN_GetStringIdentifier("appendChild");
	  sPluginType_id = NPN_GetStringIdentifier("PluginType");
*/
		getTouchInfoId= NPN_GetStringIdentifier("getTouchInfo");
		getScreenInfoId= NPN_GetStringIdentifier("getScreenInfo");
		enableTouchId= NPN_GetStringIdentifier("enableTouch");
		moveWindowToScreenId= NPN_GetStringIdentifier("moveWindowToScreen");
		setWindowOnTopId= NPN_GetStringIdentifier("setWindowOnTop");
		loadLibraryId = NPN_GetStringIdentifier("loadLibrary");
		strcpy(m_String, "BEE(TM) TouchKit 1.0.0.1");
		//log<<"ids";
		//log.flush();
/*
	  NPVariant v;
	  INT32_TO_NPVARIANT(46, v);

	  NPN_SetProperty(m_pNPInstance, sWindowObj, n, &v);

	  NPVariant rval;
	  NPN_GetProperty(m_pNPInstance, sWindowObj, n, &rval);

	  if (NPVARIANT_IS_INT32(rval)) {
		printf("rval = %d\n", NPVARIANT_TO_INT32(rval));
	  }

	  n = NPN_GetStringIdentifier("document");

	  if (!NPN_IdentifierIsString(n)) {
		NPString str;
		str.UTF8Characters = "alert('NPN_IdentifierIsString() test failed!');";
		str.UTF8Length = strlen(str.UTF8Characters);

		NPN_Evaluate(m_pNPInstance, sWindowObj, &str, NULL);
	  }

	  NPObject *doc;

	  NPN_GetProperty(m_pNPInstance, sWindowObj, n, &rval);

	  if (NPVARIANT_IS_OBJECT(rval) && (doc = NPVARIANT_TO_OBJECT(rval))) {
		n = NPN_GetStringIdentifier("title");

		NPN_GetProperty(m_pNPInstance, doc, n, &rval);

		if (NPVARIANT_IS_STRING(rval)) {
		  printf ("title = %s\n", NPVARIANT_TO_STRING(rval).UTF8Characters);

		  NPN_ReleaseVariantValue(&rval);
		}

		n = NPN_GetStringIdentifier("plugindoc");

		OBJECT_TO_NPVARIANT(doc, v);
		NPN_SetProperty(m_pNPInstance, sWindowObj, n, &v);

		NPString str;
		str.UTF8Characters = "document.getElementById('result').innerHTML += '<p>' + 'NPN_Evaluate() test, document = ' + this + '</p>';";
		str.UTF8Length = strlen(str.UTF8Characters);

		NPN_Evaluate(m_pNPInstance, doc, &str, NULL);

		NPN_ReleaseObject(doc);
	  }

	  NPVariant barval;
	  NPN_GetProperty(m_pNPInstance, sWindowObj, sBar_id, &barval);

	  NPVariant arg;
	  OBJECT_TO_NPVARIANT(sWindowObj, arg);

	  NPN_InvokeDefault(m_pNPInstance, NPVARIANT_TO_OBJECT(barval), &arg, 1,
						&rval);

	  if (NPVARIANT_IS_INT32(rval) && NPVARIANT_TO_INT32(rval) == 4) {
		printf ("Default function call SUCCEEDED!\n");
	  } else {
		printf ("Default function call FAILED!\n");
	  }

	  NPN_ReleaseVariantValue(&barval);
	  NPN_ReleaseVariantValue(&rval);


	#if 0
	  n = NPN_GetStringIdentifier("prompt");

	  NPVariant vars[3];
	  STRINGZ_TO_NPVARIANT("foo", vars[0]);
	  STRINGZ_TO_NPVARIANT("bar", vars[1]);
	  STRINGZ_TO_NPVARIANT("foof", vars[2]);

	  NPN_Invoke(sWindowObj, n, vars, 3, &rval);

	  if (NPVARIANT_IS_STRING(rval)) {
		printf ("prompt returned '%s'\n", NPVARIANT_TO_STRING(rval).UTF8Characters);
	  }

	  NPN_ReleaseVariantValue(&rval);
	#endif

	  NPObject *myobj =
		NPN_CreateObject(m_pNPInstance,
						 GET_NPOBJECT_CLASS(ScriptablePluginObject));

	  n = NPN_GetStringIdentifier("pluginobj");

	  OBJECT_TO_NPVARIANT(myobj, v);
	  NPN_SetProperty(m_pNPInstance, sWindowObj, n, &v);

	  NPN_GetProperty(m_pNPInstance, sWindowObj, n, &rval);

	  printf ("Object set/get test ");

	  if (NPVARIANT_IS_OBJECT(rval) && NPVARIANT_TO_OBJECT(rval) == myobj) {
		printf ("succeeded!\n");
	  } else {
		printf ("FAILED!\n");
	  }

	  NPN_ReleaseVariantValue(&rval);
	  NPN_ReleaseObject(myobj);

	  const char *ua = NPN_UserAgent(m_pNPInstance);
	  strcpy(m_String, ua);
*/
	}
	catch(...)
	{
		//log<<"exception on constructor";
		//log.flush();
	}
}

CPlugin::~CPlugin()
{
	
  if (sWindowObj)
    NPN_ReleaseObject(sWindowObj);
  if (m_pScriptableObject)
    NPN_ReleaseObject(m_pScriptableObject);

  sWindowObj = 0;
}

#ifdef XP_WIN
static LRESULT CALLBACK PluginWinProc(HWND, UINT, WPARAM, LPARAM);
static WNDPROC lpOldProc = NULL;
#endif






NPBool CPlugin::init(NPWindow* pNPWindow)
{
	//log<<"init\n";
	//log.flush();
	if(pNPWindow == NULL)
	return false;

	#ifdef XP_WIN
		m_hWnd = (HWND)pNPWindow->window;
		if(m_hWnd == NULL)
		return false;

		// subclass window so we can intercept window messages and
		// do our drawing to it
		lpOldProc = SubclassWindow(m_hWnd, (WNDPROC)PluginWinProc);
		// associate window with our CPlugin object so we can access 
		// it in the window procedure
		SetWindowLongPtr(m_hWnd, GWLP_USERDATA, (LONG_PTR)this);
	#endif

	m_Window = pNPWindow;
	paramonInitialize(m_hWnd,this);
	m_bInitialized = true;
	//log<<"init end\n";
	//log.flush();
	return true;
}

void CPlugin::shut()
{
#ifdef XP_WIN
  // subclass it back
  SubclassWindow(m_hWnd, lpOldProc);
  m_hWnd = NULL;
#endif
  paramonDispose();
  m_bInitialized = false;
  
}

NPBool CPlugin::isInitialized()
{
  return m_bInitialized;
}

int16_t CPlugin::handleEvent(void* event)
{
#ifdef XP_MAC
  NPEvent* ev = (NPEvent*)event;
  if (m_Window) {
    Rect box = { m_Window->y, m_Window->x,
                 m_Window->y + m_Window->height, m_Window->x + m_Window->width };
    if (ev->what == updateEvt) {
      ::TETextBox(m_String, strlen(m_String), &box, teJustCenter);
    }
  }
#endif
  return 0;
}

// this will force to draw a version string in the plugin window
void CPlugin::showVersion()
{
  const char *ua = NPN_UserAgent(m_pNPInstance);
  strcpy(m_String, ua);

#ifdef XP_WIN
  InvalidateRect(m_hWnd, NULL, true);
  UpdateWindow(m_hWnd);
#endif

  if (m_Window) {
    NPRect r =
      {
        (uint16_t)m_Window->y, (uint16_t)m_Window->x,
        (uint16_t)(m_Window->y + m_Window->height),
        (uint16_t)(m_Window->x + m_Window->width)
      };

    NPN_InvalidateRect(m_pNPInstance, &r);
  }
}

// this will clean the plugin window
void CPlugin::clear()
{
  strcpy(m_String, "");

#ifdef XP_WIN
  InvalidateRect(m_hWnd, NULL, true);
  UpdateWindow(m_hWnd);
#endif
}

void CPlugin::getVersion(char* *aVersion)
{
  const char *ua = NPN_UserAgent(m_pNPInstance);
  char*& version = *aVersion;
  version = (char*)NPN_MemAlloc(1 + strlen(ua));
  if (version)
    strcpy(version, ua);
}

NPObject * CPlugin::GetScriptableObject()
{
  if (!m_pScriptableObject) {
    m_pScriptableObject =
      NPN_CreateObject(m_pNPInstance,
                       GET_NPOBJECT_CLASS(ScriptablePluginObject));
  }

  if (m_pScriptableObject) {
    NPN_RetainObject(m_pScriptableObject);
  }

  return m_pScriptableObject;
}

#ifdef XP_WIN
static LRESULT CALLBACK PluginWinProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	/*
ofstream file;
Counter++;
file.open("c:\\logs\\touch22.log");  
file<<msg<<" "<<Counter;
*/
  switch (msg) {
    case WM_PAINT:
      {
        // draw a frame and display the string
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        RECT rc;
        GetClientRect(hWnd, &rc);
        FrameRect(hdc, &rc, GetStockBrush(BLACK_BRUSH));
        CPlugin * p = (CPlugin *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
        if(p) {
          if (p->m_String[0] == 0) {
            strcpy("foo", p->m_String);
          }

          DrawTextA(hdc, p->m_String, strlen(p->m_String), &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        }

        EndPaint(hWnd, &ps);
      }
      break;
	case  0x8111:
		{
			paramonOnTouch(LOWORD(lParam),HIWORD(lParam),wParam);
		}
		break;
	case WM_MOUSEMOVE:
		{

			/*
				char* message = "mousemove";
		  CPlugin * p = (CPlugin *)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		  // Get window object.
		  NPObject* window = NULL;
		  NPN_GetValue(p->m_pNPInstance, NPNVWindowNPObject, &window);

		  // Get console object.
		  NPVariant consoleVar;
		  NPIdentifier id = NPN_GetStringIdentifier("console");
		  NPN_GetProperty(p->m_pNPInstance, window, id, &consoleVar);
		  NPObject* console = NPVARIANT_TO_OBJECT(consoleVar);

		  // Get the debug object.
		  id = NPN_GetStringIdentifier("debug");

		  // Invoke the call with the message!
		  NPVariant type;
		  STRINGZ_TO_NPVARIANT(message, type);
		  NPVariant args[] = { type };
		  NPVariant voidResponse;
		  NPN_Invoke(p->m_pNPInstance, console, id, args,
					 sizeof(args) / sizeof(args[0]),
					 &voidResponse);

		  // Cleanup all allocated objects, otherwise, reference count and
		  // memory leaks will happen.
		  NPN_ReleaseObject(window);
		  NPN_ReleaseVariantValue(&consoleVar);
		  NPN_ReleaseVariantValue(&voidResponse);
		  //PostMessage(hWnd,TOUCH_MESSAGE,0,0);
		  */
		}
		break;
    default:
      break;
  }
//file.close();
  return DefWindowProc(hWnd, msg, wParam, lParam);
}
#endif
