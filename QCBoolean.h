/*
 *  QCBoolean.h
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 *
 * See license.txt for licensing terms and conditions.
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
