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

namespace D
{
	struct QCDeleter
	{
		template < class T >
		void operator () (T * const t)
		{
			if (CFType_traits<T *>::is_CFType && t != NULL)
			{
				CFRelease(t);
			}
			else
			{
				// no-op
				;
			}
		}
	};
	
} /* anonymous namespace */

template < class T, bool = CFType_traits<T>::is_CFType >
class QCSharedPtr;

template < class T >
class QCSharedPtr < T*, true > //  : public std::tr1::shared_ptr< T >
{
	typedef std::tr1::shared_ptr< T >	shared_ptr;
	typedef D::QCDeleter				Deleter;
	
public:
	
	QCSharedPtr()
	// we want our custom deleter, so we would have to give _ptr 2 explicit arguments;
	// but constructing with NULL gives it a control block, i.e. it's not "empty"
	: _ptr( ) // default construction
	{ }
	
	template < class Other >
	explicit QCSharedPtr(Other * obj)
	: _ptr( obj, Deleter() )
	{ }
	
	template < class Other, class D >
	QCSharedPtr(Other * o, D dtor)
	: _ptr( o, dtor )
	{ }
	
	// default copy c'tor, d'tor
	
	// TODO: copy the list of public members of shared_ptr here
	
	void swap(QCSharedPtr &sp)
	{
		_ptr.swap(sp);
	}
    void reset()
	{
		_ptr.reset();
	}
	
	// default reset should use our Deleter
    template < class Other >
	void reset(Other * o)
	{
		_ptr.reset(o, Deleter());
	}
	
    template < class Other, class D >
	void reset(Other *o, D d)
	{
		_ptr.reset(o, d);
	}
	
	T *get() const
	{
		return _ptr.get();
	}
    T& operator*() const
	{
		return *_ptr;
	}
    T *operator->() const
	{
		// forward the operator explicitly
		return _ptr.operator->();
	}
    long use_count() const
	{
		return _ptr.use_count();
	}
    bool unique() const
	{
		return _ptr.unique();
	}
    operator bool() const
	{
		return _ptr;
	}
	
public:
	// CoreFoundation functions that apply to all CFTypes
	CFStringRef CopyDescription() const		{ return CFCopyDescription(get()); }
	Boolean Equal(T * const rhs) const		{ return CFEqual(get(), rhs); } // could use a CFTypeRef instead of a T*
	CFAllocatorRef GetAllocator() const		{ return CFGetAllocator(get()); }
	CFIndex GetRetainCount() const			{ return CFGetRetainCount(get()); }
	CFTypeID GetTypeID() const				{ return CFGetTypeID(get()); }
	CFHashCode Hash() const					{ return CFHash(get()); }
	void Show() const						{ CFShow(get()); }
	
	// Conceptually non-const memory-management methods.
	// Return get() to preserve type information that would otherwise become CFTypeRef.
	T MakeCollectible()						{ CFMakeCollectable(get()); return get(); }
	T Retain()								{ CFRetain(get()); return get(); }
	void Release()							{ CFRelease(get()); }
	
	
private:
	shared_ptr	_ptr;
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
			Release(obj);
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
			Release(obj);
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
	
	T get()
	{
		return obj;
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
