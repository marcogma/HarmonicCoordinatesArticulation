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
  float step; 
  Eigen::MatrixXi G;  //Grid size*size. G(x,y) represents a Region: UNTYPED,BOUNDARY,INTERIOR,EXTERIOR
  Eigen::MatrixXi Coarse_G;  //Grid size-1*size-1. G(x,y) represents coarse version of G, following the "pulling" rules
  Eigen::MatrixXf Harmonics; //size*size Harmonics(x,y) contains the Harmonic Coordinate of vertex x,y
  public:
    Grid(const int &s, const float &step){
      size = s;
	  G = Eigen::MatrixXi::Zero(s, s);
	  Harmonics = Eigen::MatrixXi::Zero(s, s);
	  Coarse_G= Eigen::MatrixXi::Zero(s-1, s-1);
    }

    void Tag_Cell(int x, int y, int value)
	{
		G(x, y) = value;
    }

	int get_CellTag(int x, int y)
	{
		return G(x,y);
	}

	//Traverse the grid and changes the OldTag to a NewTag until a StopTag Appears 
	void Flood_Fill(int x, int y, int OldTag, int NewTag, int StopTag)
	{
		if (x < 0 || x >= size || y < 0 || y >= size || OldTag== StopTag)
			return;

		G(x,y) = NewTag;

		Flood_Fill( x + 1, y, G(x+1, y), NewTag, StopTag);
		Flood_Fill( x - 1, y, G(x-1, y), NewTag, StopTag);
		Flood_Fill( x, y + 1, G(x, y+1), NewTag, StopTag);
		Flood_Fill( x, y - 1, G(x, y-1), NewTag, StopTag);
		
	}


	//Interpolation of cage vertices (discretize the cage) , it must set the BOUNDARY region and calculate Harmonic in cage
	void Scan_Convert_Boundary(MatrixXi Cage)
	{
	
	}

	//Get the harmonic coordinates on the boundaries
	float phi_cage(int i, int x, int y)
	{

		return 0.;
	}


	//Fill the grid starting with exterior region and then the interior
	void Fill_Grid_Regions()
	{
		Flood_Fill(0, 0, G(0, 0), EXTERIOR, BOUNDARY);

		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				if (G(i, j) == UNTYPED)
				{
					G(i, j) = INTERIOR;
				}
			}

		}

	}

	//Probably need to create a dictionary of interior Vertices, otherwise recursion will take forever traversing the Grid
	//key = vertex , value = array of 4 neighbors vertices 
	void Laplacian_Smooth()
	{



	}

	

	float Neighborhood_mean(int x, int y)
	{
		float mean = (Harmonics(x + 1, y) + Harmonics(x - 1, y) + Harmonics(x, y + 1) + Harmonics(x, y - 1)) / 4;
		return mean;
	}



	void Laplacian_Smooth_Coarser()
	{

	}



  };
