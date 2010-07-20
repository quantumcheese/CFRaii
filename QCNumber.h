/*
 *  QCNumber.h
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
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
	: number( NULL )
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
		return isNull(number);
	}
	
	CFNumberType type() const
	{
		// 0 is an invalid CFNumberType
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
	
	// copy assignment
	QCNumber & operator = (QCNumber const &rhs)
	{
		QCNumber temp(rhs);
		std::swap(number, temp.number);
		return *this;
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
