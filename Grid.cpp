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
  	Eigen::MatrixXf Harmonics; //size*size Harmonics(x,y) contains the Harmonic Coordinate of vertex x,y
  	Eigen::MatrixXi InteriorV;
  	std::vector<MatrixXd> v_Harmonics;
	


  public:

    Grid(const int &s){
      size = std::pow(2,s);
	  step = (xMax - xMin) / (size - 1);
	  G = Eigen::MatrixXi::Zero(size, size);
	  //std::cout << G.rows() << " " << G.cols() << std::endl;
	  Harmonics = Eigen::MatrixXf::Zero(size, size);
	  //Coarse_G= Eigen::MatrixXi::Zero(s-1, s-1);
    }

    void Tag_Cell(int x, int y, int value)
	{
		G(x, y) = value;
    }

	int get_CellTag(int x, int y)
	{
		return G(x,y);
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
	


	//Interpolation of cage vertices (discretize the cage) , it must set the BOUNDARY region and calculate Harmonic in cage
	void Scan_Convert_Boundary(MatrixXi Cage)
	{

	
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

	//Get the harmonic coordinates on the boundaries
	float phi_cage(int i, int x, int y)
	{

		return 0.;
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

	//Probably need to create a dictionary of interior Vertices, otherwise recursion will take forever traversing the Grid
	//key = vertex , value = array of 4 neighbors vertices 
	void Laplacian_Smooth(float tau=0.00001)
	{
		float change = 1.0;
		while (change > tau)
		{
			Eigen::MatrixXf TempHarmonics=Harmonics;

			for (int i = 0; i < InteriorV.rows(); i++)
			{
				int x = InteriorV.row(i)(0);
				int y = InteriorV.row(i)(1);
				float mean= Neighborhood_mean(x, y);
				float cellValue = TempHarmonics(x, y);
				change = abs(mean - cellValue);
				if (change < tau)
					return;

				TempHarmonics(x, y) = Neighborhood_mean(x, y);
			}

			Harmonics = TempHarmonics;

		}
		
	}

	

	float Neighborhood_mean(int x, int y)
	{
		float mean = (Harmonics(x + 1, y) + Harmonics(x - 1, y) + Harmonics(x, y + 1) + Harmonics(x, y - 1)) / 4;
		return mean;
	}



	void Laplacian_Smooth_Coarser()
	{

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
