// Jeff Ruhl
// Poison Pills
#include <iostream>
#include <fstream>
#include <string>
#include <queue>
using namespace std;

typedef queue<bool> PillsQueue;
typedef queue<int> CaptivesQueue;
typedef priority_queue<int, vector<int>, greater<int>> DeadmanQueue;

bool is_deadly( char pill_name ) {
	return ('B' == pill_name ? true : false);
}

int main( int argc, char* argv[] ) {
	ifstream fin( "pills.in" );
	if ( fin ) {
		ofstream fout("pills.out");

		int pill_count = 0, captive_count = 0;
		fin >> pill_count;
		fin >> captive_count;
		while ( 0 != pill_count && 0 != captive_count ) {
			PillsQueue pills;
			CaptivesQueue captives;
			DeadmanQueue dead_men;

			char pill_name;
			for ( int i = 0; i < pill_count; i++ ) {
				fin >> pill_name;
				pills.push( is_deadly( pill_name ) );
			}
			
			for ( int i = 1; i <= captive_count; i++ ) {
				captives.push( i );
			}

			while ( !captives.empty() ) {
				int captive = captives.front(); captives.pop();

				// put back the first pill
				pills.push( pills.front() );
				pills.pop();

				// the next determines their fate
				if ( pills.front() ) {
					dead_men.push( captive );
				}
				pills.pop();
			}

			if ( dead_men.empty() ) {
				fout << 0;
			}
			while ( !dead_men.empty() ) {
				fout << dead_men.top(); dead_men.pop();
				// the last dead man gets no space
				if ( !dead_men.empty() ) {
					fout << " ";
				}
			}
			fout << endl;

			fin >> pill_count;
			fin >> captive_count;
		}

		fout.close();
		fin.close();
	}
	else {
		cout << "Could not open file 'pills.in'" << endl;
	}

	return 0;
}