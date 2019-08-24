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

#include <allegro.h>

#include "jinete/jinete.h"

#include "commands/command.h"
#include "core/core.h"
#include "modules/gui.h"

//////////////////////////////////////////////////////////////////////
// about

class AboutCommand : public Command
{
public:
  AboutCommand();
  Command* clone() const { return new AboutCommand(*this); }

protected:
  void onExecute(Context* context);
};

AboutCommand::AboutCommand()
  : Command("about",
	    "About",
	    CmdUIOnlyFlag)
{
}

void AboutCommand::onExecute(Context* context)
{
  FramePtr frame(new Frame(false, _("About " PACKAGE)));
  Widget* box1 = jbox_new(JI_VERTICAL);
  Widget* grid = jgrid_new(2, false);
  Label* title = new Label(PACKAGE " | Allegro Sprite Editor v" VERSION);
  Label* subtitle = new Label(_("A pixel art program"));
  Widget* authors_separator1 = ji_separator_new("Authors:", JI_HORIZONTAL | JI_TOP);
  Widget* authors_separator2 = ji_separator_new(NULL, JI_HORIZONTAL);
  Label* author1 = new LinkLabel("http://www.davidcapello.com.ar/", "David Capello");
  Label* author1_desc = new Label("| Programming");
  Label* author2 = new LinkLabel("http://ilkke.blogspot.com/", "Ilija Melentijevic");
  Label* author2_desc = new Label("| Skin and Graphics");
  Label* author3 = new Label("Trent Gamblin");
  Label* author3_desc = new Label("| MAC OS X builds");
  Widget* bottom_box1 = jbox_new(JI_HORIZONTAL);
  Widget* bottom_box2 = jbox_new(JI_HORIZONTAL);
  Widget* bottom_box3 = jbox_new(JI_HORIZONTAL);
  Label* copyright = new Label(COPYRIGHT);
  Label* website = new LinkLabel(WEBSITE);
  Widget* close_button = jbutton_new(_("&Close"));

  jgrid_add_child(grid, title, 2, 1, 0);
  jgrid_add_child(grid, subtitle, 2, 1, 0);
  jgrid_add_child(grid, authors_separator1, 2, 1, 0);
  jgrid_add_child(grid, author1, 1, 1, 0);
  jgrid_add_child(grid, author1_desc, 1, 1, 0);
  jgrid_add_child(grid, author2, 1, 1, 0);
  jgrid_add_child(grid, author2_desc, 1, 1, 0);
  jgrid_add_child(grid, author3, 1, 1, 0);
  jgrid_add_child(grid, author3_desc, 1, 1, 0);
  jgrid_add_child(grid, authors_separator2, 2, 1, 0);
  jgrid_add_child(grid, copyright, 2, 1, 0);
  jgrid_add_child(grid, website, 2, 1, 0);
  jgrid_add_child(grid, bottom_box1, 2, 1, 0);
  
  jwidget_magnetic(close_button, true);

  jwidget_expansive(bottom_box2, true);
  jwidget_expansive(bottom_box3, true);

  jwidget_add_children(bottom_box1, bottom_box2, close_button, bottom_box3, NULL);
  jwidget_add_child(box1, grid);
  jwidget_add_child(frame, box1);

  jwidget_set_border(close_button,
  		     close_button->border_width.l + 16*jguiscale(),
  		     close_button->border_width.t,
  		     close_button->border_width.r + 16*jguiscale(),
  		     close_button->border_width.b);

  frame->open_window_fg();
}

//////////////////////////////////////////////////////////////////////
// CommandFactory

Command* CommandFactory::create_about_command()
{
  return new AboutCommand;
}
