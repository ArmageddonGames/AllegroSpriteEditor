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

#include "jinete/jinete.h"

#include "app.h"
#include "commands/command.h"
#include "mem_utils.h"
#include "modules/gui.h"
#include "raster/cel.h"
#include "raster/image.h"
#include "raster/layer.h"
#include "raster/sprite.h"
#include "raster/stock.h"
#include "raster/undo.h"
#include "sprite_wrappers.h"

class CelPropertiesCommand : public Command
{
public:
  CelPropertiesCommand();
  Command* clone() const { return new CelPropertiesCommand(*this); }

protected:
  bool onEnabled(Context* context);
  void onExecute(Context* context);
};

CelPropertiesCommand::CelPropertiesCommand()
  : Command("cel_properties",
	    "Cel Properties",
	    CmdUIOnlyFlag)
{
}

bool CelPropertiesCommand::onEnabled(Context* context)
{
  const CurrentSpriteReader sprite(context);
  return
    sprite &&
    sprite->getCurrentLayer() &&
    sprite->getCurrentLayer()->is_image();
}

void CelPropertiesCommand::onExecute(Context* context)
{
  JWidget label_frame, label_pos, label_size;
  JWidget slider_opacity, button_ok;
  char buf[1024];
  int memsize;

  const CurrentSpriteReader sprite(context);
  const Layer* layer = sprite->getCurrentLayer();

  // Get current cel (can be NULL)
  const Cel* cel = static_cast<const LayerImage*>(layer)->get_cel(sprite->getCurrentFrame());

  FramePtr window(load_widget("cel_properties.xml", "cel_properties"));
  get_widgets(window,
	      "frame", &label_frame,
	      "pos", &label_pos,
	      "size", &label_size,
	      "opacity", &slider_opacity,
	      "ok", &button_ok, NULL);

  // Mini look for the opacity slider
  setup_mini_look(slider_opacity);

  /* if the layer isn't writable */
  if (!layer->is_writable()) {
    button_ok->setText(_("Locked"));
    button_ok->setEnabled(false);
  }

  usprintf(buf, "%d/%d", sprite->getCurrentFrame()+1, sprite->getTotalFrames());
  label_frame->setText(buf);

  if (cel != NULL) {
    /* position */
    usprintf(buf, "%d, %d", cel->x, cel->y);
    label_pos->setText(buf);

    /* dimension (and memory size) */
    memsize =
      image_line_size(sprite->getStock()->image[cel->image],
		      sprite->getStock()->image[cel->image]->w)*
      sprite->getStock()->image[cel->image]->h;

    usprintf(buf, "%dx%d (",
	     sprite->getStock()->image[cel->image]->w,
	     sprite->getStock()->image[cel->image]->h);
    get_pretty_memory_size(memsize,
			   buf+ustrsize(buf),
			   sizeof(buf)-ustrsize(buf));
    ustrcat(buf, ")");

    label_size->setText(buf);

    /* opacity */
    jslider_set_value(slider_opacity, cel->opacity);
    if (layer->is_background()) {
      slider_opacity->setEnabled(false);
      jwidget_add_tooltip_text(slider_opacity, "The `Background' layer is opaque,\n"
					       "you can't change its opacity.");
    }
  }
  else {
    label_pos->setText("None");
    label_size->setText("Empty (0 bytes)");
    jslider_set_value(slider_opacity, 0);
    slider_opacity->setEnabled(false);
  }

  window->open_window_fg();

  if (window->get_killer() == button_ok) {
    SpriteWriter sprite_writer(sprite);
    Layer* layer_writer = sprite_writer->getCurrentLayer();
    Cel* cel_writer = static_cast<LayerImage*>(layer_writer)->get_cel(sprite->getCurrentFrame());

    int new_opacity = jslider_get_value(slider_opacity);

    /* the opacity was changed? */
    if (cel_writer != NULL &&
	cel_writer->opacity != new_opacity) {
      if (undo_is_enabled(sprite_writer->getUndo())) {
	undo_set_label(sprite_writer->getUndo(), "Cel Opacity Change");
	undo_int(sprite_writer->getUndo(), (GfxObj *)cel_writer, &cel_writer->opacity);
      }

      /* change cel opacity */
      cel_set_opacity(cel_writer, new_opacity);

      update_screen_for_sprite(sprite);
    }
  }
}

//////////////////////////////////////////////////////////////////////
// CommandFactory

Command* CommandFactory::create_cel_properties_command()
{
  return new CelPropertiesCommand;
}
