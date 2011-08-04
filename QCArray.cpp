/*
 *  QCArray.cpp
 *  CFRaii
 *
 * Copyright (c) 2009-2010 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */

#include "QCArray.h"
#include "QCURL.h"
#include "QCString.h"

BEGIN_QC_NAMESPACE

void QCArray1::show() const
{
#ifndef NDEBUG
	CFShow(Array());
#endif
}

bool QCArray1::writeToFile(QCString const &filePath, CFPropertyListFormat const format = kCFPropertyListXMLFormat_v1_0) const
{
	QCURL fileURL(filePath, 0); // is not directory
	CFWriteStreamRef writeStream(CFWriteStreamCreateWithFile(kCFAllocatorDefault, fileURL));
	
	bool result(CFWriteStreamOpen(writeStream) != 0); // convert from Boolean
	if (result)
	{
		// don't care about the error string
		result = CFPropertyListWriteToStream(array, writeStream, format, NULL) != 0;
		
		CFWriteStreamClose(writeStream); // we're not ignoring anything -- it returns void
	}
	
	Release(writeStream);
	
	return result;
}

// static method
QCArray1 QCArray1::arrayFromFile(QCString const &filePath)
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
		return QCArray1();
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
		Release(plist);
		return QCArray1();
	}
	
	return QCArray1(static_cast<CFArrayRef> ( plist ));
}

END_QC_NAMESPACE
