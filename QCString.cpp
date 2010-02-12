/*
 *  QCString.cpp
 *  CFRaii
 *
 * Copyright (c) 2009-2010 Richard A. Brown
 *
 * See license.txt for licensing terms and conditions.
 */

#include "QCString.h"

#include "QCData.h"

char *QCString::CString() const
{
	return CString_malloc();
}

// returns a C-string that requires free()ing
char *QCString::CString_malloc() const
{
	if (null()) return 0;
	
	CFIndex length = CFStringGetMaximumSizeOfFileSystemRepresentation(string);
	char *buffer = static_cast<char *> (malloc(length * sizeof(char)));
	
	if (CFStringGetFileSystemRepresentation(String(), buffer, length) == true)
	{
		return buffer;
	}
	// fall-through failure case
	free(buffer);
	return 0;
}

// returns a C-string that requires delete-ing
char *QCString::CString_new() const
{
	if (null()) return 0;
	
	CFIndex length = CFStringGetMaximumSizeOfFileSystemRepresentation(string);
	char *buffer = new char[length];
	
	if (CFStringGetFileSystemRepresentation(String(), buffer, length) == true)
	{
		return buffer;
	}
	// fall-through failure case
	delete buffer;
	return 0;
}

std::string QCString::StdString() const
{
	// not very efficient
	char * const cString = CString_new();
	std::string stdStr(cString);
	delete cString;
	
	return stdStr;
}

void QCString::show() const
{
#ifndef NDEBUG
	CFShow(String());
#endif
}

bool QCString::writeToFile(QCString const &filePath) const
{
	char *buffer = CString();
	
	QCData bytes( CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, reinterpret_cast<UInt8 *> (buffer), length(), kCFAllocatorNull) );
	bool written = bytes.writeToFile(filePath);
	
	free(buffer);
	
	return written;
}
