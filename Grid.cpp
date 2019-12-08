#include <igl/opengl/glfw/Viewer.h>
#include <iostream>
#include <ostream>
#include <cmath>
#include <math.h>
#include <utility>


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
  	std::vector<std::pair<int, int>> InteriorV;
  	std::vector<MatrixXf> Harmonics;
	


  public:

    Grid(const int &s){
      size = std::pow(2,s);
	  step = (xMax - xMin) / (size - 1);
	  G = Eigen::MatrixXi::Zero(size, size);
    }

	void Add_Cage(MatrixXd& cage)
	{
		for (int i = 0; i < cage.rows(); i++){
			Harmonics.push_back(MatrixXf::Zero(size, size));
		}
		for (int i = 0; i < cage.rows(); i++)
		{
			int x0, y0, x1, y1;
			x0 = std::round((cage.row(i)(0) - xMin) / step);
			y0 = std::round((cage.row(i)(1) - yMin) / step);
			if (i < cage.rows() - 1)
			{
				x1 = std::round((cage.row(i + 1)(0) - xMin) / step);
				y1 = std::round((cage.row(i + 1)(1) - yMin) / step);
			}
			else
			{
				x1 = std::round((cage.row(0)(0) - xMin) / step);
				y1 = std::round((cage.row(0)(1) - yMin) / step);
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
		Harmonics[i](x0,y0) = 1;

		int dx = abs(x1 - x0);
		int sx = x0 < x1 ? 1 : -1;
		int dy = abs(y1 - y0);
		int sy = y0 < y1 ? 1 : -1;
		int err = (dx > dy ? dx : -dy) / 2;
		int e2;
		int xb = x0;
		int yb = y0;
		std::vector<std::pair<int, int>> pts;
		while (1)
		{
			G(xb, yb) = BOUNDARY;
			if (xb == x1 && yb == y1) break;
			e2 = err;
			if (e2 > -dx)
			{
				err -= dy;
				xb += sx;
			}
			if (e2 < dy) 
			{
				err += dx;
				yb += sy;
			}
			pts.push_back(std::pair<int,int>(xb, yb));
		}

		float t = 1 / (float) pts.size();

		for (int j = 0; j < pts.size(); j++){
			Harmonics[i](pts[j].first, pts[j].second) = 1 - t*(j+1);

			if(i < Harmonics.size() - 1){
				Harmonics[i+1](pts[j].first, pts[j].second) = t*(j+1);
			}
			else{
				Harmonics[0](pts[j].first, pts[j].second) = t*(j+1);
			}
		}
	}



	//Fill the grid starting with exterior region and then the interior
	void Fill_Grid_Regions()
	{
		Flood_Fill(0, 0);
		//std::cout << "Working!" << std::endl;
		for (int x = 0; x < size; x++)
		{
			for (int y = 0; y < size; y++)
			{
				if (G(x, y) == UNTYPED)
				{
					G(x, y) = INTERIOR;
					InteriorV.push_back(std::pair<int,int>(x, y));
			 	}
			}

		}

	}

	float mean2d(int x, int y, int idx){
		return (Harmonics[idx](x+1,y) + Harmonics[idx](x, y+1) + Harmonics[idx](x, y - 1) + Harmonics[idx](x-1, y)) / 4.0;
	}

	void Laplacian_Smooth(float tau=0.00001, int idx = 0)
	{
		float change = 1;
		float tmp;
		while (change > tau)
		{
			change = 0;
			Eigen::MatrixXf TempHarmonics=Harmonics[idx];
			for (int i = 0; i < InteriorV.size(); i++)
			{
				int x = InteriorV[i].first;
				int y = InteriorV[i].second;
				//std::cout << "X: " << x << " Y: " << y << std::endl;
				float mean = mean2d(x, y, idx);
				float cellValue = TempHarmonics(x, y);
				tmp = abs(mean - cellValue);
				change = std::max(tmp, change);
				TempHarmonics(x, y) = mean2d(x, y, idx);
			}
			std::cout <<"Change: " << change << std::endl;
			Harmonics[idx] = TempHarmonics;
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

	void Print_Harmonics(int k) 
	{
		for (int i = 0; i < size; i++)
		{
			for (int j = 0; j < size; j++)
			{
				std::cout << Harmonics[k](i, j) << "  ";
			}
			std::cout << "\n";
		}
	}

	void draw_heatmap(igl::opengl::glfw::Viewer& viewer, int idx){
  		viewer.append_mesh();
  		for (int i = 0; i < Harmonics[idx].rows(); i++){
    		for (int j = 0; j < Harmonics[idx].cols(); j++){
      			RowVectorXd point(3);
      			float x = xMin + i*step;
      			float y = yMin + j*step;
      			point << x,y,0;
      			if(Harmonics[idx](i,j) == 0){
        			viewer.data(0).add_points(point, Eigen::RowVector3d(1, 0, 0));
      			}
      			else{
        			viewer.data(0).add_points(point, Eigen::RowVector3d(0, Harmonics[idx](i,j), 0));
      			}
   		 	}
  		}
	}	



  };
