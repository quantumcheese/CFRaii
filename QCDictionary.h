/*
 *  QCDictionary.h
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */

#ifndef _QC_DICTIONARY_GUARD_
#define _QC_DICTIONARY_GUARD_

#include <CoreFoundation/CoreFoundation.h>
#include <algorithm>
#include "CFRaiiCommon.h"

#include "QCString.h"
#include "QCURL.h"

class QCDictionary
{
	CFMutableDictionaryRef dict;
	
	CFMutableDictionaryRef CFMutableDictionaryFromCFDictionary(CFDictionaryRef const &inDict) const
	{
		CFMutableDictionaryRef temp(0);
		if (inDict != 0)
		{
			temp = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0, inDict);	
		}
		return temp;
	}
	
public:
	QCDictionary( )
	: dict( CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks) )
	{ }
	
	QCDictionary(CFMutableDictionaryRef const &inDict)
	: dict( inDict )
	{
		// do not release the dictionary!
	}
	
	QCDictionary(CFDictionaryRef const &inDict)
	: dict( CFMutableDictionaryFromCFDictionary(inDict) )
	{
		// because we're 'taking ownership' of this dictionary, release the original
		QCRelease(inDict);
	}
	
	// copy constructor
	QCDictionary(QCDictionary const &inDict)
	: dict( QCRetain(inDict.dict) )
	{ }
	
	// destructor
	~QCDictionary( )
	{
		QCRelease(dict);
	}
	
	void makeUnique()
	{
		if (! null() && CFGetRetainCount(dict) > 1)
		{
			CFMutableDictionaryRef newDict = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0, dict);
			QCRelease(dict);
			dict = newDict;
		}
	}
	
	bool null() const
	{
		return dict == 0;
	}
	
	CFIndex count() const
	{
		return null() ? 0 : CFDictionaryGetCount(dict);
	}
	
	bool empty() const
	{
		return count() == 0;
	}
	
	QCDictionary copy() const
	{
		return null() ? QCDictionary() : QCDictionary(CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0, dict));
	}
	
	// Operators
	
	// assignment
	QCDictionary & operator = (QCDictionary &rhs)	
	{
		if (dict != rhs.dict)
		{
			QCRelease(dict);
			dict = QCRetain(rhs.dict);
		}
		return *this;
	}
	
	QCDictionary & operator = (QCDictionary const &rhs)
	{
		QCDictionary temp(rhs);
		std::swap(dict, temp.dict);
		return *this;
	}
	
	// comparison operators
	
	bool operator == (QCDictionary const &rhs) const
	{
		return (dict == rhs.dict)
				|| (CFEqual(dict, rhs.dict) == true);
	}
	
	bool operator != (QCDictionary const &rhs) const
	{
		return !(*this == rhs);
	}
	
	// accessors
	
	CFTypeRef operator [] (CFTypeRef const key) const
	{
		return (*this)(key);
		return null() ? 0 : CFDictionaryGetValue(dict, key);
	}
	
	CFTypeRef operator () (CFTypeRef const key) const
	{
		return (null() || key == 0)
		? 0
		: CFDictionaryGetValue(dict, key);
	}
	
	void operator () (CFTypeRef const key, CFTypeRef const value)
	{
		if (!null())
		{
			makeUnique();
			CFDictionarySetValue(dict, key, value);
		}
	}
	
	void addValue(CFTypeRef const key, CFTypeRef const value)
	{
		makeUnique();
		CFDictionaryAddValue(dict, key, value);
	}
	void setValue(CFTypeRef const key, CFTypeRef const value)
	{
		makeUnique();
		CFDictionarySetValue(dict, key, value);
	}
	
	// conversion operators
	operator CFMutableDictionaryRef ()
	{
		return dict;
	}
	
	operator CFDictionaryRef () const
	{
		return dict;
	}
	
	void show() const;
	
	bool writeToFile(QCString const &filePath, CFPropertyListFormat const format = kCFPropertyListXMLFormat_v1_0) const;
	
	static QCDictionary dictionaryFromFile(QCString const &filePath)
	{
		CFPropertyListFormat plistFormat;
		return dictionaryFromFile(filePath, plistFormat);
	}
	
	static QCDictionary dictionaryFromFile(QCString const &filePath, CFPropertyListFormat &plistFormat);
	
};

typedef QCDictionary const QCFixedDictionary;

#endif