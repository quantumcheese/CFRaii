/*
 *  QCString.h
 *  CFRaii
 *
 * Copyright (c) 2009-2010 Richard A. Brown
 *
 * See license.txt for licensing terms and conditions.
 */

#ifndef _QC_STRING_GUARD_
#define _QC_STRING_GUARD_

#include <CoreFoundation/CoreFoundation.h>
#include <algorithm>
#include "CFRaiiCommon.h"
#include <CoreServices/CoreServices.h>

#include <iostream>
#include <string>

// this class is intended as an RAII wrapper for CFStringRefs
class QCString
{
private:
	// class invariant: only one of string and mString may be non-NULL at a time
	CFMutableStringRef	mString;
	CFStringRef			string;
	
	CFMutableStringRef CFMutableStringFromCFString(CFStringRef const &inString) const
	{
		CFMutableStringRef temp(NULL);
		if (inString != NULL)
		{
			temp = CFStringCreateMutableCopy(kCFAllocatorDefault, 0, inString);
		}
		return temp;
	}
	
	CFStringRef CFStringFromCString(char const * const inString) const
	{
		CFStringRef temp(NULL);
		if (inString != NULL)
		{
			temp = CFStringCreateWithCString(kCFAllocatorDefault, inString, kCFStringEncodingUTF8);
		}
		return temp;
	}
	
	CFStringRef CFStringFromHFSUniStr255(HFSUniStr255 const &inString) const
	{
		CFStringRef temp(NULL);
		
		// make sure the string has positive length
		if (0 < inString.length)
		{
			temp = CFStringCreateWithCharacters(kCFAllocatorDefault, inString.unicode, inString.length);
		}
		
		return temp;
	}
	
public:
	QCString( )
	: string( NULL )
	, mString( NULL )
	{ }
	
	QCString(CFMutableStringRef const &inString)
	: string( NULL )
	, mString( inString )
	{ }
	
	QCString(CFStringRef const &inString)
	: string( inString )
	, mString( NULL ) // maybe we'll never need it
	{ }
	
	QCString(HFSUniStr255 const &inString)
	: string( CFStringFromHFSUniStr255(inString) )
	, mString( NULL )
//	: mString( CFMutableStringFromHFSUniStr255(inString) )
	{ }
	
	// creating a CFString from a C-string
	QCString(char const * const inString)
	: string( CFStringFromCString(inString) )
	, mString( NULL )
	{ }
	
	QCString(std::string const &inString)
	: string( CFStringFromCString(inString.c_str()) )
	, mString( NULL )
	{ }
	
	
	// copy constructor
	QCString(QCString const &inString)
	: string( QCRetain(inString.string) )
	, mString( QCRetain(inString.mString) )
	{ }
	
	// destructor
	~QCString()
	{
		QCRelease(string);
		QCRelease(mString);
	}
	
	CFStringRef String() const
	{
		return isNotNull(mString) ? mString : string;
	}
	
	CFStringRef CFString() const
	{
		return isNotNull(string) ? string : mString;
	}
	
	
	// not threadsafe!  (should become an atomic function)
	void makeUnique()
	{
		// don't care if 'string' is shared, just 'mString'
		
		if (!isNull(mString) && CFGetRetainCount(mString) > 1)
		{
			// relinquish our ownership
			CFMutableStringRef newString = CFStringCreateMutableCopy(kCFAllocatorDefault, 0, mString);
			CFRelease(mString);
			mString = newString;
		}
		return;
		
		if (isNull(mString))
		{
			// new string will be owned by only us
			mString = CFMutableStringFromCFString(string);
			// remove string to preserve class invariant
			QCRelease(string);
			string = 0;
		}
		else if (CFGetRetainCount(mString) > 1)
		{
			// make fresh copy of mString for ourselves to modify
			CFMutableStringRef newString = CFStringCreateMutableCopy(kCFAllocatorDefault, 0, mString);
			QCRelease(mString);
			mString = newString;
		}
	}
	
	bool null() const
	{
		return isNull(mString) && isNull(string);
	}
	
	CFIndex length() const
	{
		CFStringRef str = String();
		return isNull(str) ? 0 : CFStringGetLength(str);
	}
	
	// returns a deep copy
	QCString copy() const
	{
		CFStringRef str = String();
		return QCString(str);
	}
	
	UniChar at(CFIndex const idx) const
	{
		return CFStringGetCharacterAtIndex(String(), idx);
	}
	
	// Operators
	
	// copy assignment -- copy and swap
	QCString & operator = (QCString const &rhs)
	{
		QCString temp(rhs);
		std::swap(mString, temp.mString);
		std::swap(string, temp.string);
		return *this;
	}
	
	// comparison operators
	bool operator == (QCString const &rhs) const
	{
		return (String() == rhs.String()) // optimization
				|| (CFStringCompare(String(), rhs.String(), 0) == kCFCompareEqualTo);
	}
	
	bool operator != (QCString const &rhs) const
	{
		return !(*this == rhs);
	}
	
	bool operator < (QCString const &rhs) const
	{
		return (String() != rhs.String())
				&& (CFStringCompare(String(), rhs.String(), 0) == kCFCompareLessThan);
	}
	
	bool operator > (QCString const &rhs) const
	{
		return (String() != rhs.String())
				&& (CFStringCompare(String(), rhs.String(), 0) == kCFCompareGreaterThan);
	}
	
	// conversion operators
	
	operator CFStringRef () const
	{
		// can use a MutableString when you want a non-mutable, but not the reverse
		return String();
	}

