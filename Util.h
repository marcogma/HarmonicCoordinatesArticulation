#ifndef _UTIL_H_
#define _UTIL_H_
#endif

#include <igl/opengl/glfw/Viewer.h>
#include <iostream>
#include <ostream>
#include <igl/readOFF.h>
#include <igl/writeOFF.h>
#include <igl/readPLY.h>
#include "Grid.cpp"

using namespace std;

void draw_curve(igl::opengl::glfw::Viewer& viewer, const MatrixXd& V);

void draw_points(igl::opengl::glfw::Viewer& viewer, const MatrixXd& V);

void build_linspace(MatrixXd& linspace, const MatrixXd& V);

bool key_down(igl::opengl::glfw::Viewer &viewer, unsigned char key, int modifier);

void createOctagon(MatrixXd &Vertices, MatrixXi &Faces);

void createRectangle(MatrixXd &Vertices, MatrixXi &Faces);

void createTriangle(MatrixXd& Vertices, MatrixXi& Faces);

