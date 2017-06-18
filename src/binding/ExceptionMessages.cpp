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

#include "ExceptionMessages.h"

namespace cyder {
    std::string ExceptionMessages::FailedToConvertJSValue(const std::string& type) {
        return "Failed to convert value to '" + type + "'.";
    }

    std::string ExceptionMessages::FailedToConstruct(const std::string& type, const std::string& detail) {
        return "Failed to construct '" + type + (detail.length() > 0 ? "': " + detail : "'");
    }

    std::string ExceptionMessages::FailedToEnumerate(const std::string& type,
                                                     const std::string& detail) {
        return "Failed to enumerate the properties of '" + type +
               (detail.length() > 0 ? "': " + detail : "'");
    }

    std::string ExceptionMessages::FailedToExecute(const std::string& method,
                                                   const std::string& type,
                                                   const std::string& detail) {
        return "Failed to execute '" + method + "' on '" + type +
               (detail.length() > 0 ? "': " + detail : "'");
    }

    std::string ExceptionMessages::FailedToGet(const std::string& property,
                                               const std::string& type,
                                               const std::string& detail) {
        return "Failed to read the '" + property + "' property from '" +
               type + "': " + detail;
    }

    std::string ExceptionMessages::FailedToSet(const std::string& property,
                                               const std::string& type,
                                               const std::string& detail) {
        return "Failed to set the '" + property + "' property on '" +
               type + "': " + detail;
    }

    std::string ExceptionMessages::FailedToDelete(const std::string& property,
                                                  const std::string& type,
                                                  const std::string& detail) {
        return "Failed to delete the '" + property + "' property from '" +
               type + "': " + detail;
    }

    std::string ExceptionMessages::FailedToGetIndexed(const std::string& type,
                                                      const std::string& detail) {
        return "Failed to read an indexed property from '" + type + "': " +
               detail;
    }

    std::string ExceptionMessages::FailedToSetIndexed(const std::string& type,
                                                      const std::string& detail) {
        return "Failed to set an indexed property on '" + type + "': " +
               detail;
    }

    std::string ExceptionMessages::FailedToDeleteIndexed(const std::string& type,
                                                         const std::string& detail) {
        return "Failed to delete an indexed property from '" + type + "': " +
               detail;
    }

    std::string ExceptionMessages::ConstructorNotCallableAsFunction(const std::string& type) {
        return FailedToConstruct(type,
                                 "Please use the 'new' operator, this object "
                                         "constructor cannot be called as a function.");
    }

    std::string ExceptionMessages::IncorrectPropertyType(const std::string& property,
                                                         const std::string& detail) {
        return "The '" + property + "' property " + detail;
    }

    std::string ExceptionMessages::InvalidArity(const std::string& expected,
                                                int provided) {
        return "Valid arities are: " + expected + ", but " +
               NumberUtil::FiniteToString(provided) + " arguments provided.";
    }

    std::string ExceptionMessages::ArgumentNullOrIncorrectType(
            int argument_index,
            const std::string& expected_type) {
        return "The " + OrdinalNumber(argument_index) +
               " argument provided is either null, or an invalid " + expected_type +
               " object.";
    }

    std::string ExceptionMessages::NotASequenceTypeProperty(
            const std::string& property_name) {
        return "'" + property_name +
               "' property is neither an array, nor does it have indexed properties.";
    }

    std::string ExceptionMessages::NotEnoughArguments(int expected,
                                                      int provided) {
        return NumberUtil::FiniteToString(expected) + " argument" + (expected > 1 ? "s" : "") +
               " required, but only " + NumberUtil::FiniteToString(provided) + " present.";
    }

    std::string ExceptionMessages::NotAFiniteNumber(double value, const std::string& name) {
        return "The " + name + " is " + (std::isinf(value) ? "infinite" : "not a number") + ".";
    }

    std::string ExceptionMessages::OrdinalNumber(int number) {
        std::string suffix = "th";
        switch (number % 10) {
            case 1:
                if (number % 100 != 11)
                    suffix = "st";
                break;
            case 2:
                if (number % 100 != 12)
                    suffix = "nd";
                break;
            case 3:
                if (number % 100 != 13)
                    suffix = "rd";
                break;
            default:
                break;
        }
        return NumberUtil::FiniteToString(number) + suffix;
    }

    std::string ExceptionMessages::ReadOnly(const std::string& detail) {
        static std::string readOnly = "This object is read-only.";
        return detail.length() > 0 ? "This object is read-only, because " + detail + "." : readOnly;
    }
}