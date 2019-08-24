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

#include <allegro/unicode.h>

#include "jinete/jbase.h"

#include "commands/command.h"
#include "app.h"
#include "widgets/colbar.h"

class SwitchColorsCommand : public Command
{
public:
  SwitchColorsCommand();

protected:
  void onExecute(Context* context);
};

SwitchColorsCommand::SwitchColorsCommand()
  : Command("switch_colors",
	    "SwitchColors",
	    CmdUIOnlyFlag)
{
}

void SwitchColorsCommand::onExecute(Context* context)
{
  ColorBar* colorbar = app_get_colorbar();
  color_t fg = colorbar->getFgColor();
  color_t bg = colorbar->getBgColor();

  colorbar->setFgColor(bg);
  colorbar->setBgColor(fg);
}

//////////////////////////////////////////////////////////////////////
// CommandFactory

Command* CommandFactory::create_switch_colors_command()
{
  return new SwitchColorsCommand;
}
