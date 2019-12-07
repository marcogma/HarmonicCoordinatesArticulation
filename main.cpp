#include "Util.h"

MatrixXd V;
MatrixXi F;
MatrixXd Vi;
MatrixXi Fi;
MatrixXd U;
MatrixXi G;
VectorXi J;


// ------------ main program ----------------
int main(int argc, char *argv[])
{

	if (argc < 2)
	{
		std::cout << "Creating a rectangle" << std::endl;
		createRectangle(V, F);
	}
	else
	{

	}
	Grid G(6);
	G.Add_Cage(V);

	G.Fill_Grid_Regions();
	G.Print_Grid();

	igl::opengl::glfw::Viewer viewer; // create the 3d viewer
	viewer.callback_key_down = &key_down;

	draw_points(viewer, V); // draw the bounding box (red edges and vertices)
	draw_curve(viewer, V);



	viewer.core(0).align_camera_center(V, F);
	viewer.launch(); // run the viewer
	
}
