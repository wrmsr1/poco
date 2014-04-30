//
// ParseHandler.cpp
//
// $Id$
//
// Library: JSON
// Package: JSON
// Module:  ParseHandler
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
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


#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/Object.h"


using Poco::Dynamic::Var;


namespace Poco {
namespace JSON {


ParseHandler::ParseHandler(bool preserveObjectOrder) : Handler(),
	_preserveObjectOrder(preserveObjectOrder)
{
}


ParseHandler::~ParseHandler()
{
}


void ParseHandler::reset()
{
	while (!_stack.empty()) _stack.pop();
	_key = "";
	_result.empty();
}


void ParseHandler::startObject()
{
	Object::Ptr newObj = new Object(_preserveObjectOrder);

	if ( _stack.empty() ) // The first object
	{
		_result = newObj;
	}
	else
	{
		Var parent = _stack.top();

		if ( parent.type() == typeid(Array::Ptr) )
		{
			Array::Ptr arr = parent.extract<Array::Ptr>();
			arr->add(newObj);
		}
		else if ( parent.type() == typeid(Object::Ptr) )
		{
			poco_assert_dbg(!_key.empty());
			Object::Ptr obj = parent.extract<Object::Ptr>();
			obj->set(_key, newObj);
			_key.clear();
		}
	}

	_stack.push(newObj);
}


void ParseHandler::endObject()
{
	if (!_stack.empty()) _stack.pop();
}


void ParseHandler::startArray()
{
	Array::Ptr newArr = new Array();

	if ( _stack.empty() ) // The first array
	{
		_result = newArr;
	}
	else
	{
		Var parent = _stack.top();

		if ( parent.type() == typeid(Array::Ptr) )
		{
			Array::Ptr arr = parent.extract<Array::Ptr>();
			arr->add(newArr);
		}
		else if ( parent.type() == typeid(Object::Ptr) )
		{
			poco_assert_dbg(!_key.empty());
			Object::Ptr obj = parent.extract<Object::Ptr>();
			obj->set(_key, newArr);
			_key.clear();
		}
	}

	_stack.push(newArr);
}


void ParseHandler::endArray()
{
	if (!_stack.empty()) _stack.pop();
}


void ParseHandler::key(const std::string& k)
{
	_key = k;
}


void ParseHandler::setValue(const Var& value)
{
	Var parent = _stack.top();

	if ( parent.type() == typeid(Array::Ptr) )
	{
		Array::Ptr arr = parent.extract<Array::Ptr>();
		arr->add(value);
	}
	else if ( parent.type() == typeid(Object::Ptr) )
	{
		poco_assert_dbg(!_key.empty());
		Object::Ptr obj = parent.extract<Object::Ptr>();
		obj->set(_key, value);
		_key.clear();
	}
}


} } // namespace Poco::JSON
