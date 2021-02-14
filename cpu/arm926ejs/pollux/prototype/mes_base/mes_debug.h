//------------------------------------------------------------------------------
//
//	Copyright (C) 2007 MagicEyes Digital Co., Ltd All Rights Reserved
//	MagicEyes Digital Co. Proprietary & Confidential
//
//	MAGICEYES INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND	WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//	Module     : mes_base
//	File       : mes_debug.h
//	Description: support assert, trace routine
//	Author     : Goofy
//	Export     : 
//	History    : 
//		2007.04.04	Goofy First draft
//------------------------------------------------------------------------------
#ifndef __MES_DEBUG_H__
#define	__MES_DEBUG_H__

//------------------------------------------------------------------------------
//	Platform check
//------------------------------------------------------------------------------
// Microsoft 32bits Windows
#if defined(WIN32) && !defined(UNDER_CE)
	#ifdef	_MES_PLATFORM_
		#error "WIN32 and __arm and UNDER_CE predefine must use exclusively!!!"
	#endif

	#define _MES_PLATFORM_

	// Debug break definition, default is noexist
	#ifdef MES_DEBUG
		#define	MES_DEBUG_BREAK() 	{ __asm int 3 }
	#endif
#endif	// WIN32 && !UNDER_CE

//------------------------------------------------------------------------------
// ARM compiler
#ifdef __arm
	#ifdef	_MES_PLATFORM_
		#error "WIN32 and __arm and UNDER_CE predefine must use exclusively!!!"
	#endif

	#define _MES_PLATFORM_

	// Debug break definition, default is noexist
	#ifdef MES_DEBUG
	#define	MES_DEBUG_BREAK() 	{ volatile static int i = 1; while(i); }
	#endif
#endif	// __arm

//------------------------------------------------------------------------------
// Microsoft Window CE 
#ifdef UNDER_CE
	#ifdef	_MES_PLATFORM_
		#error "WIN32 and __arm and UNDER_CE predefine must use exclusively!!!"
	#endif

	#define _MES_PLATFORM_

	#include <windows.h>

	// Debug break definition, default is noexist
	#ifdef MES_DEBUG
		#define	MES_DEBUG_BREAK 	DebugBreak
	#endif
#endif	// UNDER_CE

//------------------------------------------------------------------------------
// LINUX / GNU C Compiler
#ifdef __GNUC__ 
	#ifdef	_MES_PLATFORM_
		#error "WIN32 and __arm and __GNUC__ predefine must use exclusively!!!"
	#endif

	#define _MES_PLATFORM_

	// Debug break definition, default is noexist
	#ifdef MES_DEBUG
		#define	MES_DEBUG_BREAK 	DebugBreak
	#endif
#endif	// LINUX / GNU C Compiler

#ifndef _MES_PLATFORM_
	#error "Unknown platform"
#endif

//------------------------------------------------------------------------------
//	compile mode check
//------------------------------------------------------------------------------
#ifdef MES_DEBUG
	#ifdef _MES_COMPILE_MODE_
		#error "MES_DEBUG and MES_RELEASE predefine must use exclusively!!!"
	#endif
	
	#define _MES_COMPILE_MODE_
#endif

#ifdef MES_RELEASE
	#ifdef _MES_COMPILE_MODE_
		#error "MES_DEBUG and MES_RELEASE predefine must use exclusively!!!"
	#endif
	
	#define _MES_COMPILE_MODE_
#endif

#ifndef _MES_COMPILE_MODE_
	#error "You must have pre-definition ( MES_DEBUG or MES_RELEASE )!!!"
#endif


//------------------------------------------------------------------------------
///	@name	Trace macro
/// @brief	MES_TRACE print formated string through user print function.
//------------------------------------------------------------------------------
/// @{
#ifdef MES_DEBUG
	#ifdef UNDER_CE
		#define	MES_TRACE(expr)		RETAILMSG( 1, expr )
	#else
		//----------------------------------------------------------------------
		///	@brief Extern function for print formated string
		/// You have to implement MES_DEBUG_PRINTF function for MES_TRACE.
		//----------------------------------------------------------------------
		#ifdef	__cplusplus
			extern "C" void MES_DEBUG_PRINTF(const char * const, ... );
		#else
			extern void MES_DEBUG_PRINTF(const char * const, ... );
		#endif
		
		#define MES_TRACE(expr)		MES_DEBUG_PRINTF expr
	#endif
#else
	#define MES_TRACE(expr)
#endif // MES_DEBUG
/// @}
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
///	@name 	Assert macro
/// @brief	Assert macro is composed by MES_ASSERT MES_CHECK MES_DEBUG_CODE.
//------------------------------------------------------------------------------
/// @{

#ifdef MES_DEBUG
	#ifndef MES_DEBUG_BREAK
		#error "there is no debug break !!!"
	#endif
#endif

#ifndef	TEXT
	#define TEXT
#endif

//------------------------------------------------------------------------------
/// MES_ASSERT
/// @brief  runtime assert in normal case
/// @code
///	int UnKnownFunction(int a, int b) {
/// 
///		int Result;
/// 	... 
/// 	
///		for(int i = 0; i < 10; i++) {
///			MES_ASSERT(i < MAXNUM_OF_NKNOWNARRY);
///			UnknownArray[i] = RandomNumberGen();
///		}
///		
///		... 
/// }
/// @endcode
// 
//------------------------------------------------------------------------------
#ifdef MES_DEBUG
	#define MES_ASSERT(expr)												\
	{                                                   					\
		if (!(expr))                                    					\
		{                                               					\
			MES_TRACE((TEXT("%s(%d) : %s (%s)\n"),							\
				TEXT(__FILE__), __LINE__, TEXT("ASSERT"), TEXT(#expr)));	\
			MES_DEBUG_BREAK();                          					\
		}                                               					\
	}
#else
	#define MES_ASSERT(expr)
#endif // MES_DEBUG


//------------------------------------------------------------------------------
/// MES_CHECK
/// @brief  Note that this assertion preserves the expression when disabled.	
/// 
/// @code
///		MES_CHECK((fp = fopen("file", "rb+")) != CNULL); // don't occur
///													     // compile time error
///														 // int MES_RELEASE
///		...
///		fclose(fp);
/// @endcode
//------------------------------------------------------------------------------
#ifdef MES_DEBUG
	#define MES_CHECK(expr)							                        \
	{                                                   					\
		if (!(expr))                                    					\
		{                                               					\
			MES_TRACE((TEXT("%s(%d) : %s (%s)\n"),							\
				TEXT(__FILE__), __LINE__, TEXT("CHECK"),TEXT(#expr))); 		\
			MES_DEBUG_BREAK();                          					\
		}                                               					\
	}
#else                          
	#define MES_CHECK(expr)								                    \
	{													                    \
		expr ;									                    		\
	}                                                                       
#endif // MES_DEBUG


//------------------------------------------------------------------------------
///	MES_DEBUG_CODE
/// @brief	for debug code or debug varible declare
///
/// @code
/// {
/// 	MES_DEBUG_CODE(int iDebugCounter = 0);
///		...
///		MES_DEBUG_CODE(iDebugCount++);
///		...
///		MES_DEBUG_CODE(
///			if(iDebugCount == x) 
///				MES_TRACE("... found!!!\n");
///		)
/// }
/// @endcode
//------------------------------------------------------------------------------
#ifdef MES_DEBUG
	#define MES_DEBUG_CODE(code) code
#else
	#define MES_DEBUG_CODE(code)
#endif // MES_DEBUG
/// @}
//------------------------------------------------------------------------------

#endif	//  __MES_DEBUG_H__

