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

#include "jinete/jwidget.h"

#include "app.h"
#include "commands/command.h"
#include "modules/gui.h"
#include "raster/layer.h"
#include "raster/sprite.h"
#include "raster/undo.h"
#include "undoable.h"
#include "sprite_wrappers.h"
#include "widgets/statebar.h"

//////////////////////////////////////////////////////////////////////
// remove_layer

class RemoveLayerCommand : public Command
{
public:
  RemoveLayerCommand();
  Command* clone() { return new RemoveLayerCommand(*this); }

protected:
  bool onEnabled(Context* context);
  void onExecute(Context* context);
};

RemoveLayerCommand::RemoveLayerCommand()
  : Command("remove_layer",
	    "Remove Layer",
	    CmdRecordableFlag)
{
}

bool RemoveLayerCommand::onEnabled(Context* context)
{
  const CurrentSpriteReader sprite(context);
  return
    sprite != NULL &&
    sprite->getCurrentLayer() != NULL;
}

void RemoveLayerCommand::onExecute(Context* context)
{
  std::string layer_name;
  CurrentSpriteWriter sprite(context);
  {
    Undoable undoable(sprite, "Remove Layer");

    layer_name = sprite->getCurrentLayer()->get_name();

    undoable.remove_layer(sprite->getCurrentLayer());
    undoable.commit();
  }
  update_screen_for_sprite(sprite);

  app_get_statusbar()->dirty();
  app_get_statusbar()
    ->showTip(1000, _("Layer `%s' removed"),
	      layer_name.c_str());
}

//////////////////////////////////////////////////////////////////////
// CommandFactory

Command* CommandFactory::create_remove_layer_command()
{
  return new RemoveLayerCommand;
}
