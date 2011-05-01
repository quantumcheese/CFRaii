/*
 *  QCData.h
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */

#ifndef _QC_DATA_GUARD_
#define _QC_DATA_GUARD_

#include <CoreFoundation/CoreFoundation.h>
//#include <algorithm>
#include "CFRaiiCommon.h"

#include "QCString.h"
#include "QCURL.h"

BEGIN_QC_NAMESPACE

class QCData
{
	// class invariant: at most one of data and mData may be non-NULL at a time
private:
	CFMutableDataRef	mData;
	CFDataRef			data;
	
	CFMutableDataRef CFMutableDataFromCFData(CFDataRef const inData) const
	{
		return (inData == NULL) ? NULL : CFDataCreateMutableCopy(kCFAllocatorDefault, 0, inData);
#if 0
		CFMutableDataRef temp(0);
		if (inData != 0)
		{
			temp = CFDataCreateMutableCopy(kCFAllocatorDefault, 0, inData);
		}
		return temp;
#endif
	}
	
public:
	QCData()
	: data( NULL )
	, mData( CFDataCreateMutable(kCFAllocatorDefault, 0) )
	{ }
	
	explicit QCData(CFMutableDataRef const &inData )
	: data( NULL )
	, mData( inData )
	{ }
	
	explicit QCData(CFDataRef const &inData)
	: data( inData )
	, mData( NULL )
	{
		QCRelease(inData);
	}
	
	// copy constructor
	QCData(QCData const &inData)
	: data( QCRetain(inData.data) )
	, mData( QCRetain(inData.mData) )
	{ }
	
	// destructor
	~QCData()
	{
		QCRelease(data);
		QCRelease(mData);
	}
	
	CFDataRef Data () const
	{
		return isNotNull(mData) ? mData : data;
	}
	
	CFDataRef CFData () const
	{
		return isNotNull(data) ? data : mData;
	}
	
	// should be atomic
	void makeMutable()
	{
		if (mData == NULL)
		{
			if (data != NULL)
			{
				mData = CFMutableDataFromCFData(data);
				CFRelease(data);
				data = NULL;
			}
			else {
				mData = CFDataCreateMutable(kCFAllocatorDefault, 0);
			}
			
		}
	}
	
	// should be atomic
	void makeUnique()
	{
		if (!isNull(mData) && CFGetRetainCount(mData) > 1)
		{
			// someone else owns it now
			CFMutableDataRef newData = CFDataCreateMutableCopy(kCFAllocatorDefault, 0, mData);
			CFRelease(mData);
			mData = newData;
		}
		return;
		
		if (! null() && CFGetRetainCount(Data()) > 1)
		{
			CFMutableDataRef newData = CFDataCreateMutableCopy(kCFAllocatorDefault, 0, Data());
			QCRelease(Data());
			data = NULL;
			mData = newData;
		}
	}
	
	bool null() const
	{
		return isNull(Data());
	}
	
	CFIndex length () const
	{
		return CFDataGetLength(Data());
	}
	
	QCData copy() const
	{
		return QCData(*this);//null() ? QCData() : QCData(CFDataCreateMutableCopy(kCFAllocatorDefault, 0, data));
	}
	
	void GetBytes(UInt8 *buffer) const
	{
		if (!null())
		{
			CFDataGetBytes(Data(), CFRangeMake(0, length()), buffer);
		}
	}
	
	UInt8 const * GetBytePtr() const
	{
		return null() ? 0 : CFDataGetBytePtr(Data());
	}
	
	// operators
	
	// copy assignment
	QCData & operator = (QCData const &rhs)
	{
		// copy and swap
		QCData temp(rhs);
		std::swap(data, temp.data);
		std::swap(mData, temp.mData);
		return *this;
	}
	
	// comparison operators
	bool operator == (QCData const &rhs) const
	{
		return (Data() == rhs.Data())
		|| (CFEqual(Data(), rhs.Data()) == true);
	}
	
	bool operator != (QCData const &rhs) const
	{
		return !(*this == rhs);
	}
	
	// conversion operator
	operator CFDataRef () const
	{
		return Data();
	}
	
	void AppendBytes(UInt8 const * const buffer, CFIndex const &bufferSize)
	{
		makeMutable();
		makeUnique();
		CFDataAppendBytes(mData, buffer, bufferSize);
	}
	
	void show() const;
	
	bool writeToFile(CFStringRef const filePath) const;
	
	static QCData dataFromFile(CFURLRef const fileURL);
	static QCData dataFromFile(CFStringRef const filePath);
};

typedef QCData const QCFixedData;

END_QC_NAMESPACE

#endif
