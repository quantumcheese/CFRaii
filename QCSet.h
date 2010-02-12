/*
 *  QCSet.h
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */

#ifndef _QC_SET_GUARD_
#define _QC_SET_GUARD_

#include <CoreFoundation/CoreFoundation.h>
#include <algorithm>
#include "CFRaiiCommon.h"

class QCSet
{
private:
	CFMutableSetRef set;
	
	CFMutableSetRef CFMutableSetFromCFSet(CFSetRef const &inSet) const
	{
		CFMutableSetRef temp(0);
		if (inSet != 0)
		{
			temp = CFSetCreateMutableCopy(kCFAllocatorDefault, 0, inSet);
		}
		return temp;
	}
	
public:
	QCSet( )
	: set( CFSetCreateMutable(kCFAllocatorDefault, 0, &kCFTypeSetCallBacks) )
	{ }
	
	QCSet(CFMutableSetRef const &inSet)
	: set( inSet )
	{ }
	
	QCSet(CFSetRef const &inSet)
	: set( CFMutableSetFromCFSet(inSet) )
	{
		QCRelease(inSet);
	}
	
	// copy constructor
	QCSet(QCSet const &inSet)
	: set( QCRetain(inSet.set) )
	{ }
	
	// destructor
	~QCSet( )
	{
		QCRelease(set);
	}
	
	void makeUnique()
	{
		if (! null() && CFGetRetainCount(set) > 1)
		{
			CFMutableSetRef newSet = CFSetCreateMutableCopy(kCFAllocatorDefault, 0, set);
			QCRelease(set);
			set = newSet;
		}
	}
	
	bool null() const
	{
		return set == 0;
	}
	
	CFIndex count() const
	{
		return null() ? 0 : CFSetGetCount(set);
	}
	
	bool empty() const
	{
		return count() == 0;
	}

	// conversion operators
	operator CFMutableSetRef ()
	{
		return set;
	}
	
	operator CFSetRef () const
	{
		return set;
	}
	
	QCSet copy() const
	{
		return null() ? QCSet() : QCSet(CFSetCreateMutableCopy(kCFAllocatorDefault, 0, set));
	}
	
	void add(CFTypeRef const &value)
	{
		makeUnique();
		CFSetAddValue(set, value);
	}
	
	bool contains(CFTypeRef const &value) const
	{
		return CFSetContainsValue(set, value) == true;
	}
	
	void show() const;
	
};

typedef QCSet const QCFixedSet;

#endif
