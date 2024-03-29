/*
 -------------------------------------------------------------------------------
 This file is part of the Plink, Plonk, Plank libraries
  by Martin Robinson
 
 http://code.google.com/p/pl-nk/
 
 Copyright University of the West of England, Bristol 2011-14
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright
   notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
   notice, this list of conditions and the following disclaimer in the
   documentation and/or other materials provided with the distribution.
 * Neither the name of University of the West of England, Bristol nor 
   the names of its contributors may be used to endorse or promote products
   derived from this software without specific prior written permission.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE 
 DISCLAIMED. IN NO EVENT SHALL UNIVERSITY OF THE WEST OF ENGLAND, BRISTOL BE 
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE 
 GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) 
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT 
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 
 This software makes use of third party libraries. For more information see:
 doc/license.txt included in the distribution.
 -------------------------------------------------------------------------------
 */

#ifndef PLONK_STANDARDHEADER_H
#define PLONK_STANDARDHEADER_H

#define PLONK_PRESTRING2(a) #a
#define PLONK_PRESTRING(a) PLONK_PRESTRING2(a)

// from now on: odd build numbers are development versions, even are releases
#define PLONK_MAJOR_VERSION      0
#define PLONK_MINOR_VERSION      5
#define PLONK_BUILDNUMBER        9
#define PLONK_VERSION           "v" PLONK_PRESTRING(PLONK_MAJOR_VERSION) "." PLONK_PRESTRING(PLONK_MINOR_VERSION) "." PLONK_PRESTRING(PLONK_BUILDNUMBER)

#include "../../plank/plank.h"

#define PLONK_EMPTYDEFINE

#define PLONK_BIGENDIAN     PLANK_BIGENDIAN
#define PLONK_LITTLEENDIAN  PLANK_LITTLEENDIAN

#if PLANK_WIN
    #define PLONK_WIN 1
#endif

#if PLANK_APPLE
    #define PLONK_APPLE 1
#endif

#if PLANK_MAC
    #define PLONK_MAC 1
#endif

#if PLANK_IOS
    #define PLONK_IOS 1
#endif

#if PLANK_LINUX
    #define PLONK_LINUX 1
#endif

#if PLANK_ANDROID
    #define PLONK_ANDROID 1
#endif

#if PLANK_GCC
    #define PLONK_GCC 1
#endif

#if PLANK_LLVM
    #define PLONK_LLVM 1
#endif

#if PLANK_APPLE_LLVM
    #define PLONK_APPLE_LLVM 1
#endif

#if PLANK_ARM
    #define PLONK_ARM 1
#endif

#if PLANK_X86
    #define PLONK_X86 1
#endif

#if PLANK_PPC
    #define PLONK_PPC 1
#endif

#if PLANK_32BIT
    #define PLONK_32BIT 1
    #define PLONK_WORDBITS 32
    #define PLONK_HALFWORDBITS 16
    #define PLONK_WORDSIZE 4
	#define PLONK_WIDESIZE 8
#endif

#if PLANK_64BIT
    #define PLONK_64BIT 1
    #define PLONK_WORDBITS 64
    #define PLONK_HALFWORDBITS 32
    #define PLONK_WORDSIZE 8
	#define PLONK_WIDESIZE 16
#endif

#define PLONK_ALIGN(X) PLANK_ALIGN(X)

#ifdef PLONK_USEPLINK
    #include "../../plink/plink.h"
#endif

#define PLONK_NAMESPACE plonk

#if defined(PLONK_NAMESPACE) || DOXYGEN
    #define BEGIN_PLONK_NAMESPACE    namespace PLONK_NAMESPACE {
    #define END_PLONK_NAMESPACE      }
#else
    #define BEGIN_PLONK_NAMESPACE 
    #define END_PLONK_NAMESPACE   
#endif

#if !defined(_NDEBUG) && !defined(NDEBUG) && !defined(_DEBUG) && !defined(DEBUG)
    #warning You should define appropriate preprocessor macros in Debug (_DEBUG=1 DEBUG=1) and Release (_NDEBUG=1 NDEBUG=1)
#endif

#ifndef PLONK_DEBUG
    #if !defined(NDEBUG) || defined(_DEBUG)
        #define PLONK_DEBUG 1
    #endif
#else
    #if !PLONK_DEBUG
        #undef PLONK_DEBUG
    #endif
#endif

#define PLONK_INLINE_LOW  PLANK_INLINE_LOW
#define PLONK_INLINE_MID  PLANK_INLINE_MID
#define PLONK_INLINE_HIGH PLANK_INLINE_HIGH


/* If you get a compiler error here you need to make sure you're using a C++
 compiler. If you're using Objective C then use Objective C++ by using .mm files
 instead of .m as an extension. */
