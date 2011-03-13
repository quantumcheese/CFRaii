/*
 *  QCBoolean.cpp
 *  CFRaii
 *
 * Copyright (c) 2009 Richard A. Brown
 *
 * See license.txt for licensing terms and conditions.
 */


#include "QCBoolean.h"

BEGIN_QC_NAMESPACE

void QCBoolean::show() const
{
#ifndef NDEBUG
	CFShow(boolean);
#endif
}

END_QC_NAMESPACE
