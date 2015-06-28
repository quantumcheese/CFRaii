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

BEGIN_QC_NAMESPACE

// this class is intended as an RAII wrapper for CFStringRefs
class QCString
{
private:
    // class invariant: only one of string and mString may be non-NULL at a time
    CFMutableStringRef	mString;
    CFStringRef			string;

    CFMutableStringRef CFMutableStringFromCFString(CFStringRef inString) const
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
    /* empty QCString */
    QCString( )
    : string( NULL )
    , mString( NULL )
    { }

    /* Take ownership of a Mutable String */
    explicit QCString(CFMutableStringRef const &inString)
    : string( NULL )
    , mString( inString )
    { }

    /* Take ownership of an immutable String */
    explicit QCString(CFStringRef const &inString)
    : string( inString )
    , mString( NULL ) // maybe we'll never need it
    { }

    /* Convert HFSUniStr255 into an immutable String */
    explicit QCString(HFSUniStr255 const &inString)
    : string( CFStringFromHFSUniStr255(inString) )
    , mString( NULL )
    { }

    /* Convert C-style string into an immutable String */
    explicit QCString(char const * const inString)
    : string( CFStringFromCString(inString) )
    , mString( NULL )
    { }

    /* Convert C++ native string into an immutable String */
    explicit QCString(std::string const &inString)
    : string( CFStringFromCString(inString.c_str()) )
    , mString( NULL )
    { }

    // copy constructor
    QCString(QCString const &inString)
    : string( Retain(inString.string) )
    , mString( Retain(inString.mString) )
    { }

    // destructor
    ~QCString()
    {
        Release(string);
        Release(mString);
    }

    CFStringRef String() const
    {
        return isNotNull(mString) ? mString : string;
    }

    CFStringRef CFString() const
    {
        return isNotNull(string) ? string : mString;
    }

    void makeUniqueMutable() {
        if (NULL != mString) {
            CFMutableStringRef tmp = mString;
            mString = CFStringCreateMutableCopy(kCFAllocatorDefault, 0, mString);
            CFRelease(tmp);
        } else if (NULL != string) {
            mString = CFStringCreateMutableCopy(kCFAllocatorDefault, 0, string);
            CFRelease(string);
            string = NULL;
        } else {
            mString = CFStringCreateMutable(kCFAllocatorDefault, 0);
        }
    }

    bool null() const
    {
        return isNull(mString) && isNull(string);
    }

    CFIndex length() const
    {
        CFStringRef str = CFString();
        return isNull(str) ? 0 : CFStringGetLength(str);
    }

    // returns a deep copy
    QCString copy() const
    {
        CFStringRef str = CFString();
        return QCString(CFStringCreateMutableCopy(CFGetAllocator(str), 0, str));
    }

    UniChar at(CFIndex const idx) const
    {
        return CFStringGetCharacterAtIndex(CFString(), idx);
    }

    CFStringRef get() const {
        return NULL != mString ? mString : string;
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
        return kCFCompareEqualTo == CFStringCompare(CFString(), rhs.CFString(), 0);
    }

    bool operator != (QCString const &rhs) const
    {
        return !(*this == rhs);
    }

    bool operator < (QCString const & rhs) const {
        return kCFCompareLessThan == CFStringCompare(get(), rhs.get(), 0);
    }

    bool operator > (QCString const & rhs) const {
        return kCFCompareGreaterThan == CFStringCompare(get(), rhs.get(), 0);
    }

    // conversion operators

    operator CFStringRef () const
    {
        // can use a MutableString when you want a non-mutable, but not the reverse
        return CFString();
    }

    // concatenate operator
    QCString & operator += (QCString const &rhs)
    {
        makeUniqueMutable();
        //		return (*this += rhs.string); // would this work?

        if (isNull(mString))
        {
            // this case is possible only if string and mString were both NULL before calling makeUnique
            mString = CFStringCreateMutableCopy(kCFAllocatorDefault, 0, rhs.CFString());
        }
        else
        {
            CFStringAppend(mString, rhs.String());
        }
        return *this;
    }

    QCString & operator += (CFMutableStringRef const &rhs)
    {
        if (rhs != NULL)
        {
            makeUniqueMutable();
            if (isNull(mString))
            {
                mString = Retain(rhs);
            }
            else
            {
                CFStringAppend(mString, rhs);
            }
        }
        return *this;
    }

    QCString & operator += (CFStringRef rhs) {
        if (rhs != NULL) {
            makeUniqueMutable();
            CFStringAppend(mString, rhs);
        }
        return *this;
    }

    QCString & operator += (char const *rhs)
    {
        makeUniqueMutable();

        return (rhs == NULL) ? *this : *this += QCString(rhs);
    }

    bool hasPrefix(CFStringRef const prefix) const
    {
        return CFStringHasPrefix(CFString(), prefix) == true;
    }

    bool hasSuffix(CFStringRef const suffix) const
    {
        return CFStringHasSuffix(CFString(), suffix) == true;
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
        // TODO: optimize with CFStringGetCStringPtr
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
        if (isNull(CFString())
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
        CFStringRef temp = CFStringCreateWithSubstring(kCFAllocatorDefault, CFString(), range);

        return QCString(temp); // takes ownership & releases when done
    }

    QCString substring(CFRange range) const
    {
        return QCString( CFStringCreateWithSubstring(kCFAllocatorDefault, CFString(), range) );
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
        CFIndex location = (CFStringFind(CFString(), searchString, 0)).location + range.location;
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

template <class OStream>
OStream & operator << (OStream & os, QCString const &str)
{
    char const *c_str = CFStringGetCStringPtr(str.CFString(), kCFStringEncodingUTF8);
    if (c_str == NULL)
    {
        c_str = str.CString_new();
        if (c_str != NULL)
        {
            os << c_str;
            delete c_str;
        }

        return os;
    }
    else
    {
        // we got a pointer to String()'s internal c-string; no cleanup here.
        return os << c_str;
    }
}

typedef QCString const QCFixedString;

END_QC_NAMESPACE

#endif

