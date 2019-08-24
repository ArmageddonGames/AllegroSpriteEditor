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

#include "tests/test.h"

#include "jinete/jinete.h"
#include "core/file_system.h"

static void display_fileitem(FileItem *fi, int level, int deep)
{
  JList list;
  JLink link;
  int c;

  if (level == 0)
    printf("+ ");
  else
    printf("  ");

  for (c=0; c<level; ++c)
    printf("  ");

  printf("%s (%s)\n",
	 fileitem_get_filename(fi),
	 fileitem_get_displayname(fi));
  fflush(stdout);

  if (!fileitem_is_browsable(fi) || deep == 0)
    return;

  list = fileitem_get_children(fi);
  if (list) {
    JI_LIST_FOR_EACH(list, link) {
      display_fileitem(reinterpret_cast<FileItem*>(link->data), level+1, deep-1);
    }
  }
}

int main(int argc, char *argv[])
{
  test_init();
  ASSERT(file_system_init());

  trace("*** Listing root of the file-system (deep = 2)...\n");
  {
    FileItem *root = get_root_fileitem();
    ASSERT(root != NULL);
    display_fileitem(root, 0, 2);
  }

#ifdef ALLEGRO_WINDOWS
  trace("*** Testing specific directories retrieval...\n");
  {
    FileItem* c_drive,* c_drive2;
    FileItem* my_pc;

    trace("*** Getting 'C:\\' using 'get_fileitem_from_path'...\n");
    c_drive = get_fileitem_from_path("C:\\");
    ASSERT(c_drive != NULL);

    trace("*** Getting 'C:\\' again\n");
    c_drive2 = get_fileitem_from_path("C:\\");
    ASSERT(c_drive == c_drive2);

    trace("*** Displaying 'C:\\'...\n");
    display_fileitem(c_drive, 0, 0);

    trace("*** Getting 'My PC' (using 'fileitem_get_parent')...\n");
    my_pc = fileitem_get_parent(c_drive);
    ASSERT(my_pc != NULL);

    trace("*** Listing 'My PC'...\n");
    display_fileitem(my_pc, 0, 1);
  }
#endif

  trace("*** Testing 'filename_has_extension'...\n");
  {
    ASSERT(filename_has_extension("hi.png", "png"));
    ASSERT(!filename_has_extension("hi.png", "pngg"));
    ASSERT(!filename_has_extension("hi.png", "ppng"));
    ASSERT(filename_has_extension("hi.jpeg", "jpg,jpeg"));
    ASSERT(filename_has_extension("hi.jpg", "jpg,jpeg"));
    ASSERT(!filename_has_extension("hi.ase", "jpg,jpeg"));
    ASSERT(filename_has_extension("hi.ase", "jpg,jpeg,ase"));
    ASSERT(filename_has_extension("hi.ase", "ase,jpg,jpeg"));
  }

  file_system_exit();
  return test_exit();
}

END_OF_MAIN();
