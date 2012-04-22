// Jeff Ruhl
// OSPF Routing
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
using namespace std;

typedef vector<vector<float> > RoutingTable;

float calculate_bandwidth_cost( unsigned long long );
int find_short_path_value( int, int, const RoutingTable& );

struct RouterDistance
{
	int router;
	float distance;
};

int main( int argc, char const *argv[] ) {
	ifstream fin("ospf.in");
	if ( fin ) {
		ofstream fout("ospf.out");

		int router_count = 0;
		fin >> router_count;
		while ( 0 != router_count ) {
			RoutingTable routing_table( router_count + 1, RoutingTable::value_type( router_count + 1, 0 ) );

			for ( int router = 1; router <= router_count; ++router ) {
				for ( int connected_router = 1; connected_router <= router_count; ++connected_router ) {
					unsigned long long  bps = 0;
					fin >> bps;
					routing_table[router][connected_router] = calculate_bandwidth_cost( bps );
				}
			}

			int from_router = 0;
			fin >> from_router;
			while ( from_router != 0 ) {
				for ( int to_router = 1; to_router <= router_count; ++to_router ) {
					if ( to_router == from_router ) {
						break;
					}

					find_short_path_value( from_router, to_router, routing_table );
				}

				fin >> from_router;
			}

			fin >> router_count;
		}

		fout.close();
		fin.close();
	}
	else {
		cout << "Couldn't open file 'ospf.in'" << endl;
	}

	return 0;
}

inline float calculate_bandwidth_cost( unsigned long long bandwidth ) {
	return (0 == bandwidth ? 0 : 100000000.0 / bandwidth);
}

int find_short_path_value( int from, int to, const RoutingTable& routing_table ) {
	vector<int> parents( 0, routing_table.size() );
	priority_queue<RouterDistance> distances;

	for ( int router = 1; router < routing_table.size(); ++router ) {
		RouterDistance distance = { router, -1 };
		distances.push( distance );
	}



	return 0;
}
