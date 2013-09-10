/*
 *  QCMacrosInternal.h
 *  CFRaii
 *
 * Copyright (c) 2009-2012 Richard A. Brown
 *
 */

#ifndef _QC_MACROS_INTERNAL_
#define _QC_MACROS_INTERNAL_

#ifndef BEGIN_NAMESPACE
#define BEGIN_NAMESPACE(x) namespace x {
#endif
#ifndef END_NAMESPACE
#define END_NAMESPACE }
#endif


#if 1
	#define BEGIN_QC_NAMESPACE	BEGIN_NAMESPACE(QC)
	#define END_QC_NAMESPACE	END_NAMESPACE
#else
	#define BEGIN_QC_NAMESPACE
	#define END_QC_NAMESPACE
#endif


#if defined (__llvm__)
	#define DEPRECATED_DECLARATION(x)	__attribute__((deprecated(x)))
	#define WARNING_DECLARATION(x)		__attribute__((warning(x))
#elif defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
	#define DEPRECATED_DECLARATION(x)	__attribute__((deprecated))
	#define WARNING_DECLARATION(x)		// ???
#else
	#warn Unknown compiler
	#define DEPRECATED_DECLARATION(x)	// ???
	#define WARNING_DECLARATION(x)		// ???
#endif


#endif
