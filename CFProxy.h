/*
 *  CFProxy.h
 *  CFRaii
 *
 * Copyright (c) 2010 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */

/* C++ wrappers for generic type objects that need no special functionality.
 * Provides exception-safety for CF objects.
 */

#ifndef _CF_PROXY_GUARD_
#define _CF_PROXY_GUARD_

#include "CFRaiiCommon.h"

template<class T>
class CFProxy
{
private:
	// data members
	T obj;
	CFIndex *proxyCount;
	
public:
	// RAII ctor -- take ownership!
	CFProxy(T const ptr)
	: obj(ptr), proxyCount(new CFIndex(1))
	{ }
	
	// copy ctor
	CFProxy(CFProxy const &proxy)
	: obj(proxy.obj), proxyCount(proxy.proxyCount)
	{
		// could retain here and release in dtor regardless of proxyCount
		++(*proxyCount);
	}
	
	// dtor
	~CFProxy()
	{
		// last one out turn off the lights and close & lock the door
		if (--(*proxyCount) == 0)
		{
			QCRelease(obj);
			delete proxyCount;
		}
	}
	
	bool isNull() const
	{
		return obj == NULL;
	}
	
	void release()
	{
		if ( --(*proxyCount) == 0)
		{
			QCRelease(obj);
			obj = NULL;
		}
	}
	
	void retain()
	{
		++(*proxyCount);
	}
	
	
	bool null() const
	{
		return obj == NULL;
	}
	
	// copy assignment
	CFProxy & operator = (CFProxy const &rhs)
	{
		// increment the reference count
		rhs.retain();
		
		release();
		if (*proxyCount == 0)
		{
			delete proxyCount;
		}
		
		// TODO: switch to copy-and-swap ?
		
		// replace my count with rhs'
		proxyCount = rhs.proxyCount;
		
		// and finally, replace my obj with rhs'
		obj = rhs.obj;
	}
	
	// equality comparators
	bool operator == (CFProxy const &rhs) const
	{
		// pointer equality
		return obj == rhs.obj; // CFEqual(obj, rhs.obj);
	}
	
	bool operator != (CFProxy const &rhs) const
	{
		return ! (*this == rhs);
	}
	
	// dereference operator reveals the proxied object
	T operator * () const
	{
		return obj;
	}
	
	// conversion operator
	operator T () const
	{
		return obj;
	}
	
};

class CFTypeProxy
{
private:
	// data member
	CFTypeRef obj;
	
	// no copy assignment
	CFTypeProxy & operator = (CFTypeProxy const &);
public:
	// ctor
	CFTypeProxy(CFTypeRef const cfobj)
	: obj(QCRetain(cfobj))
	{ }
	
	// copy ctor
	CFTypeProxy(CFTypeProxy const &proxy)
	: obj(QCRetain(proxy.obj))
	{ }
	
	// dtor
	CFTypeProxy()
	{
		QCRelease(obj);
	}
	
	CFTypeRef get() const
	{
		return obj;
	}
	
	operator CFTypeRef () const
	{
		return obj;
	}
};

#endif