#include <cstdlib>

#include <stdarg.h>
#include <climits>
#include <cfloat>
#include <cmath>
#include <cwchar>
#include <typeinfo>
#include <cstring>
#include <cstdio>

#if !PLONK_ANDROID
#include <iostream>
#endif

#ifdef PLONK_DEBUG
    #include <cassert>
    #define plonk_assert        assert
	#define plonk_assertfalse	(assert (false))
#else
	#define plonk_assert(x)     { }
	#define plonk_assertfalse	{ }
#endif


/* if you're getting your first compile error here on iOS
 it's probably because the file your compiling which includes
 plonk headers is not a C++ file. To make an Obj-C file (.m) 
 an Obj-C++ file use .mm as the file extension. */
BEGIN_PLONK_NAMESPACE
template <bool x> struct PlonkStaticAssertUtility;
template <> struct PlonkStaticAssertUtility<true> { static PLONK_INLINE_LOW void plonkStaticAssertionFailure() {} };

#ifdef PLONK_DEBUG
    #define plonk_staticassert(x) plonk::PlonkStaticAssertUtility<x>::plonkStaticAssertionFailure();
#else
    #define plonk_staticassert(x) { }
#endif

typedef PlankResult ResultCode;

typedef PlankF Float;
typedef PlankD Double;

typedef PlankC Char;
typedef PlankI Int; 
typedef PlankS Short;
typedef PlankL Long;
typedef PlankLL LongLong;

typedef PlankUC UnsignedChar;
typedef PlankUI UnsignedInt; 
typedef PlankUS UnsignedShort;
typedef PlankUL UnsignedLong;
typedef PlankULL UnsignedLongLong;

typedef bool Bool;
typedef PlankP Pointer;
typedef PlankM Method;
typedef PlankFourCharCode FourCharCode;

struct VoidReturn { enum Type { Pass = 1 }; };

//#if defined (__clang__) && defined (__has_feature)
//    #if !__has_feature (cxx_noexcept)
//        #define PLONK_USING_NEWCXXFEATURES 1
//    #endif
//#endif
//
//#if PLONK_USING_NEWCXXFEATURES
//    #define noexcept throw()
//    const                           // this is a const object...
//    class {
//    public:
//        template<class T>           // convertible to any type
//        operator T*() const         // of null non-member
//        { return 0; }               // pointer...
//        template<class C, class T>  // or any type of null
//        operator T C::*() const     // member pointer...
//        { return 0; }
//    private:
//        void operator&() const;     // whose address can't be taken
//    } nullptr = {};                 // and whose name is nullptr
//#endif

#define PLONK_INT24_MAX PLANK_INT24_MAX

#if PLONK_WIN
	#pragma warning(disable : 4244) // loss of precision
	#pragma warning(disable : 4996) // security of snprintf etc
	#pragma warning(disable : 4800) // bool performance warning
	#pragma warning(disable : 4290) // exceptions re: std::bad_alloc
	#define _CRT_SECURE_NO_WARNINGS
#endif


/** To help raise compiler errors when function prototypes change.
 This can be used as the function return type in place of the original
 type to force the compiler to identify places in the source tree where the
 function (normally a virtual) changed in the base class but derived classes 
 are still using the old version (this may otherwise cause strange behaviour). */
struct ForceErrorStruct { int dummy; };
#define FORCE_COMPILER_ERROR_FUNCTION_PROTOTYPE_CHANGED ForceErrorStruct


#if PLONK_USEPLINK
    template<class SampleType = float>
    struct PlinkBuffer
    {
        int bufferSize;
        const SampleType* buffer;
    };

    template<int N, class SampleType = float>
    struct PlinkProcess
    {        
        PlinkProcessBase base;
        PlinkBuffer<SampleType> buffers[N];
        
        static void init (PlinkProcess* pp, void* userData, int numOutputs, int numInputs)
        {
            plonk_assert (N >= (numInputs + numOutputs));
            
            pp->base.userData   = userData;
            pp->base.numOutputs = numOutputs;
            pp->base.numInputs  = numInputs;
            pp->base.numBuffers = N;        
        }
    };
#endif

#include "plonk_Memory.h"

#define PLONK_UNARYOPFUNCTION(TYPE,NAME)        TYPE (*NAME)(TYPE const&)
#define PLONK_BINARYOPFUNCTION(TYPE,NAME)       TYPE (*NAME)(TYPE const&, TYPE const&)
#define PLONK_BINARYOPFUNCTION_TYPE(TYPE)       TYPE (TYPE const&, TYPE const&)
#define PLONK_TERNARYOPFUNCTION(TYPE,NAME)      TYPE (*NAME)(TYPE const&, TYPE const&, TYPE const&)
#define PLONK_QUATERNARYOPFUNCTION(TYPE,NAME)   TYPE (*NAME)(TYPE const&, TYPE const&, TYPE const&, TYPE const&)
#define PLONK_QUINARYOPFUNCTION(TYPE,NAME)      TYPE (*NAME)(TYPE const&, TYPE const&, TYPE const&, TYPE const&, TYPE const&)

