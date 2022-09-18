//
// Created by mecha on 08.09.2022.
//

#pragma once

#define has_member(memberType, member) \
    template <typename T, typename = memberType>   \
    struct member : std::false_type{};\
                                          \
    template <typename T>                 \
    struct member<T, decltype(T::member)> : std::true_type {}; \
