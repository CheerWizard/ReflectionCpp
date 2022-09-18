//
// Created by mecha on 08.09.2022.
//

#include "types.h"

namespace reflect {

    //--------------------------------------------------------
    // A type descriptor for int
    //--------------------------------------------------------

    struct Int : Type {
        Int() : Type("int", sizeof(int)) {}

        void dump(const void* obj, int) const override {
            std::cout << "int{" << *(const int*) obj << "}";
        }
    };

    template<>
    Type* getPrimitive<int>() {
        static Int typeDesc;
        return &typeDesc;
    }

    //--------------------------------------------------------
    // A type descriptor for std::string
    //--------------------------------------------------------

    struct StdString : Type {
        StdString() : Type("std::string", sizeof(std::string)) {}

        void dump(const void* obj, int) const override {
            std::cout << "std::string{\"" << *(const std::string*) obj << "\"}";
        }
    };

    template<>
    Type* getPrimitive<std::string>() {
        static StdString typeDesc;
        return &typeDesc;
    }

}