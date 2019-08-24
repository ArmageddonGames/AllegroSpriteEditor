/* ASE - Allegro Sprite Editor
 * Copyright (C) 2001-2010  David Capello
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "config.h"

/* #include <stdio.h> */
#include <string.h>
#include <allegro/unicode.h>
#include <exception>

#include "jinete/jinete.h"

#include "console.h"
#include "commands/command.h"
#include "commands/commands.h"

#undef FOR_EACH_COMMAND
  #define FOR_EACH_COMMAND(name) \
    const char* CommandId::name = #name;
  #include "commands/commands_list.h"
#undef FOR_EACH_COMMAND

CommandsModule* CommandsModule::m_instance = NULL;

CommandsModule::CommandsModule()
{
  ASSERT(m_instance == NULL);
  m_instance = this;

  #undef FOR_EACH_COMMAND
  #define FOR_EACH_COMMAND(name) \
    m_commands.push_back(CommandFactory::create_##name##_command());

  #include "commands/commands_list.h"
  #undef FOR_EACH_COMMAND
}

CommandsModule::~CommandsModule()
{
  ASSERT(m_instance == this);

  for (CommandsList::iterator
	 it = m_commands.begin(); it != m_commands.end(); ++it) {
    delete *it;
  }

  m_commands.clear();
  m_instance = NULL;
}

CommandsModule* CommandsModule::instance()
{
  ASSERT(m_instance != NULL);
  return m_instance;
}

Command* CommandsModule::get_command_by_name(const char* name)
{
  if (!name)
    return NULL;

  for (CommandsList::iterator
	 it = m_commands.begin(); it != m_commands.end(); ++it) {
    if (ustricmp((*it)->short_name(), name) == 0)
      return *it;
  }

  return NULL;
}
