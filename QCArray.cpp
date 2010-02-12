/*
 *  QCArray.cpp
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

#include "QCArray.h"
#include "QCURL.h"
#include "QCString.h"

void QCArray::show() const
{
#ifndef NDEBUG
	CFShow(array);
#endif
}

bool QCArray::writeToFile(QCString const &filePath, CFPropertyListFormat const format = kCFPropertyListXMLFormat_v1_0) const
{
	QCURL fileURL(filePath, 0); // is not directory
	CFWriteStreamRef writeStream(CFWriteStreamCreateWithFile(kCFAllocatorDefault, fileURL));
	
	bool result(CFWriteStreamOpen(writeStream) != 0); // convert from Boolean
	if (result)
	{
		// don't care about the error string
		result = CFPropertyListWriteToStream(array, writeStream, format, 0) != 0;
		
		CFWriteStreamClose(writeStream);
	}
	
	QCRelease(writeStream);
	
	return result;
}

// static method
QCArray QCArray::arrayFromFile(QCString const &filePath)
{
	QCURL fileURL(filePath, false);
	CFPropertyListRef plist(0);
	CFStringRef errStr(0);
	CFPropertyListFormat plistFormat;
	
	CFReadStreamRef readStream = CFReadStreamCreateWithFile(kCFAllocatorDefault, fileURL);
	if (CFReadStreamOpen(readStream) == 0)
	{
		// couldn't read file; return an empty array
		CFRelease(readStream);
		return QCArray();
	}
	
	plist = CFPropertyListCreateFromStream(kCFAllocatorDefault
										   , readStream
										   , 0 // full file
										   , kCFPropertyListMutableContainersAndLeaves
										   , &plistFormat
										   , &errStr);
	CFReadStreamClose(readStream);
	CFRelease(readStream);
	
	if (errStr != 0
		// if the root of the plist is not an array,
		// then we want nothing to do with it.
		|| CFGetTypeID(plist) != CFArrayGetTypeID())
	{
		QCRelease(plist);
		return QCArray();
	}
	
	return QCArray(static_cast<CFArrayRef> ( plist ));
}
