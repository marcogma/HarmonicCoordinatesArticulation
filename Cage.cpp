#include <igl/opengl/glfw/Viewer.h>
#include <iostream>
#include <ostream>
using namespace Eigen;

class Cage
{
  int size;
  Eigen::MatrixXd V;

  public:
    Cage(const MatrixXd& Vi ){
		V = Vi;
		size = V.rows();
    }

  };
