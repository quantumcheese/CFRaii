/*
 *  CFProxy.h
 *  CFRaii
 *
 * Copyright (c) 2010 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
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
	
	// copy assignment
	CFProxy & operator = (CFProxy const &rhs)
	{
		release();
		if (*proxyCount == 0)
		{
			// increment the reference count
			rhs.retain();
			
			// replace my count with rhs'
			delete proxyCount;
			proxyCount = rhs.proxyCount;
			
			// and finally, replace my obj with rhs'
			obj = rhs.obj;
		}
	}
	
	// equality comparators
	bool operator == (CFProxy const &rhs) const
	{
		return CFEqual(obj, rhs.obj);
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

class CFObjectProxy
{
private:
	// data member
	CFTypeRef obj;
	
	// no copy assignment
	CFObjectProxy & operator = (CFObjectProxy const &);
public:
	// ctor
	CFObjectProxy(CFTypeRef const cfobj)
	: obj(QCRetain(cfobj))
	{ }
	
	// copy ctor
	CFObjectProxy(CFObjectProxy const &proxy)
	: obj(QCRetain(proxy.obj))
	{ }
	
	// dtor
	~CFObjectProxy()
	{
		QCRelease(obj);
	}
};

#endif
