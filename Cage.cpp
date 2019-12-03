#include <igl/opengl/glfw/Viewer.h>
#include <iostream>
#include <ostream>
using namespace Eigen;

class Cage
{
  int size;
  Eigen::MatrixXi V;

  public:
    Cage(const MatrixXd& Vi, const int& s ){
		V = Vi;
		size = s;
    }

  };
