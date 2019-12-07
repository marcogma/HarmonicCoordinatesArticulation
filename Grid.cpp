#include <igl/opengl/glfw/Viewer.h>
#include <iostream>
#include <ostream>
#include <cmath>
#include <math.h>


using namespace Eigen;

#define UNTYPED 0
#define BOUNDARY 1
#define INTERIOR 2
#define EXTERIOR 3

const float xMin = -10;
const float yMin = -10;
const float xMax = 10;
const float yMax = 10;

class Grid
{

  private:
  	int size;
  	float step; 
  	Eigen::MatrixXi G;  //Grid size*size. G(x,y) represents a Region: UNTYPED,BOUNDARY,INTERIOR,EXTERIOR
  	Eigen::MatrixXi Coarse_G;  //Grid size-1*size-1. G(x,y) represents coarse version of G, following the "pulling" rules
  	MatrixXi InteriorV;
  	std::vector<MatrixXf> Harmonics;
	


  public:

    Grid(const int &s){
      size = std::pow(2,s);
	  step = (xMax - xMin) / (size - 1);
	  G = Eigen::MatrixXi::Zero(size, size);
    }

	void Add_Cage(MatrixXd cage)
	{
		for (int i = 0; i < cage.rows(); i++)
		{
			int x0, y0, x1, y1;
			x0 = (int)(cage.row(i)(0) - xMin) / step;
			y0 = (int)(cage.row(i)(1) - yMin) / step;
			if (i < cage.rows() - 1)
			{
				x1 = (int) (cage.row(i + 1)(0) - xMin) / step;
				y1 = (int) (cage.row(i + 1)(1) - yMin) / step;
			}
			else
			{
				x1 = (int) (cage.row(0)(0) - xMin) / step;
				y1 = (int) (cage.row(0)(1) - yMin) / step;
			}
			BresenhamsAlgorithm(x0,y0,x1,y1,i);
		}

		
	}

	//Traverse the grid and changes the OldTag to a NewTag until a StopTag Appears 
	void Flood_Fill(int x, int y)
	{
		if (x < 0 || x > size - 1 || y < 0 || y > size - 1)
			return;

		if (G(x, y) == BOUNDARY || G(x,y) == EXTERIOR)
			return;

		G(x,y) = EXTERIOR;

		Flood_Fill( x + 1, y);
		Flood_Fill( x - 1, y);
		Flood_Fill( x, y + 1);
		Flood_Fill( x, y - 1);
		
	}
	


	void BresenhamsAlgorithm(int x0, int y0, int x1, int y1, int i)
	{
		int dx = abs(x1 - x0);
		int sx = x0 < x1 ? 1 : -1;
		int dy = abs(y1 - y0);
		int sy = y0 < y1 ? 1 : -1;
		int err = (dx > dy ? dx : -dy) / 2;
		int e2;
		while (1)
		{
			G(x0, y0) = BOUNDARY;
			if (x0 == x1 && y0 == y1) break;
			e2 = err;
			if (e2 > -dx)
			{
				err -= dy;
				x0 += sx;
			}
			if (e2 < dy) 
			{
				err += dx;
				y0 += sy;
			}
		}
	}



	//Fill the grid starting with exterior region and then the interior
	void Fill_Grid_Regions()
	{
		Flood_Fill(0, 0);
		std::cout << "Working!" << std::endl;
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				if (G(x, y) == UNTYPED)
				{
					G(x, y) = INTERIOR;

					
			 	}
			}

		}

	}


	void Print_Grid() 
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				std::cout << G(i, j) << "  ";
			}
			std::cout << "\n";
		}
	}



  };
