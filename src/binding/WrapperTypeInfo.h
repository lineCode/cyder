//////////////////////////////////////////////////////////////////////////////////////
//
//  The MIT License (MIT)
//
//  Copyright (c) 2017-present, cyder.org
//  All rights reserved.
//
//  Permission is hereby granted, free of charge, to any person obtaining a copy of
//  this software and associated documentation files (the "Software"), to deal in the
//  Software without restriction, including without limitation the rights to use, copy,
//  modify, merge, publish, distribute, sublicense, and/or sell copies of the Software,
//  and to permit persons to whom the Software is furnished to do so, subject to the
//  following conditions:
//
//      The above copyright notice and this permission notice shall be included in all
//      copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED *AS IS*, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
//  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
//  PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
//  HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
//  OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
//////////////////////////////////////////////////////////////////////////////////////

#ifndef CYDER_WRAPPERTYPEINFO_H
#define CYDER_WRAPPERTYPEINFO_H

#include <functional>
#include <v8.h>

namespace cyder {
    /**
     * Bitflags to show where the member will be defined.
     */
    enum InstallLocation : unsigned {
        InstallOnInstance = 1 << 0,
        InstallOnPrototype = 1 << 1,
        InstallOnClass = 1 << 2
    };

    /**
     * AccessorConfiguration translates into calls to SetAccessorProperty() on prototype ObjectTemplate.
     */
    struct AccessorConfiguration {
        AccessorConfiguration& operator=(const AccessorConfiguration&) = delete;
        const char* name;
        v8::FunctionCallback getter;
        v8::FunctionCallback setter;
        unsigned attribute; // v8::PropertyAttribute
        unsigned location;
    };

    /**
     * MethodConfiguration translates into calls to Set() for setting up an object's callbacks. It sets the method
     * on both the FunctionTemplate or the ObjectTemplate.
     */
    struct MethodConfiguration {
        MethodConfiguration& operator=(const MethodConfiguration&) = delete;
        const char* name;
        v8::FunctionCallback callback;
        int length; // the minimum number of callback function's parameters
        unsigned attribute; // v8::PropertyAttribute
        unsigned location;
    };

    typedef std::function<v8::Local<v8::Value>(v8::Isolate*)> DelayedScriptValue;
    DelayedScriptValue ConstantValue(double value);
    DelayedScriptValue ConstantValue(int value);
    DelayedScriptValue ConstantValue(const char* value);

    /**
     * ConstantConfiguration translates into calls to Set() for setting up an object's constants. It sets the
     * constant on both the FunctionTemplate and the ObjectTemplate. PropertyAttributes is always ReadOnly.
     */
    struct ConstantConfiguration {
        ConstantConfiguration& operator=(const ConstantConfiguration&) = delete;
        const char* name;
        DelayedScriptValue value;
    };

    /**
     * AttributeConfiguration translates into calls to SetLazyDataProperty() on the prototype ObjectTemplate
     */
    struct LazyAttributeConfiguration {
        LazyAttributeConfiguration& operator=(const LazyAttributeConfiguration&) = delete;
        const char* name;
        v8::AccessorNameGetterCallback getter;
        v8::AccessorNameSetterCallback setter;
        unsigned attribute; // v8::PropertyAttribute
    };

    struct WrapperTypeInfo {
        const WrapperTypeInfo* parentClass;
        const char* className;
        const v8::FunctionCallback constructor;
        const int constructorLength; // the minimum number of constructor's parameters
        const AccessorConfiguration* accessors;
        const int accessorCount;
        const MethodConfiguration* methods;
        const int methodCount;
        const ConstantConfiguration* constants;
        const int constantCount;
        const LazyAttributeConfiguration* lazyAttributes;
        const int lazyAttributeCount;
    };
}

#endif //CYDER_WRAPPERTYPEINFO_H
