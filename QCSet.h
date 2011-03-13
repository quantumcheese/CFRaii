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

BEGIN_QC_NAMESPACE

class QCSet
{
private:
	CFMutableSetRef	mSet;
	CFSetRef		set;
	
public:
	QCSet( )
	: set( NULL )
	, mSet( CFSetCreateMutable(kCFAllocatorDefault, 0, &kCFTypeSetCallBacks) )
	{ }
	
	explicit QCSet(CFMutableSetRef const inSet)
	: set( NULL )
	, mSet( inSet )
	{ }
	
	explicit QCSet(CFSetRef const inSet)
	: set( inSet )
	, mSet( NULL )
	{
		QCRelease(inSet);
	}
	
	// copy constructor
	QCSet(QCSet const &inSet)
	: set( QCRetain(inSet.set) )
	, mSet( QCRetain(inSet.mSet) )
	{ }
	
	// destructor
	~QCSet( )
	{
		QCRelease(set);
		QCRelease(mSet);
	}
	
	CFSetRef Set() const
	{
		return isNotNull(mSet) ? mSet : set;
	}
	
	CFSetRef CFSet() const
	{
		return isNotNull(set) ? set : mSet;
	}
	
	void makeMutable()
	{
		if (mSet != NULL)
		{
			if (set != NULL)
			{
				mSet = CFSetCreateMutableCopy(kCFAllocatorDefault, 0, set);
				CFRelease(set);
				set = NULL;
			}
			else
			{
				mSet = CFSetCreateMutable(kCFAllocatorDefault, 0, &kCFTypeSetCallBacks);
			}

		}
	}
	
	void makeUnique()
	{
		if (! null() && CFGetRetainCount(Set()) > 1)
		{
			CFMutableSetRef newSet = CFSetCreateMutableCopy(kCFAllocatorDefault, 0, Set());
			QCRelease(Set());
			set = NULL;
			mSet = newSet;
		}
	}
	
	bool null() const
	{
		return Set() == NULL;
	}
	
	CFIndex count() const
	{
		return null() ? 0 : CFSetGetCount(Set());
	}
	
	bool empty() const
	{
		return count() == 0;
	}

	// conversion operators
	operator CFSetRef () const
	{
		return Set();
	}
	
	QCSet copy() const
	{
		return QCSet(*this);//null() ? QCSet() : QCSet(CFSetCreateMutableCopy(kCFAllocatorDefault, 0, set));
	}
	
	void add(CFTypeRef const &value)
	{
		makeUnique();
		CFSetAddValue(mSet, value);
	}
	
	bool contains(CFTypeRef const &value) const
	{
		return CFSetContainsValue(Set(), value) == true;
	}
	
	void show() const;
	
};

typedef QCSet const QCFixedSet;

END_QC_NAMESPACE

#endif
