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
	CFMutableDictionaryRef	mDict;
	CFDictionaryRef			dict;
	
	CFMutableDictionaryRef CFMutableDictionaryFromCFDictionary(CFDictionaryRef const inDict) const
	{
		return (inDict == NULL) ? NULL : CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0, inDict);
#if 0
		CFMutableDictionaryRef temp(0);
		if (inDict != 0)
		{
			temp = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0, inDict);	
		}
		return temp;
#endif
	}
	
public:
	QCDictionary( )
	: dict( NULL)
	, mDict( CFDictionaryCreateMutable(kCFAllocatorDefault, 0, &kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks) )
	{ }
	
	explicit QCDictionary(CFMutableDictionaryRef const &inDict)
	: dict( NULL)
	, mDict( inDict )
	{ }
	
	explicit QCDictionary(CFDictionaryRef const &inDict)
	: dict( inDict )
	, mDict( NULL )
	{ }
	
	// copy constructor
	QCDictionary(QCDictionary const &inDict)
	: dict( QCRetain(inDict.dict) )
	, mDict( QCRetain(inDict.mDict) )
	{ }
	
	// destructor
	~QCDictionary( )
	{
		QCRelease(dict);
		QCRelease(mDict);
	}
	
	CFDictionaryRef Dictionary () const
	{
		return isNotNull(mDict) ? mDict : dict;
	}
	
	CFDictionaryRef CFDictionary () const
	{
		return isNotNull(dict) ? dict : mDict;
	}
	
	void makeMutable()
	{
		if (mDict == NULL)
		{
			if (dict != NULL)
			{
				mDict = CFMutableDictionaryFromCFDictionary(dict);
				QCRelease(dict);
				dict = NULL;
			}
			else
			{
				mDict = CFDictionaryCreateMutable(kCFAllocatorDefault
												  , 0
												  , &kCFTypeDictionaryKeyCallBacks
												  , &kCFTypeDictionaryValueCallBacks);
			}

		}
	}
	
	void makeUnique()
	{
		if (! null() && CFGetRetainCount(Dictionary()) > 1)
		{
			CFMutableDictionaryRef newDict = CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0, Dictionary());
			QCRelease(Dictionary());
			dict = NULL;
			mDict = newDict;
		}
	}
	
	bool null() const
	{
		return isNull(Dictionary());
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
		return QCDictionary(*this); // null() ? QCDictionary() : QCDictionary(CFDictionaryCreateMutableCopy(kCFAllocatorDefault, 0, dict));
	}
	
	// operators
	
	// copy assignment
	QCDictionary & operator = (QCDictionary const &rhs)	
	{
		// copy and swap
		QCDictionary temp(rhs);
		std::swap(dict, temp.dict);
		std::swap(mDict, temp.mDict);
		return *this;
	}
	
	// comparison operators
	
	bool operator == (QCDictionary const &rhs) const
	{
		return (Dictionary() == rhs.Dictionary())
				|| (CFEqual(Dictionary(), rhs.Dictionary()) == true);
	}
	
	bool operator != (QCDictionary const &rhs) const
	{
		return !(*this == rhs);
	}
	
#pragma mark class CFTypeProxy
	class CFTypeProxy
	{
	private:
		CFDictionaryRef	dict;
		CFTypeRef		key;
		
		// no copy-assigmnet
		CFTypeProxy & operator = (CFTypeProxy const &);
		
	public:
		// ctor
		CFTypeProxy(CFDictionaryRef const dictRef, CFTypeRef const keyRef)
		: dict( QCRetain(dictRef) ), key( QCRetain(keyRef) )
		{ }
		
		// copy ctor
		CFTypeProxy(CFTypeProxy const &proxy)
		: dict( QCRetain(proxy.dict) ), key( QCRetain(proxy.key) )
		{ }
		
		// dtor
		~CFTypeProxy()
		{
			QCRelease(dict);
			QCRelease(key);
		}
		
		// comparison operators
		bool operator == (CFTypeProxy const &rhs) const
		{
			// could use CFEqual(array, rhs.array)
			return dict == rhs.dict && key == rhs.key;
		}
		
		bool operator != (CFTypeProxy const &rhs) const
		{
			return !(*this == rhs);
		}
		
		// conversion operator
		operator CFTypeRef () const
		{
			return CFDictionaryGetValue(dict, key);
		}
	}; // class CFTypeProxy
	
#pragma mark class CFMutableTypeProxy
	class CFMutableTypeProxy
	{
	private:
		CFMutableDictionaryRef	dict;
		CFTypeRef				key;
		
		// no copy-assignment
		CFMutableTypeProxy & operator = (CFMutableTypeProxy const &);
		
	public:
		// ctor
		CFMutableTypeProxy(CFMutableDictionaryRef const dictRef, CFTypeRef const keyRef)
		: dict( QCRetain(dictRef) ), key( QCRetain(keyRef) )
		{ }
		
		// copy ctor
		CFMutableTypeProxy(CFMutableTypeProxy const &proxy)
		: dict( QCRetain(proxy.dict) ), key( QCRetain(proxy.key) )
		{ }
		
		// dtor
		~CFMutableTypeProxy()
		{
			QCRelease(dict);
			QCRelease(key);
		}
		
		// conversion operator
		operator CFTypeRef () const
		{
			return CFDictionaryGetValue(dict, key);
		}
		
		// assignment replaces or adds an element in the dictionary
		CFMutableTypeProxy & operator = (CFTypeRef const newValue)
		{
			// 'this' doesn't change, but the pointed-to dictionary does
			// "add if absent, replace if present"
			CFDictionarySetValue(dict, key, newValue);
			return *this;
		}
	}; // class CFMutableTypeProxy
	
	// accessors
	
	CFMutableTypeProxy operator [] (CFTypeRef const key)
	{
		makeMutable();
		makeUnique();
		return CFMutableTypeProxy(mDict, key);
	}
	
	CFTypeRef operator [] (CFTypeRef const key) const
	{
#if 0
		return (*this)(key); // see below
//		return CFTypeProxy(Dictionary(), key);
	}
	
	CFTypeRef operator () (CFTypeRef const key) const
	{
#endif
		return (null() || key == NULL)
		? NULL
		: CFDictionaryGetValue(Dictionary(), key);
	}
#if 0
	void operator () (CFTypeRef const key, CFTypeRef const value)
	{
		makeMutable();
		makeUnique();
		CFDictionarySetValue(mDict, key, value);
	}
#endif
	void addValue(CFTypeRef const key, CFTypeRef const value)
	{
		makeUnique();
		makeMutable();
		CFDictionaryAddValue(mDict, key, value);
	}
	void setValue(CFTypeRef const key, CFTypeRef const value)
	{
		makeUnique();
		makeMutable();
		CFDictionarySetValue(mDict, key, value);
	}
	
	operator CFDictionaryRef () const
	{
		return Dictionary();
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
