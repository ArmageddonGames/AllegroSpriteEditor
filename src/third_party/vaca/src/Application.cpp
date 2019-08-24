// Vaca - Visual Application Components Abstraction
// Copyright (c) 2005-2009 David Capello
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// * Redistributions of source code must retain the above copyright
//   notice, this list of conditions and the following disclaimer.
// * Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in
//   the documentation and/or other materials provided with the
//   distribution.
// * Neither the name of the author nor the names of its contributors
//   may be used to endorse or promote products derived from this
//   software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
// OF THE POSSIBILITY OF SUCH DAMAGE.

#include "Vaca/Application.h"
#include <vector>
#include <cassert>

#ifdef VACA_ON_WINDOWS
#include <windows.h>
#endif

using namespace Vaca;

Application* Application::m_instance = NULL;
std::vector<String> Application::m_args;

Application::Application()
{
#ifdef VACA_ON_WINDOWS
  CoInitialize(NULL);
#endif
}

Application::~Application()
{
}

size_t Application::getArgc()
{
  return m_args.size();
}

const String& Application::getArgv(size_t i)
{
  return m_args[i];
}

/**
   Returns the parameters in the command line.

   @c Application::getArgs()[0] is the name of the executable file.
*/
const std::vector<String>& Application::getArgs()
{
  return m_args;
}

void Application::setArgs(const std::vector<String>& args)
{
  m_args = args;
}

/**
   Returns the Application's singleton.

   A program using Vaca must have one instance of Applicaton or a
   class derived from it.
*/
Application* Application::getInstance()
{
  assert(m_instance != NULL);
  return m_instance;
}
