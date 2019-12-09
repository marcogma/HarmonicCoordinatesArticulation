#include "Util.h"
#include <igl/readPLY.h>
MatrixXd V;
MatrixXi F;
MatrixXd Vi;
MatrixXi Fi;
MatrixXd U;
MatrixXi G;
MatrixXd Vmouse;
MatrixXi Fmouse;
VectorXi J;
bool click = false;
int clickedVertex;


// ------------ main program ----------------
int main(int argc, char *argv[])
{

	if (argc < 1)
	{
		std::cout << "Creating a rectangle" << std::endl;
		createRectangle(V, F,4);
	}
	else
	{
		igl::readPLY("../data/figure.ply", V, F);
	}
	//createRectangle(V, F, 4);
	createRectangle(Vmouse, Fmouse, 20.0); // Mouse area
	Grid G(6);
	G.Add_Cage(V);
	G.Fill_Grid_Regions();
	std::cout << "********* GRID BEFORE LAPLACIAN**********" << std::endl;
	G.Print_Grid();
	//std::cout << "********* Harmonics in boundaries BEFORE LAPLACIAN**********" << std::endl;
	//G.Print_Harmonics(0);
	//Coarse grid
	G.Fill_CoarseGrid();
	std::cout << "********* Coarsed GRID BEFORE LAPLACIAN**********" << std::endl;
	G.Print_CoarseGrid();
	//std::cout << "********* Harmonics in boundaries BEFORE LAPLACIAN**********" << std::endl;
	//G.Print_CoarseHarmonics(0);
	std::cout << "********* **********" << std::endl;
	G.Laplacian_Smooth_Coarse();
	G.Push_CoarseHarmonics();
	G.Laplacian_Smooth();
	//G.Print_Harmonics(0);
	igl::opengl::glfw::Viewer viewer; // create the 3d viewer
	G.draw_heatmap(viewer, 0);


	viewer.callback_key_down = &key_down;
	viewer.callback_mouse_move =
	[&](igl::opengl::glfw::Viewer& viewer, int, int)->bool
	{
		if (click)
		{
			Vector3d Pos;
			Pos = get_MousePositionCoord(viewer, Vmouse, Fmouse);
			//std::cout << "x: " << Pos(0) << "  y: " << Pos(1) << std::endl;
			V.row(clickedVertex) = Pos;
			viewer.data(0).clear();
			viewer.data().clear();
			viewer.data(0).add_points(V, Eigen::RowVector3d(1, 0, 0));
			draw_curve(viewer, V);
			return true;
		}
		return false;
	};

	viewer.callback_mouse_down =
		[&](igl::opengl::glfw::Viewer& viewer, int, int)->bool
	{
		Vector3d Pos;
		Pos = get_MousePositionCoord(viewer, Vmouse, Fmouse);
		clickedVertex = get_ClosestVertex(V ,Pos(0), Pos(1));
		if(clickedVertex ==-1)
			return false;
		click = true;
		return true;
	};

	viewer.callback_mouse_up =
		[&](igl::opengl::glfw::Viewer& viewer, int, int)->bool
	{
		click = false;
		return true;
	};

	draw_points(viewer, V); // draw the bounding box (red edges and vertices)
	draw_curve(viewer, V);

	viewer.core(0).align_camera_center(V, F);
	viewer.launch(); // run the viewer
	


}
