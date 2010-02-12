/*
 *  QCNumber.h
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

#ifndef _QC_NUMBER_GUARD_
#define _QC_NUMBER_GUARD_

#include <CoreFoundation/CoreFoundation.h>
#include <algorithm>
#include "CFRaiiCommon.h"

class QCNumber
{
private:
	CFNumberRef number;
	
public:
	
	QCNumber()
	: number( 0 )
	{ }
	
	QCNumber(CFNumberRef const &inNum)
	: number( inNum )
	{ }
	
	QCNumber(CFIndex const &inNum)
	: number( CFNumberCreate(kCFAllocatorDefault, kCFNumberCFIndexType, &inNum) )
	{ }
	
	QCNumber(int const &inNum)
	: number( CFNumberCreate(kCFAllocatorDefault, kCFNumberIntType, &inNum) )
	{ }
	
	QCNumber(float const &inNum)
	: number( CFNumberCreate(kCFAllocatorDefault, kCFNumberFloatType, &inNum) )
	{ }
	
	QCNumber(double const &inNum)
	: number( CFNumberCreate(kCFAllocatorDefault, kCFNumberDoubleType, &inNum) )
	{ }
	
	QCNumber(long long const &inNum)
	: number( CFNumberCreate(kCFAllocatorDefault, kCFNumberLongLongType, &inNum) )
	{ }
	
	// copy constructor
	QCNumber(QCNumber const &inNum)
	: number( QCRetain(inNum.number) )
	{ }
	
	// destructor
	~QCNumber()
	{
		QCRelease(number);
	}
	
	bool null() const
	{
		return number == 0;
	}
	
	CFNumberType type() const
	{
		return null() ? 0 : CFNumberGetType(number);
	}
	
	// comparison operators
	bool operator == (QCNumber const rhs) const
	{
		return CFNumberCompare(number, rhs.number, NULL) == kCFCompareEqualTo;
	}
	
	bool operator == (CFNumberRef const rhs) const
	{
		return CFNumberCompare(number, rhs, NULL) == kCFCompareEqualTo;
	}
	
	bool operator != (QCNumber const rhs) const
	{
		return !(*this == rhs);
	}
	
	bool operator != (CFNumberRef const rhs) const
	{
		return !(*this == rhs);
	}
	
	bool operator < (QCNumber const rhs) const
	{
		return CFNumberCompare(number, rhs.number, NULL) == kCFCompareLessThan;
	}
	
	bool operator < (CFNumberRef const rhs) const
	{
		return CFNumberCompare(number, rhs, NULL) == kCFCompareLessThan;
	}
	
	// conversion operator
	operator CFNumberRef () const
	{
		return number;
	}
	
	void show() const;
};

typedef QCNumber const QCFixedNumber;

#endif
