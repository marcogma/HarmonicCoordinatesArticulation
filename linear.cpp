#include <igl/opengl/glfw/Viewer.h>
#include <igl/readOFF.h>
#include <igl/readPLY.h>
#include <igl/writeOBJ.h>
#include <iostream>
#include <ostream>
using namespace Eigen;

class LinearInterpolation{
  MatrixXd V;
  public:
    LinearInterpolation(const MatrixXd &V0){
      V = V0;
    }

    // evaluate function at time t
    float eval_function(float t)
	{
      // complete here
		for (int i = 0; i < V.rows()-1; i++)
		{
			if (V(i, 0) < V(i + 1, 0))
			{
				if (t >= V(i, 0) && t < V(i + 1, 0))
				{
					float x0, x1, y0, y1;
					y0 = V(i, 1);
					y1 = V(i + 1, 1);
					x0 = V(i, 0);
					x1 = V(i + 1, 0);

					return y0 * (1 - ((t - x0) / (x1 - x0))) + y1 * (((t - x0) / (x1 - x0)));

				}

			}
			else
			{
				if (t >= V(i+1, 0) && t < V(i, 0))
				{
					float x0, x1, y0, y1;
					y0 = V(i+1, 1);
					y1 = V(i, 1);
					x0 = V(i+1, 0);
					x1 = V(i , 0);

					return y0 * (1 - ((t - x0) / (x1 - x0))) + y1 * (((t - x0) / (x1 - x0)));

				}

			}
			

		}

		return 0.;
    }

  };
