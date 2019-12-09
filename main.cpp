#include "Util.h"

MatrixXd Cage;
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
		createTriangle(Cage, F);
	}
	else
	{

	}
	Grid G(7);
	G.Add_Cage(Cage);

	G.Fill_Grid_Regions();
	G.Laplacian_Smooth();
	igl::opengl::glfw::Viewer viewer; // create the 3d viewer
	viewer.callback_key_down = &key_down;
	//G.draw_heatmap(viewer, 0);
	draw_points(viewer, Cage); // draw the bounding box (red edges and vertices)
	draw_curve(viewer, Cage);



	viewer.core(0).align_camera_center(Cage, F);
	viewer.launch(); // run the viewer
	
}
