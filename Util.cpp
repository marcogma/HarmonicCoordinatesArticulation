#include "Util.h"

void draw_curve(igl::opengl::glfw::Viewer& viewer, const MatrixXd& V) {
	//viewer.append_mesh();
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
	//viewer.append_mesh();
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

	Vertices << -5, -5, 0.0,
				-5,  5, 0.000000,
				 5, 5, 0.000000,
			     5, -5, 0.000000,


	Faces << 0, 1, 2,
		2, 3, 1;
}

void createTriangle(MatrixXd& Vertices, MatrixXi& Faces)
{
	Vertices = MatrixXd(3, 3);
	Faces = MatrixXi(1, 3);

	
	Vertices << 2.0, 2.0, 0.0,
				4.0, 4.000000, 0.000000,
				6.0, 2.0, 0.000000,


	Faces << 0, 1, 2;
}

void createRectangle(MatrixXd& Vertices, MatrixXi& Faces, float size)
{
	Vertices = MatrixXd(4, 3);
	Faces = MatrixXi(2, 3);

	Vertices << -size, -size, 0.0,
				 size, -size, 0.0,
				 size, size, 0.0,
				-size, size, 0.0,


	Faces << 0, 1, 2,
			 2, 3, 1;
}

void createRectangleMouse(MatrixXd& Vertices, MatrixXi& Faces, float size)
{
	Vertices = MatrixXd(5, 3);
	Faces = MatrixXi(4, 3);

	Vertices << -size, -size, 0.0,
		size, -size, 0.0,
		size, size, 0.0,
		-size, size, 0.0,
		0, 0, 0;

		Faces << 0,1,4,
			     1,2,4,
			     2,3,4,
			     3,0,4;
}



RowVector3d get_MousePositionCoord(igl::opengl::glfw::Viewer& viewer, MatrixXd& V, MatrixXi& F)
{
	int fid;
	Eigen::Vector3f bc;
	// Cast a ray in the view direction starting from the mouse position
	double x = viewer.current_mouse_x;
	double y = viewer.core().viewport(3) - viewer.current_mouse_y;
	if (igl::unproject_onto_mesh(Eigen::Vector2f(x, y), viewer.core().view,
		viewer.core().proj, viewer.core().viewport, V, F, fid, bc))
	{
		// 3d position of hit
		const RowVector3d m3 = V.row(F(fid, 0)) * bc(0) + V.row(F(fid, 1)) * bc(1) + V.row(F(fid, 2)) * bc(2);
		return m3;
	}
	
	RowVector3d non;
	non << -1, -1, -1;
	return non;

}

int get_ClosestVertex(MatrixXd& V, float x, float y){
	int closest = 0;
	for (int i = 0; i < V.rows(); i++){
		if (std::sqrt(std::pow(V.row(i)(0) - x, 2) + std::pow(V.row(i)(1) - y, 2)) < 0.3){
			return(i);
		}
	}
	return -1;
}

MatrixXd createCircle(float radius, int numberPoints) {
	float da = 2 * 3.14159 / numberPoints;
	int i = 0;
	MatrixXd Vc(numberPoints, 3);
	for (double angle = 0; i < numberPoints; angle += da) {
		float x = radius * cos(angle);
		float y = radius * sin(angle);
		Vc(i, 0) = x;
		Vc(i, 1) = y;
		Vc(i, 2) = 0;
		i++;
	}
	return Vc;
	
}
