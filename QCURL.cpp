/*
 *  QCURL.cpp
 *  CFRaii
 *
 * Copyright (c) 2009-2010 Richard A. Brown
 *
 * See license.txt for licensing terms and conditions.
 */

#include "QCURL.h"

BEGIN_QC_NAMESPACE

void QCURL::show() const
{
#ifndef NDEBUG
	CFShow(url);
#endif
}

END_QC_NAMESPACE