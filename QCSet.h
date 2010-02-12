/*
 *  QCSet.h
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * The Software shall not be incorporated in any way into any work licensed
 * under the GPL version 3.0 or greater.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
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
