/*
 *  QCBoolean.h
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

#ifndef _QC_BOOLEAN_GUARD_
#define _QC_BOOLEAN_GUARD_

#include <CoreFoundation/CoreFoundation.h>
#include <algorithm>
#include "CFRaiiCommon.h"

class QCBoolean
{
private:
	CFBooleanRef boolean;
	
	CFBooleanRef CFBooleanFromBool(bool const inBool)
	{
		return inBool ? kCFBooleanTrue : kCFBooleanFalse;
	}
	
	CFBooleanRef CFBooleanFromBoolean(Boolean const inBool)
	{
		return inBool ? kCFBooleanTrue : kCFBooleanFalse;
	}
	
	Boolean BooleanFromCFBoolean() const
	{
		return CFBooleanGetValue(boolean);
	}
	
	bool BoolFromCFBoolean() const
	{
		return CFBooleanGetValue(boolean) == true; // convert from Boolean to bool
	}
	
public:
	QCBoolean(bool const inBool)
	: boolean( CFBooleanFromBool(inBool) )
	{ }
	
	QCBoolean(Boolean const inBool)
	: boolean( CFBooleanFromBoolean(inBool) )
	{ }
	
	QCBoolean(CFBooleanRef const & inBool = kCFBooleanFalse)
	: boolean( inBool )
	{ }
	
	// copy constructor
	QCBoolean(QCBoolean const &inBool)
	: boolean( inBool.boolean ) // because they are constant
	{ }
	
	// destructor
	~QCBoolean( )
	{
		// do not release the constant values
	}
	
	// comparison operators
	bool operator == (CFBooleanRef const rhs) const
	{
		// since the CFBoolean values are two constants, we can compare them
		return boolean == rhs;
	}
	
	bool operator == (QCBoolean const rhs) const
	{
		return boolean == rhs.boolean;
	}
	
	bool operator == (bool const rhs) const
	{
		return BoolFromCFBoolean() == rhs;
	}
	
	bool operator != (CFBooleanRef const rhs) const
	{
		return !(*this == rhs);
	}
	
	bool operator != (QCBoolean const rhs) const
	{
		return !(*this == rhs);
	}
	
	bool operator != (bool const rhs) const
	{
		return !(*this == rhs);
	}
	
	// conversion operators
	
	operator Boolean () const
	{
		return BooleanFromCFBoolean();
	}
	
	operator bool () const
	{
		return BoolFromCFBoolean();
	}
	
	operator CFBooleanRef () const
	{
		return boolean;
	}
	
	void show() const;
};

typedef QCBoolean QCBool;

#endif
