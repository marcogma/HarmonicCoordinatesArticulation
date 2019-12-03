#include <igl/opengl/glfw/Viewer.h>
#include <iostream>
#include <ostream>
using namespace Eigen;

#define UNTYPED 0
#define BOUNDARY 1
#define INTERIOR 2
#define EXTERIOR 3

class Grid
{
  int size;
  Eigen::MatrixXi G;
  Eigen::MatrixXi Interior;
  Eigen::MatrixXf Harmonic_C;
  public:
    Grid(const int &s){
      size = s;
	  G = Eigen::MatrixXi::Zero(s, s);
    }

    void Tag_Cell(int x, int y, int value)
	{
		G(x, y) = value;
    }

	int get_CellTag(int x, int y)
	{
		return G(x,y);
	}

	void Flood_Fill(int x, int y, int OldTag, int NewTag)
	{
		if (x < 0 || x >= size || y < 0 || y >= size || OldTag==BOUNDARY)
			return;

		G(x,y) = NewTag;

		Flood_Fill( x + 1, y, G(x+1, y), NewTag);
		Flood_Fill( x - 1, y, G(x-1, y), NewTag);
		Flood_Fill( x, y + 1, G(x, y+1), NewTag);
		Flood_Fill( x, y - 1, G(x, y-1), NewTag);
		
	}

  };
