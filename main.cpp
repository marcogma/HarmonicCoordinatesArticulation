#include <igl/opengl/glfw/Viewer.h>
#include <iostream>
#include <ostream>
#include <igl/readOFF.h>
#include <igl/writeOFF.h>
#include <igl/doublearea.h>
#include <igl/massmatrix.h>
#include <igl/invert_diag.h>
#include <igl/jet.h>
#include "Util.cpp"
#include "linear.cpp"
#include <igl/gaussian_curvature.h>
#include <igl/per_vertex_normals.h>
#include <igl/per_face_normals.h>
#include <igl/copyleft/progressive_hulls.h>
#include <igl/readPLY.h>
#include "Grid.cpp"
#include "Cage.cpp"

using namespace igl::copyleft;
using namespace Eigen; // to use the classes provided by Eigen library
using namespace std;

MatrixXd V;
MatrixXi F;
MatrixXd Vi;
MatrixXi Fi;
MatrixXd U;
MatrixXi G;
VectorXi J;

void draw_curve(igl::opengl::glfw::Viewer& viewer, const MatrixXd& V) {
	viewer.append_mesh();
	for (unsigned i = 0; i < V.rows() - 1; ++i)
		viewer.data().add_edges(
			V.row(i),
			V.row(i + 1),
			Eigen::RowVector3d(1, 0, 0));
	viewer.data().add_edges(
		V.row(V.rows()-1),
		V.row(0),
		Eigen::RowVector3d(1, 0, 0));
}

/*draw points from the list of points V*/
void draw_points(igl::opengl::glfw::Viewer& viewer, const MatrixXd& V) {
	viewer.append_mesh();
	viewer.data(0).add_points(V, Eigen::RowVector3d(1, 0, 0));
}

/*interpolate the leftmost and rightmost X position*/
void build_linspace(MatrixXd& linspace, const MatrixXd& V) {
	for (size_t i = 0; i < linspace.rows(); i++) {
		linspace(i, 0) = V.col(0).minCoeff() + ((V.col(0).maxCoeff() - V.col(0).minCoeff()) / (linspace.rows() - 1)) * i;
	}
}

// This function is called every time a keyboard button is pressed
bool key_down(igl::opengl::glfw::Viewer &viewer, unsigned char key, int modifier)
{

	if (key == '2')
	{
		int x = viewer.current_mouse_x;
		int y = viewer.core().viewport(3) - viewer.current_mouse_y;
		cout << "Cursor Position at (" << x << " : " << y << endl;
		MatrixXd point(1,3);
		point << x, y,0;
		draw_points(viewer, point);

	}

	return false;
}






/**
 * Create a triangle mesh corresponding to an octagon inscribed in the unit circle
 */
void createOctagon(MatrixXd &Vertices, MatrixXi &Faces)
{
	Vertices = MatrixXd(6, 3);
	Faces = MatrixXi(8, 3);

	Vertices << 0.0, 0.0, 1.0,
		1.000000, 0.000000, 0.000000,
		0.000000, 1.000000, 0.000000,
		-1.000000, 0.000000, 0.000000,
		0.000000, -1.000000, 0.000000,
		0.000000, 0.000000, -1.000000;

	Faces << 0, 1, 2,
		0, 2, 3,
		0, 3, 4,
		0, 4, 1,
		5, 2, 1,
		5, 3, 2,
		5, 4, 3,
		5, 1, 4;
}


void createRectangle(MatrixXd& Vertices, MatrixXi& Faces)
{
	Vertices = MatrixXd(4, 3);
	Faces = MatrixXi(2, 3);

	Vertices << 0.0, 0.0, 0.0,
		5.0, 0.000000, 0.000000,
		5.0, 5.0, 0.000000,
		0.000000, 5.0, 0.000000,


	Faces << 0, 1, 2,
		2, 3, 1;
}

// ------------ main program ----------------
int main(int argc, char *argv[])
{

	if (argc < 2)
	{
		std::cout << "Creating an octagon" << std::endl;
		//createOctagon(V, F);
		createRectangle(V, F);
	}
	else
	{
		//2D figures cage and object
		//igl::readPLY(argv[1], V, F);
		//igl::readPLY(argv[2], Vi, Fi);

	}
	createRectangle(V, F);
	Grid G(15,0.5);
	//Cage C(V);

	G.Add_Cage(V);
	G.Fill_Grid_Regions();
	G.Print_Grid();

	igl::opengl::glfw::Viewer viewer; // create the 3d viewer
	viewer.callback_key_down = &key_down;

	draw_points(viewer, V); // draw the bounding box (red edges and vertices)
	draw_curve(viewer, V);

	//draw_points(viewer, Vi); // draw the bounding box (red edges and vertices)
	//draw_curve(viewer, Vi);



	//viewer.data().set_mesh(V, F);
	cout << V.rows();
	Util curve_renderer(viewer);

	viewer.core(0).align_camera_center(V, F);
	viewer.launch(); // run the viewer
	
}
