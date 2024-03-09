//
// Created by Dhava on 17-11-2023.
//

#ifndef DEEX_LOG_H
#define DEEX_LOG_H

#include <csignal>
#include <cstdlib>
#include <format>
#include <glm/gtx/io.hpp>
#include <iostream>

template <typename... T>
void LOG(std::format_string<T...> fmt, T &&...args)
{
    std::cout << std::format(fmt, std::forward<T>(args)...) << std::endl;
}

template <typename... T>
void ASSERT(bool val, std::format_string<T...> fmt, const T &&...args)
{
    if (!val)
    {
        std::cout << std::format(fmt, args...) << std::endl;
        std::raise(SIGTRAP);
    }
}

#endif // DEEX_LOG_H
