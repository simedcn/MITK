//
// SAXException.h
//
// $Id$
//
// Library: XML
// Package: SAX
// Module:  SAX
//
// SAX exception classes.
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


#ifndef SAX_SAXException_INCLUDED
#define SAX_SAXException_INCLUDED


#include "Poco/XML/XML.h"
#include "Poco/XML/XMLException.h"
#include "Poco/XML/XMLString.h"


namespace Poco {
namespace XML {


POCO_DECLARE_EXCEPTION(XML_API, SAXException, XMLException)
	/// The base class for all SAX-related exceptions like SAXParseException,
	/// SAXNotRecognizedException or SAXNotSupportedException.
	/// 
	/// This class can contain basic error or warning information from either the XML parser 
	/// or the application: a parser writer or application writer can subclass it to provide 
	/// additional functionality. SAX handlers may throw this exception or any exception subclassed 
	/// from it.
	/// 
	/// If the application needs to pass through other types of exceptions, it must wrap those exceptions 
	/// in a SAXException or an exception derived from a SAXException.
	/// 
	/// If the parser or application needs to include information about a specific location in an XML 
	/// document, it should use the SAXParseException subclass.


POCO_DECLARE_EXCEPTION(XML_API, SAXNotRecognizedException, SAXException)
	/// Exception class for an unrecognized identifier.
	/// 
	/// An XMLReader will throw this exception when it finds an unrecognized feature or property 
	/// identifier; SAX applications and extensions may use this class for other, similar purposes.


POCO_DECLARE_EXCEPTION(XML_API, SAXNotSupportedException, SAXException)
	/// Exception class for an unsupported operation.
	/// 
	/// An XMLReader will throw this exception when it recognizes a feature or property identifier, 
	/// but cannot perform the requested operation (setting a state or value). Other SAX2 applications 
	/// and extensions may use this class for similar purposes.


class Locator;


class XML_API SAXParseException: public SAXException
	/// Encapsulate an XML parse error or warning.
	///
	/// This exception may include information for locating the error in the original XML document, 
	/// as if it came from a Locator object. Note that although the application will receive a 
	/// SAXParseException as the argument to the handlers in the ErrorHandler interface, the application 
	/// is not actually required to throw the exception; instead, it can simply read the information in it 
	/// and take a different action.
	/// 
	/// Since this exception is a subclass of SAXException, it inherits the ability to wrap another exception.
{
public:	
	SAXParseException(const std::string& msg, const Locator& loc);
		/// Create a new SAXParseException from a message and a Locator.

	SAXParseException(const std::string& msg, const Locator& loc, const Poco::Exception& exc);
		/// Wrap an existing exception in a SAXParseException.

	SAXParseException(const std::string& msg, const XMLString& publicId, const XMLString& systemId, int lineNumber, int columnNumber);
		/// Create a new SAXParseException with an embedded exception.
		/// 
		/// This constructor is most useful for parser writers.
		/// All parameters except the message are as if they were provided by a Locator. 
		/// For example, if the system identifier is a URL (including relative filename), 
		/// the caller must resolve it fully before creating the exception.

	SAXParseException(const std::string& msg, const XMLString& publicId, const XMLString& systemId, int lineNumber, int columnNumber, const Poco::Exception& exc);
		/// Create a new SAXParseException.
		/// 
		/// This constructor is most useful for parser writers.
		/// All parameters except the message are as if they were provided by a Locator. 
		/// For example, if the system identifier is a URL (including relative filename), 
		/// the caller must resolve it fully before creating the exception.

	SAXParseException(const SAXParseException& exc);
		/// Creates a new SAXParseException from another one.

	~SAXParseException() throw();
		/// Destroy the exception.

	SAXParseException& operator = (const SAXParseException& exc);
		/// Assignment operator.
		
	const char* name() const throw();
		/// Returns a static string describing the exception.

	const char* className() const throw();
		/// Returns the name of the exception class.

	Poco::Exception* clone() const;
		/// Creates an exact copy of the exception.
		
	void rethrow() const;
		/// (Re)Throws the exception.

	const XMLString& getPublicId() const;
		/// Get the public identifier of the entity where the exception occurred.
		
	const XMLString& getSystemId() const;
		/// Get the system identifier of the entity where the exception occurred.
		
	int getLineNumber() const;
		/// The line number of the end of the text where the exception occurred.
		/// The first line is line 1.
		
	int getColumnNumber() const;
		/// The column number of the end of the text where the exception occurred.
		/// The first column in a line is position 1.

protected:
	static std::string buildMessage(const std::string& msg, const XMLString& publicId, const XMLString& systemId, int lineNumber, int columnNumber);
	
private:
	SAXParseException();

	XMLString _publicId;
	XMLString _systemId;
	int       _lineNumber;
	int       _columnNumber;
};


//
// inlines
//
inline const XMLString& SAXParseException::getPublicId() const
{
	return _publicId;
}


inline const XMLString& SAXParseException::getSystemId() const
{
	return _systemId;
}


inline int SAXParseException::getLineNumber() const
{
	return _lineNumber;
}


inline int SAXParseException::getColumnNumber() const
{
	return _columnNumber;
}


} } // namespace Poco::XML


#endif // SAX_SAXException_INCLUDED
