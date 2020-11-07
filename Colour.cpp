/* PROBLEM 3 ///////////////////////////////////////////////////////////////////
 * N  rectangles of  different colours are superposed on a white sheet of paper.
 * The sheet's  sizes are:  a cm wide and b cm long. The rectangles are put with
 * their sides parallel  to the sheet's borders.  All rectangles fall within the
 * borders of  the sheet. As result, different figures of different colours will
 * be seen. Two regions of the same colour are considered to be part of the same
 * figure  if they  have  at  least one  point  in common;  otherwise,  they are
 * considered different figures. The problem is to calculate the area of each of
 * these figures. a, b are even positive integers not greater than 30.
 *
 * The  coordinate  system considered  has origin  at the sheet's center and the
 * axes parallel to the sheet's borders:  Different data  sets are written in an
 * ASCII input file,  RECTANG.DAT: a,  b and N will be in the first line of each
 * data set, separated by a blank space.
 *
 * In each of  the next N lines  you will find:  the integer  coordinates of the
 * position where the  left lower  vertex of the rectangle was put,  followed by
 * the integer  coordinates of the position  where the upper right vertex of the
 * rectangle was put and, then, the rectangle's colour represented by an integer
 * between 1 and 64.  White  colour will  be represented by 1.  The order of the
 * records  corresponds  to  the order used to put the rectangles  on the sheet.
 * Different data sets will be separated with a blank record.
 *
 * Write a program to:
 * 1. Read the next data set from RECTANG.DAT
 * 2. Calculate the area of each coloured figure
 * 3. Write in an  ASCII output  file,  RECTANG.SOL,  the colour and the area of
 *    each coloured figure as shown in the example below.  These records will be
 *    written  in  increasing order  of colour.  The solutions to different data
 *    sets will be separated by a blank record.
 *
 * Example:
 * RECTANG.DAT          RECTANG.SOL
 * 20 12 5              1 172
 * -7 -5 -3 -1 4        2 47
 * -5 -3 5 3 2          4 12
 * -4 -2 -2 2 4         4 8
 * 2 -2 3 -1 12         12 1
 * 3 1 7 5 1
 *
 * 30 30 2              1 630
 * 0 0 5 14 2           2 70
 * -10 -7 0 13 15       15 200
 *//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;
const int MaxPixSiz = 80;
const int NofDr = 4; // number of directions
const char ClrChar[] = {
	'-', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
};
class Pix {
	int a;
	int b;
	int car[MaxPixSiz][MaxPixSiz]; // colour array
	int half_a;
	int half_b;
public:
	Pix(const int a = 0, const int b = 0) {
		reset(a, b);
	}
	/* convert from coordinates to matrix indices */
	int get_i(const int y) {
		return half_b - y;
	}
	int get_j(const int x) {
		return half_a + x;
	}
	void paint(int xl, int yl, int xu, int yu, int clr) {
		int i_min = get_i(yu);
		int i_max = get_i(yl);
		int j_min = get_j(xl);
		int j_max = get_j(xu);
		for (int i = i_min; i < i_max; ++i) {
			for (int j = j_min; j < j_max; ++j) {
				car[i][j] = clr;
			}
		}
	}
	void nop() {
	}
	void reset(const int a, const int b) {
		this->a = a;
		this->b = b;
		half_a = a >> 1;
		half_b = b >> 1;
		for (int i = 0; i < b; ++i) {
			for (int j = 0; j < a; ++j) {
				car[i][j] = 0;
			}
		}
	}
	friend ostream& operator<<(ostream& os, const Pix& px) {
		ostringstream oss;
		for (int i = 0; i < px.b; ++i) {
			for (int j = 0; j < px.a; ++j) {
				oss << ClrChar[px.car[i][j]] << ' ';
			}
			oss << '\n';
		}
		return os << oss.str();
	}
	bool goNorth(int& i, int& j) {
		return !(--i < 0);
	}
	bool goEast(int& i, int& j) {
		return !(++j == a);
	}
	bool goSouth(int& i, int& j) {
		return !(++i == b);
	}
	bool goWest(int& i, int& j) {
		return !(--j < 0);
	}
	void flood_fill(int i, int j, int old_clr, int& n) {
		static bool (Pix::*f[])(int&, int&) = {
/* thats */	&Pix::goNorth,
			&Pix::goEast,
			&Pix::goSouth,
			&Pix::goWest 
		};
		car[i][j] = 0;
		++n;
		for (int k = 0; k < NofDr; ++k) {
			int i1 = i, j1 = j;
			if ((this->*f[k])(i1, j1)) {
				if (car[i1][j1] == old_clr) {
					flood_fill(i1, j1, old_clr, n);
				}
			}
		}
	}
	bool findNext(int& i, int& j) {
		for (; i < b; ++i, j = 0) {
			for (; j < a; ++j) {
				if (car[i][j]) return true;
			}
		}
		return false;
	}
	void doTheMath(ofstream &ofs) {
		int i, j;
		i = j = 0;
		while (findNext(i, j)) {
			int old_clr = car[i][j];
			int n = 0;
			flood_fill(i, j, old_clr, n);
			ofs << ClrChar[old_clr] << ' ' << n << endl;
		}
	}
};
int main(int argc, char* argv[]) {
	Pix px;
	ifstream ifs("RECTANG.DAT");
	ofstream ofs("RECTANG.SLU");
	int a, b, n;
	while (ifs >> a >> b >> n) {
		px.reset(a, b);
		for (int i = 0; i < n; ++i) {
			int x0, y0, x1, y1, clr;
			ifs >> x0 >> y0 >> x1 >> y1 >> clr;
			px.paint(x0, y0, x1, y1, clr);
		}
		ofs << px << endl;
		px.doTheMath(ofs);
		ofs << endl;
	}
	ifs.close();
	ofs.close();
}
// log:
