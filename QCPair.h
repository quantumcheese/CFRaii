/*
 *  QCPair.h
 *  CFRaii
 *
 * Copyright (c) 2010 Richard A. Brown
 *
 * See license.rtf for licensing terms and conditions.
 */

#ifndef _QC_PAIR_GUARD_
#define _QC_PAIR_GUARD_

#include <CoreFoundation/CoreFoundation.h>
#include <algorithm>
#include "CFRaiiCommon.h"

template <class T, class U>
class QCPair
{
public:
	T first;
	U second;
	
public:
	
	// ctor
	QCPair(T const a, U const b)
	: first( QCRetain(a) )
	, second( QCRetain(b) )
	{ }
	
	// copy ctor
	QCPair(QCPair const &inPair)
	: first( QCRetain(inPair.first) )
	, second( QCRetain(inPair.second) )
	{ }
	
	// dtor
	~QCPair()
	{
		QCRelease(first);
		QCRelease(second);
	}
	
	QCPair const &retain() const
	{
		QCRetain(first);
		QCRetain(second);
		
		return *this;
	}
	
	void release() const
	{
		QCRelease(first);
		QCRelease(second);
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
		QCRelease(first);
		QCRelease(second);
		
		first = QCRetain(inPair.first);
		second = QCRetain(inPair.second);
		
		return *this;
	}
	
};

#endif
