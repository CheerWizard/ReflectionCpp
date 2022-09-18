//
// Created by mecha on 08.09.2022.
//

#pragma once

#include "types.h"

#define R_BEGIN(type) \
friend struct reflect::DefaultResolver; \
static reflect::Struct typeDescriptor; \
\
static void initType(reflect::Struct* typeDesc) { \
    using T = type;   \
    typeDesc->name = #type;             \
    typeDesc->size = sizeof(T);         \
    typeDesc->members = {};             \

#define R_PROPERTY(name) \
    typeDesc->members.emplace_back(#name, offsetof(T, name), reflect::TypeResolver<decltype(T::name)>::get()); \

#define R_END } \

#define R_STRUCT(type, body) struct type { body }; \
reflect::Struct type::typeDescriptor = { type::initType }; \

#define R_STRUCT_EXT(type, parent, body) struct type : parent { \
body                                                            \
}; \
reflect::Struct type::typeDescriptor = { type::initType }; \
