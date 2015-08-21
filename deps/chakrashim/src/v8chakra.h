// Copyright Microsoft. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and / or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.


// Internal header for the v8-chakra bridge

#pragma once
#include "v8.h"
#include "jsrtutils.h"
#include <assert.h>

namespace v8 {

struct ObjectTemplateData;

extern __declspec(thread) bool g_EnableDebug;

struct ObjectData {
 public:
  JsValueRef objectInstance;
  Persistent<ObjectTemplate> objectTemplate;  // Original ObjectTemplate
  NamedPropertyGetterCallback namedPropertyGetter;
  NamedPropertySetterCallback namedPropertySetter;
  NamedPropertyQueryCallback namedPropertyQuery;
  NamedPropertyDeleterCallback namedPropertyDeleter;
  NamedPropertyEnumeratorCallback namedPropertyEnumerator;
  Persistent<Value> namedPropertyInterceptorData;
  IndexedPropertyGetterCallback indexedPropertyGetter;
  IndexedPropertySetterCallback indexedPropertySetter;
  IndexedPropertyQueryCallback indexedPropertyQuery;
  IndexedPropertyDeleterCallback indexedPropertyDeleter;
  IndexedPropertyEnumeratorCallback indexedPropertyEnumerator;
  Persistent<Value> indexedPropertyInterceptorData;
  int internalFieldCount;
  void **internalFields;

  ObjectData(ObjectTemplate* objectTemplate, ObjectTemplateData *templateData);
};

struct TemplateData {
 protected:
  Persistent<Object> properties;
  virtual void CreateProperties();  // Allow properties to be created lazily

 public:
  Object* EnsureProperties();
};

namespace chakrashim {

class InternalMethods {
 public:
  static Handle<String> GetClassName(ObjectTemplate* objectTemplate);

  static JsValueRef CALLBACK ObjectPrototypeToStringShim(
    JsValueRef callee,
    bool isConstructCall,
    JsValueRef *arguments,
    unsigned short argumentCount,
    void *callbackState);

  static Handle<String> GetClassName(Object* obj) {
    return GetClassName(obj->GetObjectTemplate());
  }

  static bool IsInstanceOf(Object* obj, ObjectTemplate* objectTemplate) {
    return obj->GetObjectTemplate() == objectTemplate;
  }
};

bool CheckSignature(Local<FunctionTemplate> receiver, Local<Object> thisPointer,
                    Local<Object>* holder);

}  // namespace chakrashim

}  // namespace v8
