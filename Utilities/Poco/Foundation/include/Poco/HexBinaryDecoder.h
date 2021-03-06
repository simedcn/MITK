//
// HexBinaryDecoder.h
//
// $Id$
//
// Library: Foundation
// Package: Streams
// Module:  HexBinary
//
// Definition of the HexBinaryDecoder class.
//
// Copyright (c) 2004-2006, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_HexBinaryDecoder_INCLUDED
#define Foundation_HexBinaryDecoder_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/UnbufferedStreamBuf.h"
#include <istream>


namespace Poco {


class Foundation_API HexBinaryDecoderBuf: public UnbufferedStreamBuf
	/// This streambuf decodes all hexBinary-encoded data read
	/// from the istream connected to it.
	/// In hexBinary encoding, each binary octet is encoded as a character tuple,  
	/// consisting of two hexadecimal digits ([0-9a-fA-F]) representing the octet code.
	/// See also: XML Schema Part 2: Datatypes (http://www.w3.org/TR/xmlschema-2/),
	/// section 3.2.15.
{
public:
	HexBinaryDecoderBuf(std::istream& istr);
	~HexBinaryDecoderBuf();
	
private:
	int readFromDevice();
	int readOne();

	std::istream& _istr;
};


class Foundation_API HexBinaryDecoderIOS: public virtual std::ios
	/// The base class for HexBinaryDecoder.
	///
	/// This class is needed to ensure the correct initialization
	/// order of the stream buffer and base classes.
{
public:
	HexBinaryDecoderIOS(std::istream& istr);
	~HexBinaryDecoderIOS();
	HexBinaryDecoderBuf* rdbuf();

protected:
	HexBinaryDecoderBuf _buf;
};


class Foundation_API HexBinaryDecoder: public HexBinaryDecoderIOS, public std::istream
	/// This istream decodes all hexBinary-encoded data read
	/// from the istream connected to it.
	/// In hexBinary encoding, each binary octet is encoded as a character tuple,  
	/// consisting of two hexadecimal digits ([0-9a-fA-F]) representing the octet code.
	/// See also: XML Schema Part 2: Datatypes (http://www.w3.org/TR/xmlschema-2/),
	/// section 3.2.15.
{
public:
	HexBinaryDecoder(std::istream& istr);
	~HexBinaryDecoder();
};


} // namespace Poco


#endif // Foundation_HexBinaryDecoder_INCLUDED
