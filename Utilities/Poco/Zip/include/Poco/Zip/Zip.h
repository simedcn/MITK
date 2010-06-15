//
// Zip.h
//
// $Id$
//
// Library: Zip
// Package: Zip
// Module:  Zip
//
// Basic definitions for the Poco Zip library.
// This file must be the first file included by every other Zip
// header file.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//


#ifndef Zip_Zip_INCLUDED
#define Zip_Zip_INCLUDED


#include "Poco/Foundation.h"

//
// Ensure that POCO_DLL is default unless POCO_STATIC is defined
//
#if defined(_WIN32) && defined(_DLL)
#if !defined(POCO_DLL) && !defined(POCO_STATIC)
#define POCO_DLL
#endif
#endif

//
// The following block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the Zip_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// Zip_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
//
#if defined(_WIN32) && defined(POCO_DLL)
	#if defined(Zip_EXPORTS)
		#define Zip_API __declspec(dllexport)
	#else
		#define Zip_API __declspec(dllimport)
	#endif
#endif


#if !defined(Zip_API)
	#define Zip_API
#endif


//
// Automatically link Zip library.
//
#if defined(_MSC_VER)
	#if !defined(POCO_NO_AUTOMATIC_LIBS) && !defined(Zip_EXPORTS)
		#if defined(POCO_DLL)
			#if defined(_DEBUG)
				#pragma comment(lib, "PocoZipd.lib")
			#else
				#pragma comment(lib, "PocoZip.lib")
			#endif
		#else
			#if defined(_DEBUG)
				#pragma comment(lib, "PocoZipmtd.lib")
			#else
				#pragma comment(lib, "PocoZipmt.lib")
			#endif
		#endif
	#endif
#endif


#endif // Zip_Zip_INCLUDED
