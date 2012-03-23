// Maze Cost
// Jeff Ruhl
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
using namespace std;

int get_cost( char );

int main( int argc, char *argv[] ) {
	ifstream in("cost.in");
	if ( in ) {
		ofstream out("cost.out");

		string line;

		getline( in, line );
		int set_count = atoi( line.c_str() );

		for ( int i = 0; i < set_count; i++ ) {
			int cost = 0;

			int row_count, col_count;
			in >> row_count; in >> col_count;
			// consume rest of line
			getline( in, line );

			for ( int row = 0; row < row_count; row++ ) {
				getline( in, line );
				for ( int col = 0; col < col_count; col++ ) {
					cost += get_cost( line[col] );
				}
			}

			out << cost << endl;
		}

		out.close();
		in.close();
	}
	else {
		cout << "File 'cost.in' not found" << endl;
	}

	return 0;
}

int get_cost( char segment ) {
	switch ( segment ) {
	case 'X':
		return 50;
	case '-':
		return 10;
	case '+':
		return 500;
	default:
		return 0;
	}

}
