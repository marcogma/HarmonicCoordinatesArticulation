#include "Util.h"

MatrixXd cageVertices;
MatrixXi cageFaces;

MatrixXd meshVertices;
MatrixXi meshFaces;

MatrixXd Vmouse;
MatrixXi Fmouse;

std::vector<std::vector<float>> weights;

bool click = false;
int clickedVertex;

void updateMesh(const std::vector<std::vector<float>> &weights, MatrixXd &vertices, const MatrixXd &cage){
	for (int i = 0; i < vertices.rows(); i++){
		RowVectorXd point(3);
		point << 0,0,0;
		for (int j = 0; j < weights[i].size(); j++){
			point(0) += weights[i][j]*cage(j,0);
			point(1) += weights[i][j]*cage(j,1);
		}
		vertices.row(i) = point;
	}
}

bool mouse_move(igl::opengl::glfw::Viewer& viewer, int mouse_x, int mouse_y){
	if (click)
		{
			Vector3d Pos;
			Pos = get_MousePositionCoord(viewer, Vmouse, Fmouse);
			cageVertices.row(clickedVertex) = Pos;
			viewer.data(0).clear();
			viewer.data().clear();
			viewer.data(0).add_points(cageVertices, Eigen::RowVector3d(1, 0, 0));
			draw_curve(viewer, cageVertices);
		}
		return false;
};

bool mouse_down(igl::opengl::glfw::Viewer& viewer, int button, int modifier){
		Vector3d Pos;
		Pos = get_MousePositionCoord(viewer, Vmouse, Fmouse);
		clickedVertex = get_ClosestVertex(cageVertices ,Pos(0), Pos(1));
		if(clickedVertex ==-1)
			return false;
		click = true;
		return false;
	};

bool mouse_up(igl::opengl::glfw::Viewer& viewer, int button, int modifier){
	if(click){
		updateMesh(weights, meshVertices, cageVertices);
		viewer.data(0).add_points(meshVertices, Eigen::RowVector3d(1, 0, 0));
		draw_curve(viewer, meshVertices);
	}

	click = false;
	return false;
};


// ------------ main program ----------------
int main(int argc, char *argv[])
{
	createRectangle(Vmouse, Fmouse, 20.0);
	if (argc < 2)
	{
		std::cout << "Creating a rectangle" << std::endl;
		createRectangle(cageVertices, cageFaces);
		createRectangle(meshVertices, meshFaces, 2);
	}
	else
	{
		igl::readPLY("../data/figure2.ply", cageVertices, cageFaces);
		igl::readPLY("../data/ifigure2.ply", meshVertices, meshFaces);
	}

	Grid G(5);
	G.Add_Cage(cageVertices);
	G.Add_Mesh(meshVertices);
	G.Fill_Grid_Regions();


	G.Fill_CoarseGrid();
	for (int i = 0; i < cageVertices.rows(); i++) {
		G.Laplacian_Smooth_Coarse(0.000001, i);
	}
	G.Push_CoarseHarmonics();


	for (int i = 0; i < cageVertices.rows();i++){
		G.Laplacian_Smooth(0.000001, i);
	}
	G.assignWeights();
	weights = G.get_weights();
	for (int i = 0; i < weights.size(); i++){
		std::cout<<"i: " << i << " ";
		float sum = 0;
		for (int j = 0; j < weights[i].size(); j++){
			sum += weights[i][j];
		}
		std::cout<< "sum: " << sum << std::endl;
	}
	igl::opengl::glfw::Viewer viewer; // create the 3d viewer
	//G.draw_heatmap(viewer,0);

	draw_points(viewer, cageVertices); // draw the bounding box (red edges and vertices)
	draw_curve(viewer, cageVertices);
	draw_points(viewer, meshVertices); // draw the bounding box (red edges and vertices)
	draw_curve(viewer, meshVertices);
	viewer.core(0).align_camera_center(cageVertices, cageFaces);

	// Callback functions
	viewer.callback_mouse_move = &mouse_move;
	viewer.callback_mouse_down = &mouse_down;
	viewer.callback_mouse_up = &mouse_up;


	viewer.launch(); // run the viewer	
}
