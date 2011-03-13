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

#include <stdexcept>
#include <tr1/memory>

#include "CFRaiiCommon.h"
#include "QCTypeTraits.h"

BEGIN_QC_NAMESPACE

namespace /* anonymous namespace */
{
	struct QCCFDeleter /* release */
	{
		template <class T>
		void operator () (T const &t)
		{
			QCRelease(t);
		}
	};
} /* anonymous namespace */

template < class T, bool = QC::is_CFType<T> >
class QCSharedPtr : public std::tr1::shared_ptr<T>;

template <>
class QCSharedPtr<T, true> : public std::tr1::shared_ptr<T>
{
	
};

template <class T>
class QCCFProxy
{
private:
	
	CFIndex increment()
	{
		CFIndex inc(0);
		if (proxyCount == NULL)
		{
			if (empty()) { /* no-op */ }
			else { throw std::logic_error("incrementing a null counter but has object"); }
		}
		else
		{
			inc = ++ (*proxyCount);
		}
		return inc;
	}
	
	CFIndex decrement()
	{
		CFIndex dec(0);
		if (proxyCount == NULL)
		{
			if (empty()) { /* no-op */ }
			else { throw std::logic_error("decrementing a null counter but has object"); }
		}
		else
		{
			dec = -- (*proxyCount);
		}
		
		if (dec == 0)
		{
			delete proxyCount;
			proxyCount = NULL;
			QCRelease(obj);
			obj = NULL;
		}
		return dec;
	}
	
public:
	
	// RAII ctor
	explicit QCCFProxy(T const & ptr)
	: obj(ptr), proxyCount(new CFIndex(1))
	{ }
	
	// generic copy ctor
	//	template<class P, bool = CFType_traits<P>::is_CFType >
	QCCFProxy(QCCFProxy const &proxy)
	: obj(proxy.obj), proxyCount(proxy.proxyCount)
	{
		increment();
	}
	
	// dtor
	virtual ~QCCFProxy()
	{
		if (decrement() == 0)
		{
			QCRelease(obj);
		}
	}
	
	operator bool () { return obj != NULL; }
	/* explicit */ operator T () { return obj; }
	
	// smart pointer behavior
	bool empty() { return obj != NULL; }
	T get() { return obj; }
	void swap( QCCFProxy & other ) { std::swap(obj, other.obj); std::swap(proxyCount, other.proxyCount); };
	
	
	// assignment
	QCCFProxy &
	operator = (QCCFProxy const &other)
	{
		decrement();
		obj = other.obj;
		proxyCount = other.proxyCount;
		increment();
	}
	
private:
	T obj;
	CFIndex *proxyCount;
	
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
