/*
 *  QCSharedPtr.h
 *  CFRaii
 *
 * Copyright (c) 2010-2011 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */

/* C++ wrappers for generic type objects that need no special functionality.
 * Provides exception-safety for CF objects.
 */

#ifndef _CF_PROXY_GUARD_
#define _CF_PROXY_GUARD_

#include <CoreFoundation/CoreFoundation.h>

#include <stdexcept>
#include <tr1/memory>
#include <tr1/type_traits>

#include "CFRaiiCommon.h"
#include "QCTypeTraits.h"

BEGIN_QC_NAMESPACE

namespace /* anonymous namespace */
{
	template < class T, bool = CFType_traits<T *>::is_CFType >
	struct QCDeleter
	{
		// shared_ptr requires operator () to exist for the false case of is_CFType.
		void operator () (T * const t)
		{
			;
		}
	};
	
	template <>
	struct QCDeleter < class T, true >
	{
		void operator () (T * const t)
		{
			if (t != NULL) { CFRelease(t); }
		}
	};
	
} /* anonymous namespace */

template < class T, bool = CFType_traits<T>::is_CFType >
class QCSharedPtr;

template < class T >
class QCSharedPtr< T*, true >  : public std::tr1::shared_ptr< T >
{
	typedef std::tr1::shared_ptr< T >	shared_ptr;
	typedef QCDeleter< T >				Deleter;
	
public:
	
	QCSharedPtr()
	// we want our custom deleter, so we have to give shared_ptr 2 explicit arguments
	// default construct with NULL
	: shared_ptr( static_cast<T *>(NULL), Deleter() )
	{ }
	
	explicit QCSharedPtr(T * const obj)
	: shared_ptr( obj, Deleter() )
	{ }
	
	// default copy c'tor, d'tor
	
	// TODO: copy the list of public members of shared_ptr here

	
private:
//	shared_ptr	_ptr;
};

template<class T>
class CFProxy
{
private:
	// data members
	T obj;
	CFIndex *proxyCount;
	
public:
	// RAII ctor -- take ownership!
	explicit CFProxy(T const ptr)
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
	
	inline bool isNull() const
	{
		return obj == NULL;
	}
	
	inline bool null() const
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
		// increment the reference count
		rhs.retain();
		
		release();
		if (*proxyCount == 0)
		{
			delete proxyCount;
		}
		
		// TODO: switch to copy-and-swap ?
		
		// replace my count with rhs'.  We already called retain(), so don't increment the proxyCount (again).
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
	
	// conversion operators
	operator T () const
	{
		return obj;
	}
	
	operator bool () const
	{
		return obj != NULL;
	}
	
};

END_QC_NAMESPACE

#endif
