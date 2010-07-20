/*
 *  QCURL.h
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 *
 * See license.txt for licensing terms and conditions.
 */

#ifndef _QC_URL_GUARD_
#define _QC_URL_GUARD_

#define qcBufferSize (4096)

#include <CoreFoundation/CoreFoundation.h>
#include <algorithm>
#include "CFRaiiCommon.h"

#include "QCString.h"

class QCURL
{
	CFURLRef url;
	
	CFURLRef CFURLFromPath(CFStringRef const &path, Boolean isDir) const
	{
		CFURLRef temp(0);
		if (path != 0)
		{
			temp = CFURLCreateWithFileSystemPath(kCFAllocatorDefault
												 , path
												 , kCFURLPOSIXPathStyle
												 , isDir);
		}
		return temp;
	}
	
public:
	QCURL()
	: url( NULL )
	{ }
	
	QCURL(CFURLRef const &inURL)
	: url( inURL )
	{
		// do not release the URL!
	}
	
	QCURL(CFStringRef const &path, Boolean isDir)
	: url( CFURLFromPath(path, isDir) )
	{
		// do not release the path string!
	}
	
	// copy constructor
	QCURL(QCURL const &inURL)
	: url( QCRetain(inURL.url) )
	{ }
	
	~QCURL()
	{
		QCRelease(url);
	}
	
	bool null() const
	{
		return url == NULL;
	}
	
	// Operators
	
	// copy assignment
	QCURL & operator = (QCURL const &rhs)
	{
		QCRelease(url);
		url = QCRetain(rhs.url);
		return *this;
	}
	
	// comparison operators
	
	bool operator == (QCURL const &rhs) const
	{
		return (url == rhs.url)
				|| (CFEqual(url, rhs.url) == true);
	}
	
	bool operator != (QCURL const &rhs) const
	{
		return !(*this == rhs);
	}
	
	// conversion operators
	operator CFURLRef () const
	{
		return url;
	}
	
	bool GetFSRef (FSRef &ref) const
	{
		return CFURLGetFSRef(url, &ref) != 0; // convert from Boolean
	}
	
	QCString FileSystemRepresentation() const
	{
		if (null())
		{
			return QCString(static_cast<CFStringRef>(NULL));
		}
		return QCString( CFURLCopyFileSystemPath(url, kCFURLPOSIXPathStyle) );
	}
	
	QCString LastPathComponent() const
	{
		if (null())
		{
			return QCString(static_cast<CFStringRef>(NULL));
		}
		return QCString(CFURLCopyLastPathComponent(url));
	}
	
	// ownership follows the Get Rule
	CFStringRef GetString() const
	{
		return CFURLGetString(url);
	}
		
	void show() const;
	
};

#undef qcBufferSize

#endif
