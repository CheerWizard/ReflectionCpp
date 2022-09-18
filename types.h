#pragma once

#include <vector>
#include <iostream>
#include <string>
#include <cstddef>
#include <sstream>

#include "template_helper.h"

namespace reflect {

    struct Type {
        const char* name;
        size_t size;

        Type(const char* name, size_t size) : name(name), size(size) {}
        virtual ~Type() = default;

        [[nodiscard]] virtual std::string getFullName() const { return name; }
        virtual void dump(const void* obj, int indentLevel = 0) const = 0;
    };

    struct Member {
        const char* name;
        size_t offset;
        Type* type;

        Member(const char* name, size_t offset, Type* type)
        : name(name), offset(offset), type(type) {}
    };

    //--------------------------------------------------------
    // Type descriptors for user-defined structs/classes
    //--------------------------------------------------------

    struct Struct : Type {

        std::vector<Member> members;

        Struct(void (*init)(Struct*)) : Type{nullptr, 0} {
            init(this);
        }

        Struct(const char* name, size_t size) : Type(name, size) {}

        Struct(const char* name, size_t size, const std::initializer_list<Member>& init)
                : Type(nullptr, 0), members(init) {}

        void dump(const void* obj, int indentLevel) const override {
            std::cout << name << " {" << std::endl;
            for (const Member& member : members) {
                std::cout << std::string(4 * (indentLevel + 1), ' ') << member.name << " = ";
                member.type->dump((char*) obj + member.offset, indentLevel + 1);
                std::cout << std::endl;
            }
            std::cout << std::string(4 * indentLevel, ' ') << "}";
        }
    };

    //--------------------------------------------------------
    // Finding type descriptors
    //--------------------------------------------------------

    has_member(Struct, typeDescriptor)

    template<typename T>
    Type* getPrimitive();

    struct DefaultResolver {

        template <class T>
        static Type* getImpl(std::true_type) {
            return &T::typeDescriptor;
        }

        template <class T>
        static Type* getImpl(std::false_type) {
            return getPrimitive<T>();
        }

        template<class T>
        static Type* get() {
            return getImpl<T>(typeDescriptor<T>{});
        }
    };

    // This is the primary class template for finding all TypeDescriptors:
    template <typename T>
    struct TypeResolver {
        static Type* get() {
            return DefaultResolver::get<T>();
        }
    };

    //--------------------------------------------------------
    // Type descriptors for std::vector
    //--------------------------------------------------------

    struct StdVector : Type {

        Type* itemType;
        size_t (*getSize)(const void*);
        const void* (*getItem)(const void*, size_t);

        template <typename ItemType>
        explicit StdVector(ItemType*)
        : Type("std::vector<>", sizeof(std::vector<ItemType>)), itemType(TypeResolver<ItemType>::get()) {

            getSize = [](const void* vecPtr) -> size_t {
                const std::vector<ItemType>& vec = *(const std::vector<ItemType>*) vecPtr;
                return vec.size();
            };

            getItem = [](const void* vecPtr, size_t index) -> const void* {
                const std::vector<ItemType>& vec = *(const std::vector<ItemType>*) vecPtr;
                return &vec[index];
            };

        }

        [[nodiscard]] std::string getFullName() const override {
            std::stringstream ss;
            ss << "std::vector<" << itemType->getFullName() << ">";
            return ss.str();
        }

        void dump(const void* obj, int indentLevel) const override {
            size_t numItems = getSize(obj);
            std::cout << getFullName();
            if (numItems == 0) {
                std::cout << "{}";
            } else {
                std::cout << "{" << std::endl;
                for (size_t index = 0; index < numItems; index++) {
                    std::cout << std::string(4 * (indentLevel + 1), ' ') << "[" << index << "] ";
                    itemType->dump(getItem(obj, index), indentLevel + 1);
                    std::cout << std::endl;
                }
                std::cout << std::string(4 * indentLevel, ' ') << "}";
            }
        }
    };

    // Partially specialize TypeResolver<> for std::vectors:
    template <typename T>
    class TypeResolver<std::vector<T>> {
    public:
        static Type* get() {
            static StdVector typeDesc { (T*) nullptr };
            return &typeDesc;
        }
    };

} // namespace reflect