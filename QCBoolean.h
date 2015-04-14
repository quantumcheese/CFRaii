/*
 *  QCBoolean.h
 *  CFRaii
 *
 * Copyright (c) 2009-2015 Richard A. Brown
 *
 * See license.txt for licensing terms and conditions.
 */

#ifndef _QC_BOOLEAN_GUARD_
#define _QC_BOOLEAN_GUARD_

#include <CoreFoundation/CoreFoundation.h>
#include <algorithm>
#include "CFRaiiCommon.h"

BEGIN_QC_NAMESPACE

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
	static QCBoolean const kQCBooleanTrue;
	static QCBoolean const kQCBooleanFalse;

	static QCBoolean QCBooleanFromBool(bool const b)
	{
		return b ? kQCBooleanTrue : kQCBooleanFalse;
	}

	static QCBoolean QCBooleanFromBool(Boolean const b)
	{
		return b ? kQCBooleanTrue : kQCBooleanFalse;
	}

	static QCBoolean QCBooleanFromBool(CFBooleanRef const b)
	{
		return QCBooleanFromBool(CFBooleanGetValue(b));
	}

	explicit QCBoolean(bool const inBool)
	: boolean( CFBooleanFromBool(inBool) )
	{ }

	explicit QCBoolean(Boolean const inBool)
	: boolean( CFBooleanFromBoolean(inBool) )
	{ }
	
	explicit QCBoolean(CFBooleanRef const inBool)
	: boolean( inBool )
	{ }
	
	// copy constructor
	QCBoolean(QCBoolean const &inBool)
	: boolean( inBool.boolean ) // because they are constant, there is no point to retaining / releasing
	{ }
	
	// destructor
	~QCBoolean( )
	{
		// do not release the constant values
	}
	
	// comparison operators
	
	bool operator == (QCBoolean const rhs) const
	{
		return boolean == rhs.boolean;
	}
	
	bool operator != (QCBoolean const rhs) const
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
	
	// Logic operators
	
	QCBoolean operator && (QCBoolean const &rhs)
	{
		return QCBoolean(
						 BoolFromCFBoolean() && rhs.BoolFromCFBoolean()
						 );
	}
	
	QCBoolean operator || (QCBoolean const &rhs)
	{
		return QCBoolean(
						 BoolFromCFBoolean() || rhs.BoolFromCFBoolean()
						 );
	}
	
	QCBoolean operator ! ()
	{
		return QCBoolean(
						 ! BoolFromCFBoolean()
						 );
	}
	
	void show() const;
};

typedef QCBoolean QCBool;

END_QC_NAMESPACE

#endif
