
#include "stdafx.h"
#include <wchar.h>
#include <string>
#include "AddInNative.h"

static const wchar_t *ClassNames[] = {L"KeyboardHook"};		//{L"Class1",L"Class2",L"Class3"}
static const wchar_t *StringOfClassNames = L"KeyboardHook";	//L"Class1|Class2|Class3";
#define NUMBEROFCLASSES 1

static const wchar_t *g_PropNamesEn[] = {L"EventOnKeyPressed",L"HookEnabled",L"FirstInterception",L"KeyboardLocked"};

static const wchar_t *g_PropNamesRu[] = {L"СобытиеПриНажатии",L"ЗахватРазрешен",L"ЗахватПервым",L"КлавиатураЗаблокирована"};

static AppCapabilities g_capabilities = eAppCapabilitiesInvalid;
static const wchar_t g_kClassNames[] = L"KeyboardHook"; //"|OtherClass1|OtherClass2";
static WcharWrapper s_names(g_kClassNames);

static CListOfObjects listOfObjects;

LRESULT CALLBACK KeyboardProc(int nCode,WPARAM wParam, LPARAM lParam);

WcharWrapper::WcharWrapper(const wchar_t* str) :
	m_str_wchar(NULL)
{
	if (str)
	{
		int len = wcslen(str);
		m_str_wchar = new wchar_t[len + 1];
		memset(m_str_wchar, 0, sizeof(wchar_t) * (len + 1));
		memcpy(m_str_wchar, str, sizeof(wchar_t) * len);
	}

}
//---------------------------------------------------------------------------//
WcharWrapper::~WcharWrapper()
{
	if (m_str_wchar)
	{
		delete[] m_str_wchar;
		m_str_wchar = NULL;
	}
}

