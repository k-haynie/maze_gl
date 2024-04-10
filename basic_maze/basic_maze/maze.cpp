#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <random>


#include "cell.h"
#include "grid.h"
#include "Kruskals.cpp"
#include "PrimsSimp.cpp"
#include "RecursiveBacktracker.cpp"

#include "glut.h"

/* algorithms to cover:

 - Eller's << p.193
 - Recursive Divison << p.200

 Better graphics:

 - 3D with unreal engine
 - ChunkyPNG equivalent for C++?
*/
/*
using namespace std;

// because of OpenGL this has to be global
vector<vector<Cell*>> maze;

int randint(int high, int low = 0) {
	random_device rd;
	mt19937 generator(rd());
	uniform_int_distribution<int> distr(low, high);
	return distr(generator);
};

void asciiPrint() {
	int r = maze.size();
	int c = maze[0].size();

	// Print out the first line
	string dash = "+";
	for (int j = 0; j < c; j++) {
		dash = dash.append("---+");
	};
	cout << dash << endl;

	// Print out the actual board
	for (int i = 0; i < r; i++) {
		string line = "|";
		for (int j = 0; j < c; j++) {
			if (!maze[i][j]->e_conn) {
				line = line.append("   |");
			}
			else {
				line = line.append("    ");
			};
		};
		cout << line << endl;

		string dash = "+";
		for (int j = 0; j < c; j++) {
			if (!maze[i][j]->s_conn) {
				dash = dash.append("---+");
			}
			else
			{
				dash = dash.append("   +");
			}
		};
		cout << dash << endl;
	}
};

void glutDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	
	// initialize units with 2 extra for padding
	float xunit = 2 / (2.0 + maze[0].size());
	float yunit = 2 / (2.0 + maze.size());

	// draw the top
	for (int i = 1; i-1 < maze[0].size(); i++) {
		glBegin(GL_LINES);
			glColor3f(0.0, 1.0, 0.0);
			glVertex2f(-1 + (xunit * i), 1 - yunit);
			glVertex2f(-1 + (xunit * (i+1)), 1 - yunit);
		glEnd();	
	}

	for (int i = 0; i < maze.size(); i++) {
		glBegin(GL_LINES);
		glColor3f(0.0, 1.0, 0.0);
		glVertex2f(-1 + xunit, 1 - (yunit * (i + 1)));
		glVertex2f(-1 + xunit, 1 - (yunit * (i + 2)));
		glEnd();

		for (int j = 0; j < maze[0].size(); j++) {

			if (!maze[i][j]->e_conn) {
				glBegin(GL_LINES);
				glColor3f(0.0, 1.0, 0.0);
				glVertex2f(-1 + (xunit * (j + 2)), 1 - (yunit * (i + 1)));
				glVertex2f(-1 + (xunit * (j + 2)), 1 - (yunit * (i + 2)));
				glEnd();
			}

			if (!maze[i][j]->s_conn) {
				glBegin(GL_LINES);
				glColor3f(0.0, 1.0, 0.0);
				glVertex2f(-1 + (xunit * (j + 1)), 1 - (yunit * (i + 2)));
				glVertex2f(-1 + (xunit * (j + 2)), 1 - (yunit * (i + 2)));
				glEnd();
			}
		}
	}

	glFlush();
}

// https://www3.ntu.edu.sg/home/ehchua/programming/opengl/CG_Examples.html
void glutRender() {
	glClear(GL_COLOR_BUFFER_BIT);

	// initialize units with 2 extra for padding
	float xunit = 2 / (2.0 + maze[0].size());
	float yunit = 2 / (2.0 + maze.size());

	// draw the top
	for (int i = 1; i - 1 < maze[0].size(); i++) {
		glBegin(GL_QUADS);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(-1 + (xunit * i), 0, 1 - yunit);
		glVertex3f(-1 + (xunit * i), 0.2, 1 - yunit);
		glVertex3f(-1 + (xunit * (i + 1)), 0.2, 1 - yunit);
		glVertex3f(-1 + (xunit * (i + 1)), 0, 1 - yunit);
		glEnd();
	}

	for (int i = 0; i < maze.size(); i++) {
		glBegin(GL_QUADS);
		glColor3f(0.0, 0.0, 1.0);
		glVertex3f(-1 + xunit, 0,  1 - (yunit * (i + 1)));
		glVertex3f(-1 + xunit, 0.2, 1 - (yunit * (i + 1)));
		glVertex3f(-1 + xunit, 0.2,  1 - (yunit * (i + 2)));
		glVertex3f(-1 + xunit, 0,  1 - (yunit * (i + 2)));
		glEnd();

		for (int j = 0; j < maze[0].size(); j++) {

			if (!maze[i][j]->e_conn) {
				glBegin(GL_QUADS);
				glColor3f(1.0, 0.0, 0.0);
				glVertex3f(-1 + (xunit * (j + 2)), 0,  1 - (yunit * (i + 1)));
				glVertex3f(-1 + (xunit * (j + 2)), 0.2, 1 - (yunit * (i + 1)));
				glVertex3f(-1 + (xunit * (j + 2)), 0.2, 1 - (yunit * (i + 2)));
				glVertex3f(-1 + (xunit * (j + 2)), 0, 1 - (yunit * (i + 2)));
				glEnd();
			}

			if (!maze[i][j]->s_conn) {
				glBegin(GL_QUADS);
				glColor3f(0.0, 1.0, 0.0);
				glVertex3f(-1 + (xunit * (j + 1)), 0, 1 - (yunit * (i + 2)));
				glVertex3f(-1 + (xunit * (j + 1)), 0.2, 1 - (yunit * (i + 2)));
				glVertex3f(-1 + (xunit * (j + 2)), 0.2, 1 - (yunit * (i + 2)));
				glVertex3f(-1 + (xunit * (j + 2)), 0, 1 - (yunit * (i + 2)));
				glEnd();
			}
		}
	}

	glFlush();
}

void glutPrint(int argc, char** argv, bool z_axis=false) {
	glutInit(&argc, argv);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Maze Window");
	glutInitWindowPosition(50, 50);
	if (!z_axis) {
		glutDisplayFunc(glutDisplay);
	}
	else {
		glutDisplayFunc(glutRender);
		gluLookAt(-0.1, 0, 0.1, 0, 0.2, 0, 0, 1, 0);
	}
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.5, 1.5, -1.5, 1.5);

	glShadeModel(GL_SMOOTH);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	
	glutMainLoop();
}


int main(int argc, char** argv) {
	int r, c;

	do {
	cout << "Width of maze (less than 26): ";
	cin >> c;
	} while (c > 100 || c < 1);

	do {
	cout << "Height of maze (less than 26): ";
	cin >> r;
	} while (r > 100 || r < 1);

	Grid grid;
	maze = grid.prepare_grid(r, c);
	grid.configure_cells(r, c, maze);
	
	//RecBktr alg;
	//alg.on(maze, randint(r-1), randint(c-1));
	Kruskals krusk;
	krusk.on(maze);

	// https://www.geeksforgeeks.org/how-to-setup-opengl-with-visual-studio-2019-on-windows-10/
	// https://www3.ntu.edu.sg/home/ehchua/programming/opengl/CG_Introduction.html
	asciiPrint();
	glutPrint(argc, argv, true);
	
	return 0;
};*/