template<class TypeA, class TypeB>
class BinaryOpTypeUtility
{
public:
    typedef double CalcType;
};

#define PLONK_BINARYOPTYPEUTILITY_DEFINE(TYPEA,TYPEB,CALCTYPE)\
    template<> class BinaryOpTypeUtility<TYPEA,TYPEB>\
    {\
    public:\
        typedef CALCTYPE CalcType;\
    }

PLONK_BINARYOPTYPEUTILITY_DEFINE(float,float,float);
PLONK_BINARYOPTYPEUTILITY_DEFINE(float,int,float);
PLONK_BINARYOPTYPEUTILITY_DEFINE(int,float,float);
PLONK_BINARYOPTYPEUTILITY_DEFINE(float,short,float);
PLONK_BINARYOPTYPEUTILITY_DEFINE(short,float,float);
PLONK_BINARYOPTYPEUTILITY_DEFINE(float,char,float);
PLONK_BINARYOPTYPEUTILITY_DEFINE(char,float,float);
PLONK_BINARYOPTYPEUTILITY_DEFINE(float,LongLong,float);
PLONK_BINARYOPTYPEUTILITY_DEFINE(LongLong,float,float);

PLONK_BINARYOPTYPEUTILITY_DEFINE(double,double,double);
PLONK_BINARYOPTYPEUTILITY_DEFINE(double,int,double);
PLONK_BINARYOPTYPEUTILITY_DEFINE(int,double,double);
PLONK_BINARYOPTYPEUTILITY_DEFINE(double,short,double);
PLONK_BINARYOPTYPEUTILITY_DEFINE(short,double,double);
PLONK_BINARYOPTYPEUTILITY_DEFINE(double,char,double);
PLONK_BINARYOPTYPEUTILITY_DEFINE(char,double,double);
PLONK_BINARYOPTYPEUTILITY_DEFINE(double,LongLong,double);
PLONK_BINARYOPTYPEUTILITY_DEFINE(LongLong,double,double);

PLONK_BINARYOPTYPEUTILITY_DEFINE(char,char,short);
PLONK_BINARYOPTYPEUTILITY_DEFINE(char,short,int);
PLONK_BINARYOPTYPEUTILITY_DEFINE(short,char,int);
PLONK_BINARYOPTYPEUTILITY_DEFINE(char,int,LongLong);
PLONK_BINARYOPTYPEUTILITY_DEFINE(int,char,LongLong);

PLONK_BINARYOPTYPEUTILITY_DEFINE(short,short,int);
PLONK_BINARYOPTYPEUTILITY_DEFINE(short,int,LongLong);
PLONK_BINARYOPTYPEUTILITY_DEFINE(int,short,LongLong);

PLONK_BINARYOPTYPEUTILITY_DEFINE(int,int,LongLong);
PLONK_BINARYOPTYPEUTILITY_DEFINE(int,LongLong,LongLong);
PLONK_BINARYOPTYPEUTILITY_DEFINE(LongLong,int,LongLong);



// getting the calc type for ternary ops (or two chained binary ops)
template<class TypeA, class TypeB, class TypeC>
class TernaryOpTypeUtility
{
public:
    typedef typename BinaryOpTypeUtility<TypeA,TypeB>::CalcType TypeD;
    typedef typename BinaryOpTypeUtility<TypeD,TypeC>::CalcType CalcType;
};

// getting the calc type for quaternary ops (or three chained binary ops)
template<class TypeA, class TypeB, class TypeC, class TypeD>
class QuaternaryOpTypeUtility
{
public:
    typedef typename BinaryOpTypeUtility<TypeA,TypeB>::CalcType TypeE;
    typedef typename BinaryOpTypeUtility<TypeE,TypeC>::CalcType TypeF;
    typedef typename BinaryOpTypeUtility<TypeF,TypeD>::CalcType CalcType;
};

// getting the calc type for quinary ops (or four chained binary ops)
template<class TypeA, class TypeB, class TypeC, class TypeD, class TypeE>
class QuinaryOpTypeUtility
{
public:
    typedef typename BinaryOpTypeUtility<TypeA,TypeB>::CalcType TypeF;
    typedef typename BinaryOpTypeUtility<TypeF,TypeC>::CalcType TypeG;
    typedef typename BinaryOpTypeUtility<TypeG,TypeD>::CalcType TypeH;
    typedef typename BinaryOpTypeUtility<TypeH,TypeE>::CalcType CalcType;
};