	// concatenate operator
	QCString & operator += (QCString const &rhs)
	{
//		makeMutable();
		makeUnique();
//		return (*this += rhs.string); // would this work?
		
		if (isNull(mString))
		{
			// this case is possible only if string and mString were both NULL before calling makeUnique
			mString = CFStringCreateMutableCopy(kCFAllocatorDefault, 0, rhs.String());
		}
		else
		{
			CFStringAppend(mString, rhs.String());
		}
		return *this;
	}
	
	QCString & operator += (CFMutableStringRef const &rhs)
	{
		if (rhs != 0)
		{
//			makeMutable();
			makeUnique();
			if (isNull(mString))
			{
				mString = QCRetain(rhs);
			}
			else
			{
				CFStringAppend(mString, rhs);
			}
		}
		return *this;
	}
	
	QCString & operator += (CFStringRef const &rhs)
	{
		if (rhs != 0)
		{
//			makeMutable();
			makeUnique();
			
			if (isNull(mString))
			{
				mString = CFMutableStringFromCFString(rhs);
			}
			else
			{
				CFStringAppend(mString, rhs);
			}
		}
		return *this;
	}
	
	QCString & operator += (char const *rhs)
	{
		makeUnique();
		
		return (rhs == NULL) ? *this : *this += QCString(rhs);
	}
	
	bool hasPrefix(CFStringRef const &prefix) const
	{
		return CFStringHasPrefix(String(), prefix) == true;
	}
	
	bool hasSuffix(CFStringRef const &suffix) const
	{
		return CFStringHasSuffix(String(), suffix) == true;
	}
	
	OSStatus FSRefFromPath(FSRef &ref, bool &isDirectory) const
	{
		char * const cPath = CString();
		if (cPath == NULL) return false;
		
		Boolean isDir;
		
		OSStatus err = FSPathMakeRefWithOptions(reinterpret_cast<UInt8 *> ( cPath ), kFSPathMakeRefDefaultOptions, &ref, &isDir);
		isDirectory = isDir == true; // convert from Boolean to bool
		
		free(cPath);
		
		return err;
	}
	
	OSStatus FSRefFromPath(FSRef &ref) const
	{
		char * const cPath = CString();
		if (cPath == NULL) return false;
		
		OSStatus err = FSPathMakeRefWithOptions(reinterpret_cast<UInt8 *> ( cPath ), kFSPathMakeRefDefaultOptions, &ref, 0);
		free(cPath);
		
		return err;
	}
	
	QCString substring(CFIndex startPos, CFIndex endPos) const
	{
		CFIndex const lengthCache = length();
		// cases of undefined behavior
		if (isNull(String())
			// ensure valid range
			|| endPos <= startPos
			|| startPos < 0
			|| lengthCache < startPos
			|| endPos < 1
			|| lengthCache < endPos
			)
		{
			// return empty string
			return QCString();
		}
		
		CFRange range = CFRangeMake(startPos, endPos - startPos + 1); // + 1 to compensate for 0-indexing
		CFStringRef temp = CFStringCreateWithSubstring(kCFAllocatorDefault, String(), range);
		
		return QCString(temp); // takes ownership & releases when done
	}
	
	QCString substring(CFRange range) const
	{
		return QCString( CFStringCreateWithSubstring(kCFAllocatorDefault, String(), range) );
//		return substring(range.location, range.location + range.length);
		/* more readable form: 
		CFIndex startPos = range.location;
		CFIndex endPos = range.location + range.length;
		
		return substring(startPos, endPos);
		 */
	}
	
	CFIndex stringIndex(CFStringRef const &searchString, CFRange const &range) const
	{
		// check range
		if (range.location < 0
			|| range.length < 1
			|| length() < range.location + range.length)
		{
			return kCFNotFound;
		}
		CFIndex location = (CFStringFind(String(), searchString, 0)).location + range.location;
		return location;
	}
	
	CFIndex stringIndex(CFStringRef const &searchString, CFIndex const startPos, CFIndex const endPos) const
	{
		return stringIndex(searchString, CFRangeMake(startPos, endPos - startPos));
	}
	
	CFIndex charIndex(char const c, CFIndex const startPos, CFIndex const endPos) const
	{
		CFIndex const lengthCache = length();
		// check range
		if (null()
			|| endPos <= startPos
			|| startPos < 0
			|| lengthCache < startPos
			|| endPos < 1
			|| lengthCache < endPos
			)
		{
			return kCFNotFound;
		}
		// create c string from char
		char str[2] = {c, '\0'};
		
		return CFStringFind(substring(startPos, endPos), QCString(str), 0).location + startPos;
	}
	
	// returns a C-string that requires free()ing
	char *CString() const;
	char *CString_malloc() const;
	// returns a C-string that requires delete-ing
	char *CString_new() const;
	
	std::string StdString() const;
	
	void show() const;
	
	bool writeToFile(QCString const &filePath) const;
	
};


inline QCString operator + (QCString const &lhs, QCString const &rhs)
{
	QCString temp(lhs);
	temp += rhs;
	return temp;
}

inline QCString operator + (QCString const &lhs, CFMutableStringRef const &rhs)
{
	QCString temp(lhs);
	temp += rhs;
	return temp;
}

inline QCString operator + (QCString const &lhs, CFStringRef const &rhs)
{
	QCString temp(lhs);
	temp += rhs;
	return temp;
}

inline QCString operator + (QCString const &lhs, char const *rhs)
{
	QCString temp(lhs);
	temp += rhs;
	return temp;
}

template <typename OStream>
OStream & operator << (OStream & os, QCString const &str)
{
	char const *c_str = str.CString_new();
	if (c_str != NULL)
	{
		os << c_str;
		delete c_str;
	}
	return os;
}

typedef QCString const QCFixedString;

#endif
