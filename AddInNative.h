
#ifndef __ADDINNATIVE_H__
#define __ADDINNATIVE_H__

#include <wtypes.h>
#include "ComponentBase.h"
#include "AddInDefBase.h"
#include "IMemoryManager.h"

//---------------------------------------------------------------------------//
static uint32_t getLenShortWcharStr(const WCHAR_T* Source)
{
    uint32_t res = 0;
    WCHAR_T *tmpShort = (WCHAR_T*)Source;

    while (*tmpShort++)
        ++res;

    return res;
}
//---------------------------------------------------------------------------//
static uint32_t convToShortWchar(WCHAR_T** Dest, const wchar_t* Source, uint32_t len)
{
    if (!len)
        len = ::wcslen(Source)+1;

    if (!*Dest)
        *Dest = new WCHAR_T[len];

    WCHAR_T* tmpShort = *Dest;
    wchar_t* tmpWChar = (wchar_t*) Source;
    uint32_t res = 0;

    ::memset(*Dest, 0, len*sizeof(WCHAR_T));
    do
    {
        *tmpShort++ = (WCHAR_T)*tmpWChar++;
        ++res;
    }
    while (len-- && *tmpWChar);

    return res;
}
//---------------------------------------------------------------------------//
static uint32_t convFromShortWchar(wchar_t** Dest, const WCHAR_T* Source, uint32_t len)
{
    if (!len)
        len = getLenShortWcharStr(Source)+1;

    if (!*Dest)
        *Dest = new wchar_t[len];

    wchar_t* tmpWChar = *Dest;
    WCHAR_T* tmpShort = (WCHAR_T*)Source;
    uint32_t res = 0;

    ::memset(*Dest, 0, len*sizeof(wchar_t));
    do
    {
        *tmpWChar++ = (wchar_t)*tmpShort++;
        ++res;
    }
    while (len-- && *tmpShort);

    return res;
}
//---------------------------------------------------------------------------//

class CItem
{
public:

	IComponentBase *p_Object;
	wchar_t *p_ClassName;
	CItem *p_next;
	CItem *p_previous;

	CItem(IComponentBase *Object, const wchar_t *wsName)
	{
		p_Object	= Object;
		int wsNamelen	= ::wcslen(wsName) + 1;
		p_ClassName	= new wchar_t[wsNamelen];
		::wmemset(p_ClassName,0,wsNamelen);
		::wcscpy_s(p_ClassName, wsNamelen, wsName);
		p_next	= 0;
		p_previous	= 0;
	};

	~CItem()
	{
		if (p_Object)
		{
			delete p_Object;
			p_Object	= 0;
		}

		if (p_ClassName)
		{
			delete [] p_ClassName;
			p_ClassName	= 0;
		}

	};
};

class CListOfObjects
{

public:
	CListOfObjects()
	{
		root	= 0;
		last	= 0;
	};

	~CListOfObjects()
	{
		CItem	*ptr		= root;
		CItem	*nextptr	= 0;
		while (ptr)
		{
			nextptr	= ptr->p_next;
			delete ptr;
			ptr	= nextptr;
		}
		root	= 0;
		last	= 0;
	};

	void AddObject(IComponentBase *Object, const wchar_t* wsName)
	{

		CItem *newitem	= new CItem(Object, wsName);
		newitem->p_previous	= last;

		if (last)
			last->p_next	= newitem;
		if (!root)
			root	= newitem;
		last	= newitem;

	};

	int DestroyObject(IComponentBase *Object)
	{
		
		CItem * obj	= FindObject(Object);

		if (!obj)
			return -1;

		CItem * prevobj	= obj->p_previous;
		CItem * nextobj	= obj->p_next;

		if (obj == root)
			root	= nextobj;
		if (obj == last)
			last	= prevobj;
		if (prevobj)
			prevobj->p_next	= nextobj;
		if (nextobj)
			nextobj->p_previous	= prevobj;
		delete obj;
		return 0;
	};

	const wchar_t *GetClassName(IComponentBase *Object)
	{
		CItem *obj = FindObject(Object);

		if (!obj)
			return 0;
		return obj->p_ClassName;
	};

	IComponentBase * GetNextObjectByName(IComponentBase *Object, const wchar_t* wsName)
	{
		CItem * obj	= 0;

		if (Object)
		{
			obj	= FindObject(Object);
			if (!obj)
				return 0;
		}
		else
		{
			obj	= root;
			if (!obj)
				return 0;
			if (::wcscmp(obj->p_ClassName, wsName) == 0)
				return obj->p_Object;
		}
		obj = obj->p_next;
		while (obj)
		{
			if (::wcscmp(obj->p_ClassName, wsName) == 0)
				return obj->p_Object;
			obj	= obj->p_next;
		}

		return 0;

	};

private:

