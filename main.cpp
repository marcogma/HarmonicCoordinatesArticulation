#include "Util.h"
#include <ctime>
#include <chrono>

MatrixXd cageVertices;
MatrixXi cageFaces;

MatrixXd meshVertices;
MatrixXi meshFaces;

MatrixXd Vmouse;
MatrixXi Fmouse;

bool parallel = false;

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
			// Drawing mesh
			updateMesh(weights, meshVertices, cageVertices);
			viewer.data(0).add_points(meshVertices, Eigen::RowVector3d(1, 0, 0));
			draw_curve(viewer, meshVertices);
		}
		return true;
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
	createRectangleMouse(Vmouse, Fmouse, 20.0);
	if (argc < 2)
	{
		cageVertices = createCircle(5, 500);
		meshVertices = createCircle(3, 800);
	}
	else
	{
		if (std::strncmp(argv[1],"-p", 2) == 0){
			parallel = true;
		}
		//igl::readPLY("../data/humanCage.ply", cageVertices, cageFaces);
		//cageVertices = createCircle(8, 8);
		createRectangle(cageVertices, cageFaces, 8);
		igl::readPLY("../data/humanFigure.ply", meshVertices, meshFaces);
	}
	;
	Grid G(7);
	G.Add_Cage(cageVertices);
	G.Add_Mesh(meshVertices);
	G.Fill_Grid_Regions();


	//G.Fill_CoarseGrid();
	//for (int i = 0; i < cageVertices.rows(); i++) {
	//	G.Laplacian_Smooth_Coarse(0.00001, i);
	//}
	//G.Push_CoarseHarmonics();

	if (parallel){
		std::cout << "Parallel realization" << std::endl; 
		auto start = std::chrono::high_resolution_clock::now();

		int num_threads = cageVertices.rows();
		std::thread t[num_threads];
		for (int i = 0; i < num_threads; i++){
			t[i] = std::thread(&Grid::Laplacian_Smooth, &G, 0.000001, i);
		}
		for (int i = 0; i < num_threads; i++){
			t[i].join();
		}
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		std::cout << "Elapsed time: " << elapsed.count() << std::endl;
	}
	else{
		std::cout << "Sequential realization" << std::endl; 
		auto start = std::chrono::high_resolution_clock::now();
		for (int i = 0; i < cageVertices.rows();i++){
			G.Laplacian_Smooth(0.000001, i);
		}
		auto finish = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double> elapsed = finish - start;
		std::cout << "Elapsed time: " << elapsed.count() << std::endl;
	}

	G.assignWeights();

	// Estimate mean error
	std::cout << "Mean error is: " << G.estimate_mean_error() << std::endl;

	weights = G.get_weights();

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
