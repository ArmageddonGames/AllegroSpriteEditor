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

class IntertwineNone : public ToolIntertwine
{
public:

  void joinPoints(IToolLoop* loop, const std::vector<Point>& points)
  {
    for (size_t c=0; c<points.size(); ++c)
      doPointshapePoint(points[c].x, points[c].y, loop);
  }

  void fillPoints(IToolLoop* loop, const std::vector<Point>& points)
  {
    joinPoints(loop, points);
  }
};

class IntertwineAsLines : public ToolIntertwine
{
public:
  bool snapByAngle() { return true; }

  void joinPoints(IToolLoop* loop, const std::vector<Point>& points)
  {
    if (points.size() == 0)
      return;

    if (points.size() == 1) {
      doPointshapePoint(points[0].x, points[0].y, loop);
    }
    else if (points.size() >= 2) {
      for (size_t c=0; c+1<points.size(); c+=2) {
	int x1 = points[c].x;
	int y1 = points[c].y;
	int x2 = points[c+1].x;
	int y2 = points[c+1].y;

	algo_line(x1, y1, x2, y2, loop, (AlgoPixel)doPointshapePoint);
      }
    }

    // Closed shape (polygon outline)
    if (loop->getFilled()) {
      algo_line(points[0].x, points[0].y,
		points[points.size()-1].x,
		points[points.size()-1].y, loop, (AlgoPixel)doPointshapePoint);
    }
  }

  void fillPoints(IToolLoop* loop, const std::vector<Point>& points)
  {
    if (points.size() < 3) {
      joinPoints(loop, points);
      return;
    }

    // Contour
    joinPoints(loop, points);

    // Fill content
    algo_polygon(points.size(), (const int*)&points[0], loop, (AlgoHLine)doPointshapeHline);
  }
};

class IntertwineAsRectangles : public ToolIntertwine
{
public:

  void joinPoints(IToolLoop* loop, const std::vector<Point>& points)
  {
    if (points.size() == 0)
      return;

    if (points.size() == 1) {
      doPointshapePoint(points[0].x, points[0].y, loop);
    }
    else if (points.size() >= 2) {
      for (size_t c=0; c+1<points.size(); c+=2) {
	int x1 = points[c].x;
	int y1 = points[c].y;
	int x2 = points[c+1].x;
	int y2 = points[c+1].y;
	int y;

	if (x1 > x2) std::swap(x1, x2);
	if (y1 > y2) std::swap(y1, y2);

	doPointshapeLine(x1, y1, x2, y1, loop);
	doPointshapeLine(x1, y2, x2, y2, loop);

	for (y=y1; y<=y2; y++) {
	  doPointshapePoint(x1, y, loop);
	  doPointshapePoint(x2, y, loop);
	}
      }
    }
  }

  void fillPoints(IToolLoop* loop, const std::vector<Point>& points)
  {
    if (points.size() < 2) {
      joinPoints(loop, points);
      return;
    }

    for (size_t c=0; c+1<points.size(); c+=2) {
      int x1 = points[c].x;
      int y1 = points[c].y;
      int x2 = points[c+1].x;
      int y2 = points[c+1].y;
      int y;

      if (x1 > x2) std::swap(x1, x2);
      if (y1 > y2) std::swap(y1, y2);

      for (y=y1; y<=y2; y++)
	doPointshapeLine(x1, y, x2, y, loop);
    }
  }
};

class IntertwineAsEllipses : public ToolIntertwine
{
public:

  void joinPoints(IToolLoop* loop, const std::vector<Point>& points)
  {
    if (points.size() == 0)
      return;

    if (points.size() == 1) {
      doPointshapePoint(points[0].x, points[0].y, loop);
    }
    else if (points.size() >= 2) {
      for (size_t c=0; c+1<points.size(); c+=2) {
	int x1 = points[c].x;
	int y1 = points[c].y;
	int x2 = points[c+1].x;
	int y2 = points[c+1].y;

	if (x1 > x2) std::swap(x1, x2);
	if (y1 > y2) std::swap(y1, y2);

	algo_ellipse(x1, y1, x2, y2, loop, (AlgoPixel)doPointshapePoint);
      }
    }
  }

  void fillPoints(IToolLoop* loop, const std::vector<Point>& points)
  {
    if (points.size() < 2) {
      joinPoints(loop, points);
      return;
    }

    for (size_t c=0; c+1<points.size(); c+=2) {
      int x1 = points[c].x;
      int y1 = points[c].y;
      int x2 = points[c+1].x;
      int y2 = points[c+1].y;

      if (x1 > x2) std::swap(x1, x2);
      if (y1 > y2) std::swap(y1, y2);

      algo_ellipsefill(x1, y1, x2, y2, loop, (AlgoHLine)doPointshapeHline);
    }
  }
};

class IntertwineAsBezier : public ToolIntertwine
{
public:

  void joinPoints(IToolLoop* loop, const std::vector<Point>& points)
  {
    if (points.size() == 0)
      return;

    for (size_t c=0; c<points.size(); c += 4) {
      if (points.size()-c == 1) {
	doPointshapePoint(points[c].x, points[c].y, loop);
      }
      else if (points.size()-c == 2) {
	algo_line(points[c].x, points[c].y,
		  points[c+1].x, points[c+1].y, loop, (AlgoPixel)doPointshapePoint);
      }
      else if (points.size()-c == 3) {
	algo_spline(points[c  ].x, points[c  ].y,
		    points[c+1].x, points[c+1].y,
		    points[c+1].x, points[c+1].y,
		    points[c+2].x, points[c+2].y, loop, (AlgoLine)doPointshapeLine);
      }
      else {
	algo_spline(points[c  ].x, points[c  ].y,
		    points[c+1].x, points[c+1].y,
		    points[c+2].x, points[c+2].y,
		    points[c+3].x, points[c+3].y, loop, (AlgoLine)doPointshapeLine);
      }
    }
  }

  void fillPoints(IToolLoop* loop, const std::vector<Point>& points)
  {
    joinPoints(loop, points);
  }
};
