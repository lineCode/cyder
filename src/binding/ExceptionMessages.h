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

#ifndef CYDER_EXCEPTIONMESSAGES_H
#define CYDER_EXCEPTIONMESSAGES_H

#include "utils/NumberUtil.h"

namespace cyder {

    class ExceptionMessages {
    public:
        enum BoundType {
            InclusiveBound,
            ExclusiveBound,
        };

        static std::string ArgumentNullOrIncorrectType(int argumentIndex, const std::string& expectedType);
        static std::string ConstructorNotCallableAsFunction(const std::string& type);

        static std::string FailedToConvertJSValue(const std::string& type);

        static std::string FailedToConstruct(const std::string& type, const std::string& detail);
        static std::string FailedToEnumerate(const std::string& type, const std::string& detail);
        static std::string FailedToExecute(const std::string& method,
                                           const std::string& type,
                                           const std::string& detail);
        static std::string FailedToGet(const std::string& property,
                                       const std::string& type,
                                       const std::string& detail);
        static std::string FailedToSet(const std::string& property,
                                       const std::string& type,
                                       const std::string& detail);
        static std::string FailedToDelete(const std::string& property,
                                          const std::string& type,
                                          const std::string& detail);
        static std::string FailedToGetIndexed(const std::string& type, const std::string& detail);
        static std::string FailedToSetIndexed(const std::string& type, const std::string& detail);
        static std::string FailedToDeleteIndexed(const std::string& type, const std::string& detail);

        static std::string IncorrectPropertyType(const std::string& property, const std::string& detail);

        template<typename NumberType>
        static std::string IndexExceedsMaximumBound(const std::string& name,
                                                    NumberType given,
                                                    NumberType bound) {
            bool eq = given == bound;
            return "The " + name + " provided (" + NumberUtil::FiniteToString(given) + ") is greater than " +
                   (eq ? "or equal to " : "") + "the maximum bound (" + NumberUtil::FiniteToString(bound) + ").";
        }

        template<typename NumberType>
        static std::string IndexExceedsMinimumBound(const std::string& name, NumberType given, NumberType bound) {
            bool eq = given == bound;
            return "The " + name + " provided (" + NumberUtil::FiniteToString(given) + ") is less than " +
                   (eq ? "or equal to " : "") + "the minimum bound (" + NumberUtil::FiniteToString(bound) + ").";
        }

        template<typename NumberType>
        static std::string IndexOutsideRange(const std::string& name, NumberType given, NumberType lowerBound,
                                             BoundType lowerType, NumberType upperBound, BoundType upperType) {
            return "The " + name + " provided (" + NumberUtil::FiniteToString(given) + ") is outside the range " +
                   (lowerType == ExclusiveBound ? '(' : '[') + NumberUtil::FiniteToString(lowerBound) + ", " +
                   NumberUtil::FiniteToString(upperBound) + (upperType == ExclusiveBound ? ')' : ']') + '.';
        }

        static std::string InvalidArity(const std::string& expected, unsigned provided);
        static std::string NotASequenceTypeProperty(const std::string& property_name);
        static std::string NotAFiniteNumber(double value,
                                            const std::string& name = "value provided");
        static std::string NotEnoughArguments(unsigned expected, unsigned provided);
        static std::string ReadOnly(const std::string& detail = 0);

    private:
        static std::string OrdinalNumber(int number);
    };

}

#endif //CYDER_EXCEPTIONMESSAGES_H
