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

#include "commands/command.h"
#include "app.h"
#include "modules/gui.h"
#include "raster/sprite.h"
#include "raster/undo.h"
#include "widgets/statebar.h"
#include "sprite_wrappers.h"

class RedoCommand : public Command
{
public:
  RedoCommand();
  Command* clone() { return new RedoCommand(*this); }

protected:
  bool onEnabled(Context* context);
  void onExecute(Context* context);
};

RedoCommand::RedoCommand()
  : Command("redo",
	    "Redo",
	    CmdUIOnlyFlag)
{
}

bool RedoCommand::onEnabled(Context* context)
{
  const CurrentSpriteReader sprite(context);
  return
    sprite != NULL &&
    undo_can_redo(sprite->getUndo());
}

void RedoCommand::onExecute(Context* context)
{
  CurrentSpriteWriter sprite(context);

  app_get_statusbar()
    ->showTip(1000, _("Redid %s"),
	      undo_get_next_redo_label(sprite->getUndo()));

  undo_do_redo(sprite->getUndo());
  sprite->generateMaskBoundaries();
  update_screen_for_sprite(sprite);
}

//////////////////////////////////////////////////////////////////////
// CommandFactory

Command* CommandFactory::create_redo_command()
{
  return new RedoCommand;
}