	CItem *root;
	CItem *last;

	CItem * FindObject(const IComponentBase *Object)
	{
		CItem * obj	= root;
		while (obj != 0)
		{
			if (obj->p_Object == Object)
			{
				return obj;
			}
			obj	= obj->p_next;
		}
		return 0;

	};

};

///////////////////////////////////////////////////////////////////////////////
// class CAddInNative
class CAddInNative : public IComponentBase
{
public:
    enum Props
    {
		EventOnKeyPressed	= 0,
		HookEnabled,
		FirstInterception,
		KeyboardLocked,
        eLastProp      // Always last
    };

    enum Methods
    {
        eLastMethod      // Always last
    };

	bool m_EventOnKeyPressed;
	bool m_HookEnabled;
	bool m_FirstInterception;
	bool m_KeyboardLocked;
	static HHOOK	m_KeyBoardHook;

    CAddInNative(void);
    virtual ~CAddInNative();
    // IInitDoneBase
    virtual bool ADDIN_API Init(void*);
    virtual bool ADDIN_API setMemManager(void* mem);
    virtual long ADDIN_API GetInfo();
    virtual void ADDIN_API Done();
    // ILanguageExtenderBase
    virtual bool ADDIN_API RegisterExtensionAs(WCHAR_T** wsLanguageExt);
    virtual long ADDIN_API GetNProps();
    virtual long ADDIN_API FindProp(const WCHAR_T* wsPropName);
    virtual const WCHAR_T* ADDIN_API GetPropName(long lPropNum, long lPropAlias);
    virtual bool ADDIN_API GetPropVal(const long lPropNum, tVariant* pvarPropVal);
    virtual bool ADDIN_API SetPropVal(const long lPropNum, tVariant* varPropVal);
    virtual bool ADDIN_API IsPropReadable(const long lPropNum);
    virtual bool ADDIN_API IsPropWritable(const long lPropNum);
    virtual long ADDIN_API GetNMethods();
    virtual long ADDIN_API FindMethod(const WCHAR_T* wsMethodName);
    virtual const WCHAR_T* ADDIN_API GetMethodName(const long lMethodNum, 
                            const long lMethodAlias);
    virtual long ADDIN_API GetNParams(const long lMethodNum);
    virtual bool ADDIN_API GetParamDefValue(const long lMethodNum, const long lParamNum,
                            tVariant *pvarParamDefValue);
    virtual bool ADDIN_API HasRetVal(const long lMethodNum);
    virtual bool ADDIN_API CallAsProc(const long lMethodNum,
                    tVariant* paParams, const long lSizeArray);
    virtual bool ADDIN_API CallAsFunc(const long lMethodNum,
                tVariant* pvarRetValue, tVariant* paParams, const long lSizeArray);
    operator IComponentBase*() { return (IComponentBase*)this; }
    // LocaleBase
    virtual void ADDIN_API SetLocale(const WCHAR_T* loc);

	bool SendEvent(wchar_t *Data);

private:
    // Attributes
    IAddInDefBase      *m_iConnect;
    IMemoryManager     *m_iMemory;

	static int		m_HookCount;

	long findName(const wchar_t* names[], const wchar_t* name, int Count) const;

};

	HHOOK	CAddInNative::m_KeyBoardHook	= 0;
	int		CAddInNative::m_HookCount	= 0;

class WcharWrapper
	{
	public:
#ifdef LINUX_OR_MACOS
		WcharWrapper(const WCHAR_T* str);
#endif
		WcharWrapper(const wchar_t* str);
		~WcharWrapper();
#ifdef LINUX_OR_MACOS
		operator const WCHAR_T*() { return m_str_WCHAR; }
		operator WCHAR_T*() { return m_str_WCHAR; }
#endif
		operator const wchar_t*() { return m_str_wchar; }
		operator wchar_t*() { return m_str_wchar; }
	private:
		WcharWrapper& operator = (const WcharWrapper& other) {};
		WcharWrapper(const WcharWrapper& other) {};
	private:
#ifdef LINUX_OR_MACOS
		WCHAR_T* m_str_WCHAR;
#endif
		wchar_t* m_str_wchar;
	};

#endif //__ADDINNATIVE_H__