//---------------------------------------------------------------------------//
long GetClassObject(const wchar_t* wsName, IComponentBase** pInterface)
{
	/*
    if(!*pInterface)
    {
		int count;
		for(count = 0; count < NUMBEROFCLASSES; count++)
		{
			if (wcscmp(ClassNames[count], wsName) == 0)
			{
				switch(count)
				{
				case 0:
					*pInterface= new CAddInNative();
					listOfObjects.AddObject(*pInterface, wsName);
					break;
				default:
					return 0;
				}
				break;
			}
		}

        return (long)*pInterface;
    }
	*/

	/*
	if (!*pInterface)
	{
		*pInterface = new CAddInNative();
		return (long)*pInterface;
	}
	
	return 0;
	*/

	if (!*pInterface)
	{
		int count;
		for (count = 0; count < NUMBEROFCLASSES; count++)
		{
			if (wcscmp(ClassNames[count], wsName) == 0)
			{
				switch (count)
				{
				case 0:
					*pInterface = new CAddInNative();
					listOfObjects.AddObject(*pInterface, wsName);
					break;
				default:
					return 0;
				}
				break;
			}
		}

		return (long)*pInterface;
	}

}
//---------------------------------------------------------------------------//
AppCapabilities SetPlatformCapabilities(const AppCapabilities capabilities)
{
	g_capabilities = capabilities;
	return eAppCapabilitiesLast;
}
//---------------------------------------------------------------------------//
long DestroyObject(IComponentBase** pIntf)
{
	/*
   if(!*pIntf)
      return -1;
   if (listOfObjects.DestroyObject(*pIntf) != 0)
	   return -1;
   *pIntf = 0;
   return 0;
   */

	/*
	if (!*pIntf)
		return -1;

	delete *pIntf;
	*pIntf = 0;
	return 0;
	*/

	if (!*pIntf)
		return -1;
	if (listOfObjects.DestroyObject(*pIntf) != 0)
		return -1;
	*pIntf = 0;
	return 0;

}
//---------------------------------------------------------------------------//
const WCHAR_T* GetClassNames()
{
	//return StringOfClassNames;
	return s_names;
}
//---------------------------------------------------------------------------//
//CAddInNative
CAddInNative::CAddInNative()
{
    m_iMemory			= 0;
    m_iConnect			= 0;
	m_EventOnKeyPressed	= true;
	m_HookEnabled		= false;
	//m_FirstInterception = true;
	m_FirstInterception = false;
	m_KeyboardLocked	= false;

	if (!m_KeyBoardHook)
	{
		DWORD ThreadId	= ::GetCurrentThreadId();
		m_KeyBoardHook	= ::SetWindowsHookExA(WH_KEYBOARD, (HOOKPROC)KeyboardProc, 0, ThreadId);
	}

	m_HookCount++;

}
//---------------------------------------------------------------------------//
CAddInNative::~CAddInNative()
{
	m_HookCount--;
	if (!m_HookCount && m_KeyBoardHook)
	{
		if (::UnhookWindowsHookEx(m_KeyBoardHook))
			m_KeyBoardHook	= 0;
	}
}
//---------------------------------------------------------------------------//
bool CAddInNative::Init(void* pConnection)
{ 
	m_iConnect	= (IAddInDefBase*) pConnection;
	return m_iConnect != 0;
}
//---------------------------------------------------------------------------//
bool CAddInNative::setMemManager(void* memManager)
{ 
	m_iMemory	= (IMemoryManager*) memManager;
	return m_iMemory != 0;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetInfo()
{ 
    return 2000; 
}
//---------------------------------------------------------------------------//
void CAddInNative::Done()
{
}
//---------------------------------------------------------------------------//
bool CAddInNative::RegisterExtensionAs(WCHAR_T** wsExtensionName)
{ 
	/*
	const wchar_t *wsClassName	= listOfObjects.GetClassName(this);

	if (!wsClassName)
		return false;

	int NameSize = ::wcslen(wsClassName) + 1;

    if (m_iMemory)
    {
        if(m_iMemory->AllocMemory((void**)wsExtensionName, NameSize * sizeof(WCHAR_T)))
		{
			::wcscpy_s( *wsExtensionName, NameSize, wsClassName);
			return true;
		}
    }

    return false; 
	*/

	/*
	const wchar_t *wsExtension = L"KeyboardHook";
	int iActualSize = ::wcslen(wsExtension) + 1;
	WCHAR_T* dest = 0;

	if (m_iMemory)
	{
		if (m_iMemory->AllocMemory((void**)wsExtensionName, iActualSize * sizeof(WCHAR_T)))
			::convToShortWchar(wsExtensionName, wsExtension, iActualSize);
		return true;
	}

	return false;
	*/

	const wchar_t *wsClassName = listOfObjects.GetClassName(this);

	if (!wsClassName)
		return false;

	//const wchar_t *wsExtension = L"KeyboardHook";
	//int iActualSize = ::wcslen(wsExtension) + 1;
	int NameSize = ::wcslen(wsClassName) + 1;

	if (m_iMemory)
	{
		if (m_iMemory->AllocMemory((void**)wsExtensionName, NameSize * sizeof(WCHAR_T)))
		{
			::wcscpy_s(*wsExtensionName, NameSize, wsClassName);
			return true;
		}
	}

	return false;

}
//---------------------------------------------------------------------------//
long CAddInNative::GetNProps()
{ 
    return eLastProp;
}
//---------------------------------------------------------------------------//
long CAddInNative::FindProp(const WCHAR_T* wsPropName)
{
    long plPropNum = -1;

	plPropNum = findName(g_PropNamesEn, wsPropName, eLastProp);

    if (plPropNum == -1)
        plPropNum = findName(g_PropNamesRu, wsPropName, eLastProp);

    return plPropNum;
}

//---------------------------------------------------------------------------//
const WCHAR_T* CAddInNative::GetPropName(long lPropNum, long lPropAlias)
{ 
    if (lPropNum >= eLastProp)
        return 0;

    const wchar_t *wsCurrentName = 0;
    WCHAR_T *wsPropName = 0;

	int iActualSize = 0;

    switch(lPropAlias)
    {
    case 0: // En
        wsCurrentName = g_PropNamesEn[lPropNum];
        break;
    case 1: // Ru
        wsCurrentName = g_PropNamesRu[lPropNum];
        break;
    default:
        return 0;
    }

    iActualSize = ::wcslen(wsCurrentName)+1;

    if (m_iMemory && wsCurrentName)
    {
        if (m_iMemory->AllocMemory((void**)&wsPropName, iActualSize * sizeof(WCHAR_T)))
			::wcscpy_s(wsPropName, iActualSize, wsCurrentName);
    }

    return wsPropName;
}
//---------------------------------------------------------------------------//
bool CAddInNative::GetPropVal(const long lPropNum, tVariant* pvarPropVal)
{ 
    switch(lPropNum)
    {
    case EventOnKeyPressed:
        TV_VT(pvarPropVal) = VTYPE_BOOL;
        TV_BOOL(pvarPropVal) = m_EventOnKeyPressed;
        break;
    case HookEnabled:
        TV_VT(pvarPropVal) = VTYPE_BOOL;
        TV_BOOL(pvarPropVal) = m_HookEnabled;
        break;
    case FirstInterception:
        TV_VT(pvarPropVal) = VTYPE_BOOL;
        TV_BOOL(pvarPropVal) = m_FirstInterception;
        break;
    case KeyboardLocked:
        TV_VT(pvarPropVal) = VTYPE_BOOL;
        TV_BOOL(pvarPropVal) = m_KeyboardLocked;
        break;
    default:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------//
bool CAddInNative::SetPropVal(const long lPropNum, tVariant* varPropVal)
{ 
    switch(lPropNum)
    { 
    case EventOnKeyPressed:
        if (TV_VT(varPropVal) != VTYPE_BOOL)
            return false;
        m_EventOnKeyPressed = TV_BOOL(varPropVal);
        break;
    case HookEnabled:
        if (TV_VT(varPropVal) != VTYPE_BOOL)
            return false;
        m_HookEnabled = TV_BOOL(varPropVal);
        break;
    case FirstInterception:
        if (TV_VT(varPropVal) != VTYPE_BOOL)
            return false;
        m_FirstInterception = TV_BOOL(varPropVal);
        break;
    case KeyboardLocked:
        if (TV_VT(varPropVal) != VTYPE_BOOL)
            return false;
        m_KeyboardLocked = TV_BOOL(varPropVal);
        break;
    default:
        return false;
    }

    return true;
}
//---------------------------------------------------------------------------//
bool CAddInNative::IsPropReadable(const long lPropNum)
{ 
    switch(lPropNum)
    { 
    case EventOnKeyPressed:
    case HookEnabled:
    case FirstInterception:
    case KeyboardLocked:
        return true;
    default:
        return false;
    }

    return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::IsPropWritable(const long lPropNum)
{
    switch(lPropNum)
    { 
    case EventOnKeyPressed:
    case HookEnabled:
    case FirstInterception:
    case KeyboardLocked:
        return true;
    default:
        return false;
    }

    return false;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetNMethods()
{ 
    return eLastMethod;
}
//---------------------------------------------------------------------------//
long CAddInNative::FindMethod(const WCHAR_T* wsMethodName)
{ 
    return -1;
}
//---------------------------------------------------------------------------//
const WCHAR_T* CAddInNative::GetMethodName(const long lMethodNum, 
                            const long lMethodAlias)
{ 
    return 0;
}
//---------------------------------------------------------------------------//
long CAddInNative::GetNParams(const long lMethodNum)
{ 
    return 0;
}
//---------------------------------------------------------------------------//
bool CAddInNative::GetParamDefValue(const long lMethodNum, const long lParamNum,
                          tVariant *pvarParamDefValue)
{ 
    return false;
} 
//---------------------------------------------------------------------------//
bool CAddInNative::HasRetVal(const long lMethodNum)
{ 
    return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::CallAsProc(const long lMethodNum,
                    tVariant* paParams, const long lSizeArray)
{ 
    return false;
}
//---------------------------------------------------------------------------//
bool CAddInNative::CallAsFunc(const long lMethodNum,
                tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray)
{ 
    return false; 
}
//---------------------------------------------------------------------------//
void CAddInNative::SetLocale(const WCHAR_T* loc)
{
    _wsetlocale(LC_ALL, loc);
}
//---------------------------------------------------------------------------//
bool CAddInNative::SendEvent(wchar_t *Data)
{
	return m_iConnect->ExternalEvent(L"KeyboardHook",L"KeyboardHook", Data);
}
//---------------------------------------------------------------------------//
long CAddInNative::findName(const wchar_t* names[], const wchar_t* name, int Count) const
{
    long ret = -1;

    for (int i = 0; i < Count; i++)
    {
        if (!wcscmp(names[i], name))
        {
            ret = i;
            break;
        }
    }
    return ret;
}
//---------------------------------------------------------------------------//

LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	wchar_t output[300];
	bool QueueEnabled = true;
    if (nCode == HC_ACTION || nCode == HC_NOREMOVE) 
	{
		IComponentBase * pObject = 0;
		pObject	= listOfObjects.GetNextObjectByName(pObject, L"KeyboardHook");
		while (pObject)
		{
			CAddInNative *pm = dynamic_cast< CAddInNative* >(pObject);
			if (pm->m_HookEnabled)
			{
				unsigned int TransitionState = (((DWORD)lParam >> 31) & 0x01);
				if (TransitionState == 0)
				{
					unsigned int RepeatCount = (DWORD) lParam & 0xFFFF;
					unsigned int ScanCode = ((DWORD) lParam >> 16) & 0xFF;
					unsigned int ExtendedKey = (((DWORD) lParam >> 24) & 0x01);
					unsigned int PreviousKeyState = (((DWORD) lParam >> 30) & 0x01);
					unsigned int VirtualKey	= wParam;
					BYTE keystatebuff[256];
					wchar_t SymbolString[10];
					::wmemset(SymbolString,0,10);
					if (::GetKeyboardState(keystatebuff))
					{
						unsigned int ExtScanCode = ScanCode;
						unsigned int flags= (((DWORD) lParam >> 29) & 0x0001) ^ 0x0001;
						int numsymbol = ::ToUnicode(VirtualKey, ExtScanCode, keystatebuff, SymbolString, 10, flags);
						if (numsymbol == 10)
							::wmemset(SymbolString,0,10);
					}
					ULONG	ReturnCode	= 0;
					if (::GetKeyState(VK_LSHIFT) & 0x80)
						ReturnCode = ReturnCode | 0x1;
					ReturnCode <<= 1;
					if (::GetKeyState(VK_RSHIFT) & 0x80)
						ReturnCode = ReturnCode | 0x1;
					ReturnCode <<= 1;
					if (::GetKeyState(VK_LCONTROL) & 0x80)
						ReturnCode = ReturnCode | 0x1;
					ReturnCode <<= 1;
					if (::GetKeyState(VK_RCONTROL) & 0x80)
						ReturnCode = ReturnCode | 0x1;
					ReturnCode <<= 1;
					if (::GetKeyState(VK_LMENU) & 0x80)
						ReturnCode = ReturnCode | 0x1;
					ReturnCode <<= 1;
					if (::GetKeyState(VK_RMENU) & 0x80)
						ReturnCode = ReturnCode | 0x1;
					ReturnCode <<= 1;
					ReturnCode = ReturnCode | ExtendedKey;
					ReturnCode <<= 8;
					ReturnCode = ReturnCode | (VirtualKey & 0x00FF);
					int StringLength = swprintf_s(output, 300, L"%05u%s", ReturnCode, SymbolString);
					pm->SendEvent(output);
				}
				QueueEnabled = QueueEnabled & !pm->m_KeyboardLocked;
			}
			pObject	= listOfObjects.GetNextObjectByName(pObject, L"KeyboardHook");
		}
	}
	if (nCode < 0 || QueueEnabled)
	{
		LRESULT RetVal = CallNextHookEx( CAddInNative::m_KeyBoardHook, nCode, wParam, lParam );
	    return  RetVal;
	}
	return -1;

}