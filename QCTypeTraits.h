/*
 *  QCTypeTraits.h
 *  CFRaii
 *
 * Copyright (c) 2011 Richard A. Brown
 * 
 * See license.txt for licensing terms and conditions.
 */


#ifndef _QC_TYPE_TRAITS_GUARD_
#define _QC_TYPE_TRAITS_GUARD_

#include <tr1/type_traits>

#include "QCMacrosInternal.h"
#include "QCUtilities.h"

// MARK: -
// MARK: Is Core Foundation type trait

BEGIN_QC_NAMESPACE

namespace Detail // we would have liked to make this an anonymous namespace,
				 // but that's bad news in a header file.
{

	typedef std::tr1::false_type false_type;
	typedef std::tr1::true_type true_type;
	
	// MARK: internal template & specializations
	template <class T>
	struct _is_CFType : public false_type
	{ };
	
#define Is_CF_Type(Type) template <> \
struct _is_CFType <CF##Type##Ref> : public true_type \
{ }
	
	Is_CF_Type(Allocator);
	
	Is_CF_Type(Array);
	
	Is_CF_Type(AttributedString);
	
	Is_CF_Type(Bag);
	
	Is_CF_Type(BinaryHeap);
	
	Is_CF_Type(BitVector);
	
	Is_CF_Type(Boolean);
	
	Is_CF_Type(Bundle);
	
	Is_CF_Type(Calendar);
	
	Is_CF_Type(CharacterSet);
	
	Is_CF_Type(Data);
	
	Is_CF_Type(Date);
	
	Is_CF_Type(DateFormatter);
	
	Is_CF_Type(Dictionary);
	
	Is_CF_Type(Error);
	
	Is_CF_Type(FileDescriptor);
	
	Is_CF_Type(Locale);
	
	Is_CF_Type(MachPort);
	
	Is_CF_Type(MessagePort);
	
	Is_CF_Type(MutableArray);
	
	Is_CF_Type(MutableAttributedString);
	
	Is_CF_Type(MutableBag);
	
	Is_CF_Type(MutableBitVector);
	
	Is_CF_Type(MutableCharacterSet);
	
	Is_CF_Type(MutableData);
	
	Is_CF_Type(MutableDictionary);
	
	Is_CF_Type(MutableSet);
	
	Is_CF_Type(MutableString);
	
	Is_CF_Type(NotificationCenter);
	
	Is_CF_Type(Null);
	
	Is_CF_Type(Number);
	
	Is_CF_Type(NumberFormatter);
	
#if 0
	// CFPluginRef is a typedef to the same opaque type as CFBundleRef
	Is_CF_Type(PlugIn);
#endif
	
	Is_CF_Type(PlugInInstance);
	
#if 0
	// CFTypeRef and CFPropertyListRef are both typedefs for void const *
	Is_CF_Type(PropertyList);
#endif
	
	Is_CF_Type(ReadStream);
	
	Is_CF_Type(RunLoop);
	
	Is_CF_Type(RunLoopObserver);
	
	Is_CF_Type(RunLoopSource);
	
	Is_CF_Type(RunLoopTimer);
	
	Is_CF_Type(Set);
	
	Is_CF_Type(Socket);
	
	Is_CF_Type(String);
	
	Is_CF_Type(StringTokenizer);
	
	Is_CF_Type(TimeZone);
	
	Is_CF_Type(Tree);
	
	Is_CF_Type(Type);
	
	Is_CF_Type(URL);
	
	Is_CF_Type(UserNotification);
	
	Is_CF_Type(UUID);
	
	Is_CF_Type(WriteStream);
	
	Is_CF_Type(XMLNode);
	
	Is_CF_Type(XMLParser);
	
#if 0
	// CFXMLTreeRef is a typedef for CFTreeRef
	Is_CF_Type(XMLTree);
#endif
#undef Is_CF_Type
} /* Detail namespace */

// -- public-facing is_CFType<> interface
template <class T>
/* we need to remove_cv (from the pointer-to-opaque-CF-type)
 * in order to ensure matching the above template specialization
 */
struct is_CFType : public Detail::_is_CFType < typename std::tr1::remove_cv<T>::type >
{ };


// MARK: -
// MARK: CFType_traits

template < class CF, bool = is_CFType<CF>::value >
struct CFType_traits
{
	static bool const is_CFType = false;
};

// partial specialization for is_CFType
template < class CF >
struct CFType_traits < CF, true >
{
	typedef CF value_type;
	static bool const is_CFType = true;
	static value_type get(CF const &obj) { return obj; }
	static CFTypeID typeID(CF const &obj) { return CFGetTypeID(obj); }
	static CFAllocatorRef allocator(CF const &obj) { return CFGetAllocator(obj); }
	static CFIndex RetainCount(CF const &obj) { return CFGetRetainCount(obj); } // should never be called
};

END_QC_NAMESPACE

#endif
