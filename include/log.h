//
// Created by Dhava on 17-11-2023.
//

#ifndef DEEX_LOG_H
#define DEEX_LOG_H

#include <format>
#include <iostream>
#include <glm/gtx/io.hpp>


template<typename... T>
void LOG(std::format_string<T...> fmt,T&&... args)
{
  std::cout<<std::format(fmt,std::forward<T>(args)...)<<std::endl;
}

template<typename... T>
void ASSERT(bool val,std::format_string<T...> fmt,const T&&... args)
{
    if(!val)
      std::cout<<std::format(fmt,args...)<<std::endl;
}

#endif //DEEX_LOG_H
