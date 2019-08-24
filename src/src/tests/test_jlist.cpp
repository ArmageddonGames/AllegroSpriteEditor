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
#include "jinete/jlist.h"

static void test_append_and_clear()
{
  JList q;

  q = jlist_new();
  ASSERT(q != NULL);
  ASSERT(jlist_length(q) == 0);

  jlist_append(q, (void *)10);
  ASSERT(jlist_length(q) == 1);

  jlist_append(q, (void *)20);
  jlist_append(q, (void *)30);
  ASSERT(jlist_length(q) == 3);
  ASSERT(jlist_nth_data(q, 0) == (void *)10);
  ASSERT(jlist_nth_data(q, 1) == (void *)20);
  ASSERT(jlist_nth_data(q, 2) == (void *)30);

  jlist_clear(q);
  ASSERT(jlist_length(q) == 0);

  jlist_free(q);
}

static void test_prepend()
{
  JList q;

  q = jlist_new();
  jlist_prepend(q, (void *)30);
  jlist_prepend(q, (void *)20);
  jlist_prepend(q, (void *)10);
  ASSERT(jlist_length(q) == 3);
  ASSERT(jlist_nth_data(q, 0) == (void *)10);
  ASSERT(jlist_nth_data(q, 1) == (void *)20);
  ASSERT(jlist_nth_data(q, 2) == (void *)30);

  jlist_free(q);
}

static void test_insert()
{
  JList q;

  q = jlist_new();
  jlist_append(q, (void *)10);
  jlist_append(q, (void *)30);
  jlist_insert(q, (void *)20, 1);
  jlist_insert(q, (void *)50, 3);
  jlist_insert(q, (void *)40, 3);
  ASSERT(jlist_length(q) == 5);
  ASSERT(jlist_nth_data(q, 0) == (void *)10);
  ASSERT(jlist_nth_data(q, 1) == (void *)20);
  ASSERT(jlist_nth_data(q, 2) == (void *)30);
  ASSERT(jlist_nth_data(q, 3) == (void *)40);
  ASSERT(jlist_nth_data(q, 4) == (void *)50);

  jlist_free(q);
}

static void test_nth_link()
{
  JList q;
  JLink a, b, c;

  q = jlist_new();
  jlist_append(q, (void *)10);
  jlist_append(q, (void *)20);
  jlist_append(q, (void *)30);

  a = jlist_nth_link(q, 0);
  b = jlist_nth_link(q, 1);
  c = jlist_nth_link(q, 2);
  ASSERT(a->data == (void *)10);
  ASSERT(b->data == (void *)20);
  ASSERT(c->data == (void *)30);

  jlist_free(q);
}

static void test_insert_before()
{
  JList q;
  JLink a, b, c;

  q = jlist_new();
  jlist_append(q, (void *)20);
  jlist_append(q, (void *)40);
  jlist_append(q, (void *)60);

  a = jlist_nth_link(q, 0);
  b = jlist_nth_link(q, 1);
  c = jlist_nth_link(q, 2);

  jlist_insert_before(q, a, (void *)10);
  jlist_insert_before(q, b, (void *)30);
  jlist_insert_before(q, c, (void *)50);
  jlist_insert_before(q, NULL, (void *)70);
  ASSERT(jlist_length(q) == 7);
  ASSERT(jlist_nth_data(q, 0) == (void *)10);
  ASSERT(jlist_nth_data(q, 1) == (void *)20);
  ASSERT(jlist_nth_data(q, 2) == (void *)30);
  ASSERT(jlist_nth_data(q, 3) == (void *)40);
  ASSERT(jlist_nth_data(q, 4) == (void *)50);
  ASSERT(jlist_nth_data(q, 5) == (void *)60);
  ASSERT(jlist_nth_data(q, 6) == (void *)70);

  jlist_free(q);
}

static void test_remove_and_remove_all()
{
  JList q;

  q = jlist_new();
  jlist_append(q, (void *)10);
  jlist_append(q, (void *)20);
  jlist_append(q, (void *)30);

  jlist_remove(q, (void *)20);
  ASSERT(jlist_length(q) == 2);
  ASSERT(jlist_nth_data(q, 0) == (void *)10);
  ASSERT(jlist_nth_data(q, 1) == (void *)30);

  jlist_append(q, (void *)10);
  jlist_remove_all(q, (void *)10);
  ASSERT(jlist_length(q) == 1);
  ASSERT(jlist_nth_data(q, 0) == (void *)30);

  jlist_free(q);
}

static void test_remove_link_and_delete_link()
{
  JList q;
  JLink b;

  q = jlist_new();
  jlist_append(q, (void *)10);
  jlist_append(q, (void *)20);
  jlist_append(q, (void *)30);

  b = jlist_nth_link(q, 1);
  jlist_remove_link(q, b);
  ASSERT(jlist_length(q) == 2);

  jlist_delete_link(q, jlist_nth_link(q, 0));
  jlist_delete_link(q, jlist_nth_link(q, 0));
  ASSERT(jlist_length(q) == 0);

  jlink_free(b);
  jlist_free(q);
}

static void test_copy()
{
  JList q, r;

  q = jlist_new();
  jlist_append(q, (void *)10);
  jlist_append(q, (void *)20);
  jlist_append(q, (void *)30);
  ASSERT(jlist_length(q) == 3);
  ASSERT(jlist_nth_data(q, 0) == (void *)10);
  ASSERT(jlist_nth_data(q, 1) == (void *)20);
  ASSERT(jlist_nth_data(q, 2) == (void *)30);

  r = jlist_copy(q);
  ASSERT(jlist_length(r) == 3);
  ASSERT(jlist_nth_data(r, 0) == (void *)10);
  ASSERT(jlist_nth_data(r, 1) == (void *)20);
  ASSERT(jlist_nth_data(r, 2) == (void *)30);

  jlist_free(q);
  jlist_free(r);
}

static void test_find()
{
  JList q;

  q = jlist_new();
  jlist_append(q, (void *)10);
  jlist_append(q, (void *)20);
  jlist_append(q, (void *)30);

  ASSERT(jlist_find(q, (void *)10) == jlist_nth_link(q, 0));
  ASSERT(jlist_find(q, (void *)20) == jlist_nth_link(q, 1));
  ASSERT(jlist_find(q, (void *)30) == jlist_nth_link(q, 2));
}

int main(int argc, char *argv[])
{
  test_init();

  test_append_and_clear();
  test_prepend();
  test_insert();
  test_nth_link();
  test_insert_before();
  test_remove_and_remove_all();
  test_remove_link_and_delete_link();
  test_copy();
  test_find();

  return test_exit();
}

END_OF_MAIN();
