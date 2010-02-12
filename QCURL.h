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
	: url( 0 )
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
#if 0
	void makeUnique()
	{
		if (! null() && CFGetRetainCount(url) > 1)
		{
			// TODO: figure out how to copy a CFURL object
			#warning url not copied in makeUnique
//			CFURLREF newURL = ? ;
		}
	}
#endif
	bool null() const
	{
		return url == 0;
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
#if 0
	QCURL & operator += (CFStringRef const &rhs)
	{
		if (!null() && rhs != 0)
		{
			makeUnique();
			QCURL newURL(CFURLCreateCopyAppendingPathExtension(kCFAllocatorDefault, url, rhs));
			std::swap(url, newURL.url);
		}
		return *this;
	}
	
	QCURL & operator += (QCString const &rhs)
	{
		makeUnique();
		// static cast to avoid infinte loop
		return (*this += static_cast<CFStringRef const> (rhs));
	}
#endif
	// we don't need an overload to take a QCURL since QCURL will convert to CFURLRef
	QCURL operator + (CFStringRef const &rhs) const
	{
		if (!null() && rhs != 0)
		{
			CFURLRef temp(CFURLCreateCopyAppendingPathExtension(kCFAllocatorDefault, url, rhs));
			return QCURL(temp);
		}
		return QCURL();
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
	
	void throwIfNull() const
	{
		if (null())
		{
			throw std::exception();
		}
	}

	
	QCString FileSystemRepresentation() const
	{
		throwIfNull();
		UInt8 buffer[qcBufferSize];
		if (CFURLGetFileSystemRepresentation(url, true, buffer, qcBufferSize) == true)
		{
			// copies only the part of the buffer that was used
			return QCString(reinterpret_cast<char *> (buffer) );
		}
		return QCString();
	}
	
	QCString LastPathComponent() const
	{
		throwIfNull();
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
