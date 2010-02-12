/*
 *  QCData.h
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

#ifndef _QC_DATA_GUARD_
#define _QC_DATA_GUARD_

#include <CoreFoundation/CoreFoundation.h>
//#include <algorithm>
#include "CFRaiiCommon.h"

#include "QCString.h"
#include "QCURL.h"

class QCData
{
private:
	CFMutableDataRef data;
	
	CFMutableDataRef CFMutableDataFromCFData(CFDataRef const &inData) const
	{
		CFMutableDataRef temp(0);
		if (inData != 0)
		{
			temp = CFDataCreateMutableCopy(kCFAllocatorDefault, 0, inData);
		}
		return temp;
	}
	
public:
	QCData()
	: data( CFDataCreateMutable(kCFAllocatorDefault, 0) )
	{ }
	
	QCData(CFMutableDataRef const &inData )
	: data( inData )
	{ }
	
	QCData(CFDataRef const &inData)
	: data( CFMutableDataFromCFData(inData) )
	{
		QCRelease(inData);
	}
	
	// copy constructor
	QCData(QCData const &inData)
	: data( QCRetain(inData.data) )
	{ }
	
	// destructor
	~QCData()
	{
		QCRelease(data);
	}
	
	void makeUnique()
	{
		if (! null() && CFGetRetainCount(data) > 1)
		{
			CFMutableDataRef newData = CFDataCreateMutableCopy(kCFAllocatorDefault, 0, data);
			QCRelease(data);
			data = newData;
		}
	}
	
	bool null() const
	{
		return data == 0;
	}
	
	CFIndex length () const
	{
		return CFDataGetLength(data);
	}
	
	QCData copy() const
	{
		return null() ? QCData() : QCData(CFDataCreateMutableCopy(kCFAllocatorDefault, 0, data));
	}
	
	void GetBytes(UInt8 *buffer) const
	{
		if (!null())
		{
			CFDataGetBytes(data, CFRangeMake(0, length()), buffer);
		}
	}
	
	UInt8 const * GetBytePtr() const
	{
		return null() ? 0 : CFDataGetBytePtr(data);
	}
	
	// operators
	
	// assignment operators
	// TODO: implement ==, !=
	bool operator == (QCData const &rhs) const
	{
		return (data == rhs.data)
				|| (CFEqual(data, rhs.data) == true);
	}
	
	bool operator != (QCData const &rhs) const
	{
		return !(*this == rhs);
	}
	
	// conversion operators
	operator CFMutableDataRef ()
	{
		return data;
	}
	
	operator CFDataRef () const
	{
		return data;
	}
	
	void AppendBytes(UInt8 const * const buffer, CFIndex const &bufferSize)
	{
		makeUnique();
		CFDataAppendBytes(data, buffer, bufferSize);
	}
	
	void show() const;
	
	bool writeToFile(QCString const &filePath) const;
	
	static QCData dataFromFile(QCURL const &fileURL);
	static QCData dataFromFile(QCString const &filePath);
};

typedef QCData const QCFixedData;

#endif
