/*
 *  QCDictionary.cpp
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 *
 * See license.txt for licensing terms and conditions.
 */

#include "QCDictionary.h"
#include "QCURL.h"
#include "QCString.h"

void QCDictionary::show() const
{
#ifndef NDEBUG
	CFShow(dict);
#endif
}

bool QCDictionary::writeToFile(QCString const &filePath, CFPropertyListFormat const format) const
{
	QCURL fileURL(filePath, 0); // is not directory
	CFWriteStreamRef writeStream(CFWriteStreamCreateWithFile(kCFAllocatorDefault, fileURL));
	
	bool result(CFWriteStreamOpen(writeStream) != 0); // convert from Boolean
	if (result)
	{
		// don't care about the error string
		result = CFPropertyListWriteToStream(dict, writeStream, format, 0) != 0;
		
		CFWriteStreamClose(writeStream);
	}
	
	QCRelease(writeStream);
	
	return result;
}

// static
QCDictionary QCDictionary::dictionaryFromFile(QCString const &filePath, CFPropertyListFormat &plistFormat)
{
	QCURL fileURL(filePath, false);
	CFPropertyListRef plist(0);
	CFStringRef errStr(0);
	
	CFReadStreamRef readStream = CFReadStreamCreateWithFile(kCFAllocatorDefault, fileURL);
	if (CFReadStreamOpen(readStream) == 0)
	{
		// couldn't read file; return an empty dictionary
		CFRelease(readStream);
		return QCDictionary();
	}
	
	plist = CFPropertyListCreateFromStream(kCFAllocatorDefault
										   , readStream
										   , 0 // full file
										   , kCFPropertyListMutableContainersAndLeaves
										   , &plistFormat
										   , &errStr);
	CFReadStreamClose(readStream);
	QCRelease(readStream);
	
	if (errStr != 0
		// if the root of the plist is not a dictionary,
		// then we want nothing to do with it.
		|| CFGetTypeID(plist) != CFDictionaryGetTypeID())
	{
		QCRelease(plist);
		return QCDictionary();
	}
	
	return QCDictionary(static_cast<CFDictionaryRef> ( plist ));
}
