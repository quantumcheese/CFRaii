/*
 *  QCPair.h
 *  CFRaii
 *
 * Copyright (c) 2010 Richard A. Brown
 *
 * See license.txt for licensing terms and conditions.
 */

#ifndef _QC_PAIR_GUARD_
#define _QC_PAIR_GUARD_

#include <CoreFoundation/CoreFoundation.h>
#include <algorithm>
#include "CFRaiiCommon.h"

BEGIN_QC_NAMESPACE

template <class T, class U>
class QCPair
{
public:
	T first;
	U second;
	
public:
	// ctor
	QCPair(T const a, U const b)
	: first( Retain(a) )
	, second( Retain(b) )
	{ }
	
	// copy ctor
	QCPair(QCPair const &inPair)
	: first( Retain(inPair.first) )
	, second( Retain(inPair.second) )
	{ }
	
	// dtor
	~QCPair()
	{
		Release(first);
		Release(second);
	}
	
	QCPair const &retain() const
	{
		Retain(first);
		Retain(second);
		
		return *this;
	}
	
	void release() const
	{
		Release(first);
		Release(second);
	}
	
	CFHashCode hash() const
	{
		return CFHash(first) + CFHash(second);
	}
	
	Boolean equal(QCPair<T, U> const &rhs) const
	{
		return CFEqual(first, rhs.first) && CFEqual(second, rhs.second);
	}
	
	CFStringRef copyDescription() const
	{
#if 0
		CFStringRef description1 = CFCopyDescription(first);
		CFStringRef description2 = CFCopyDescription(second);
#endif
		CFStringRef description = CFStringCreateWithFormat(kCFAllocatorDefault
														   , NULL
														   , CFSTR("<%@, %@>")
														   , first, second);
		return description;
	}
	
	// operators
	QCPair &operator = (QCPair const &inPair)
	{
		Release(first);
		Release(second);
		
		first = Retain(inPair.first);
		second = Retain(inPair.second);
		
		return *this;
	}
	
	bool operator == (QCPair const &rhs) const
	{
		return (first == rhs.first)
			&& (second == rhs.second);
	}
	
	bool operator != (QCPair const &rhs) const
	{
		return !(*this == rhs);
	}
	
};

END_QC_NAMESPACE

#endif