#define PLONK_INFINITY PLANK_INFINITY

//PLONK_INLINE_LOW int quantiseUp(const int a, const int q) 
//{ 
//	if(a % q)
//		return a / q * q + q;
//	else
//		return a / q * q;
//}
//
//PLONK_INLINE_LOW int quantiseDown(const int a, const int q) 
//{ 
//	return a / q * q; 
//}

//#ifndef numElementsInArray
//	#define numElementsInArray(a)   ((int) (sizeof (a) / sizeof ((a)[0])))
//#endif


#ifndef PLONK_ALIGN
    #define PLONK_ALIGN PLANK_ALIGN
#endif

#ifndef PLONK_TYPE_DEFAULT
    #define PLONK_TYPE_DEFAULT Float
#endif

typedef PLONK_TYPE_DEFAULT TypeDefault;

#ifndef LONG_LONG_MAX
	#define LONG_LONG_MAX LLONG_MAX
#endif

// unreference a variable in an inner scope to help prevent accidental usage
struct ShadowType { };

#define PLONK_SHADOW(VAR) ShadowType* VAR; (void)VAR;

#if PLONK_APPLE && defined(__OBJC__)

/** Convenience macro for adding member variables to Objective-C classes.
 This helps declare the appropriate type of Plonk Variable<> e.g., for
 providing an interface between a GUI and the audio engine. You then need to
 declare the property using PLONK_OBJC_PROPERTY_DECLARE and synthesise the
 getter and setter methods using PLONK_OBJC_PROPERTY_SYNTH.
 
 For example:
 
 @code
 @interface MyClass : NSObject
 {
 PLONK_OBJC_PROPERTY_MEMBER(float,frequency);
 PLONK_OBJC_PROPERTY_MEMBER(float,amplitude);
 }
 ...
 @endcode
 
 @see PLONK_OBJC_PROPERTY_DECLARE, PLONK_OBJC_PROPERTY_SYNTH */
#define PLONK_OBJC_PROPERTY_MEMBER(Type,Name)\
    Variable<Type> Name

/** Convenience macro for declaring Objective-C properites.
 This helps make the appropriate property declaration for a Plonk
 member variable declared using PLONK_OBJC_PROPERTY_MEMBER.
 
 For example:
 
 @code
 @interface MyClass : NSObject
 {
 PLONK_OBJC_PROPERTY_MEMBER(float,frequency);
 PLONK_OBJC_PROPERTY_MEMBER(float,amplitude);
 }
 PLONK_OBJC_PROPERTY_DECLARE(float,frequency); // instead of @property ..etc
 PLONK_OBJC_PROPERTY_DECLARE(float,amplitude); // instead of @property ..etc
 @end
 @endcode
 
 @see PLONK_OBJC_PROPERTY_MEMBER, PLONK_OBJC_PROPERTY_SYNTH */
#define PLONK_OBJC_PROPERTY_DECLARE(Type,Name)\
    @property (nonatomic,getter=get##Name,setter=set##Name:) Type Name

/** Convenience macro for synthesising properity methods.
 This helps synthesise the getter and setter methods for properties declared
 using PLONK_OBJC_PROPERTY_MEMBER and PLONK_OBJC_PROPERTY_DECLARE.
 
 For example:
 
 @code
 @implementation AudioHost
 
 PLONK_OBJC_PROPERTY_SYNTH (float,frequency); // instead of @synthesize frequency;
 PLONK_OBJC_PROPERTY_SYNTH (float,amplitude); // instead of @synthesize amplitude;
 ...
 @end
 @endcode
 
 @see PLONK_OBJC_PROPERTY_MEMBER, PLONK_OBJC_PROPERTY_DECLARE */
#define PLONK_OBJC_PROPERTY_SYNTH(Type,Name)\
    - (Type)get##Name {\
    return Name.getValue();\
    }\
    - (void)set##Name:(Type)value {\
        Name.setValue (value);\
    }


#define PLONK_OBJC_PROPERTY_MEMBER_UNIT(Type,Name)\
    Variable< UnitBase<Type>& > Name

#define PLONK_OBJC_PROPERTY_DECLARE_UNIT(Type,Name)\
    @property (nonatomic,getter=get##Name,setter=set##Name:) UnitBase<Type> Name

#define PLONK_OBJC_PROPERTY_SYNTH_UNIT(Type,Name)\
    - (UnitBase<Type>)get##Name {\
    return Name.getValue();\
    }\
    - (void)set##Name:(UnitBase<Type>)value {\
        Name.setValue (value);\
    }


#endif // PLONK_APPLE && __OBJC__


END_PLONK_NAMESPACE


#endif // PLONK_STANDARDHEADER_H