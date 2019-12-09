#include "Util.h"
#include <igl/readPLY.h>
MatrixXd Cage;
MatrixXd Figure;
MatrixXi F;
std::vector<MatrixXf> Harmonics;

bool click = false;
int clickedVertex;


// ------------ main program ----------------
int main(int argc, char *argv[])
{

	if (argc < 1)
	{
		std::cout << "Creating a rectangle" << std::endl;
		//createRectangle(V, F,4);
	}
	else
	{
		//igl::readPLY("../data/figure.ply", V, F);
	}
	createRectangle(Cage, F, 8);
	createRectangle(Figure, F, 6);
	//igl::readPLY("../data/figure2.ply", Cage, F);
	//igl::readPLY("../data/ifigure2.ply", Figure, F);

	Grid G(5);
	G.Add_Cage(Cage);
	G.Fill_Grid_Regions();
	std::cout << "********* GRID BEFORE LAPLACIAN**********" << std::endl;
	//G.Print_Grid();
	//std::cout << "********* Harmonics in boundaries BEFORE LAPLACIAN**********" << std::endl;
	//G.Print_Harmonics(0);
	//Coarse grid
	//G.Fill_CoarseGrid();
	//std::cout << "********* Coarsed GRID BEFORE LAPLACIAN**********" << std::endl;
	//G.Print_CoarseGrid();
	//std::cout << "********* Harmonics in boundaries BEFORE LAPLACIAN**********" << std::endl;
	//G.Print_CoarseHarmonics(0);
	//std::cout << "********* **********" << std::endl;
	//G.Laplacian_Smooth_Coarse();
	//G.Push_CoarseHarmonics();
	for (int i = 0; i < Cage.rows(); i++)
	{
		G.Laplacian_Smooth(0.00001, i);
	}
	

	Harmonics = G.get_Harmonics();
	//cout << "Harmoinics 0 *************" << endl;
	//G.Print_Harmonics(0);
	//cout << "Harmoinics 1 *************" << endl;
	//G.Print_Harmonics(1);
	//cout << "Harmoinics 2 *************" << endl;
	//G.Print_Harmonics(2);
	//cout << "Harmoinics 3 *************" << endl;
	//G.Print_Harmonics(3);


	igl::opengl::glfw::Viewer viewer; // create the 3d viewer
	//G.draw_heatmap(viewer, 0);
	

	viewer.callback_key_down = &key_down;
	viewer.callback_mouse_move =
	[&](igl::opengl::glfw::Viewer& viewer, int, int)->bool
	{
		if (click)
		{

			MatrixXd Vmouse;
			MatrixXi Fmouse;
			createRectangle(Vmouse, Fmouse, 40.0); // Mouse area
			Vector3d Pos;
			Pos = get_MousePositionCoord(viewer, Vmouse, Fmouse);
			//std::cout << "x: " << Pos(0) << "  y: " << Pos(1) << std::endl;
			Cage.row(clickedVertex) = Pos;
			viewer.data(0).clear();
			viewer.data().clear();
			
			viewer.data(0).add_points(Cage, Eigen::RowVector3d(1, 0, 0));
			draw_curve(viewer, Cage);
			return true;
		}
		return false;
	};

	viewer.callback_mouse_down =
		[&](igl::opengl::glfw::Viewer& viewer, int, int)->bool
	{
		Vector3d Pos;
		MatrixXd Vmouse;
		MatrixXi Fmouse;
		createRectangle(Vmouse, Fmouse, 40.0); // Mouse area
		Pos = get_MousePositionCoord(viewer, Vmouse, Fmouse);
		clickedVertex = get_ClosestVertex(Cage ,Pos(0), Pos(1));
		if(clickedVertex ==-1)
			return false;
		click = true;
		return true;
	};

	viewer.callback_mouse_up =
		[&](igl::opengl::glfw::Viewer& viewer, int, int)->bool
	{
		if (click)
		{
			updateMesh(Cage, Figure, Harmonics, G.get_step());
			viewer.data(0).add_points(Figure, Eigen::RowVector3d(1, 0, 0));
			draw_curve(viewer, Figure);
		}
		click = false;
		return true;
	};
	viewer.append_mesh();
	draw_points(viewer, Cage); // draw the bounding box (red edges and vertices)
	draw_curve(viewer, Cage);
	//viewer.append_mesh();
	draw_points(viewer, Figure); // draw the bounding box (red edges and vertices)
	draw_curve(viewer, Figure);

	viewer.core(0).align_camera_center(Cage, F);
	viewer.launch(); // run the viewer
	


}
