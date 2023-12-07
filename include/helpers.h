#pragma once
#include"log.h"
#include"glad/glad.h"

void checkGLError();

#ifndef DEEX_RELEASE
#define CHECK_GL_ERROR checkGLError();
#else
#define CHECK_GL_ERROR (void)0;
#endif