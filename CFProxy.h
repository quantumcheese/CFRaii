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
	// data member
	T obj;
	// no copy assignment (for now)
	CFProxy & operator = (CFProxy const &);
	
public:
	// ctor
	CFProxy(T const ptr)
	: obj(QCRetain(ptr))
	{ }
	
	// copy ctor
	CFProxy(CFProxy const &proxy)
	: obj(QCRetain(proxy.obj))
	{ }
	
	// dtor
	~ CFProxy()
	{
		QCRelease(obj);
	}
	
	// comparators
	
	bool operator == (CFProxy const &rhs) const
	{
		return CFEqual(obj, rhs.obj);
	}
	
	bool operator != (CFProxy const &rhs) const
	{
		return ! (*this == rhs);
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
