// Jeff Ruhl
// OSPF Routing
#include <cassert>
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <queue>
#include <algorithm>
#include <limits>
using namespace std;

typedef float Cost;
typedef vector<vector<Cost> > RoutingTable;

struct RouterCost
{
  int router_id;
  Cost cost;

  RouterCost( int router_id, Cost cost ) {
    this->router_id = router_id;
    this->cost = cost;
  }
  
  bool operator>( const RouterCost& rhs ) const {
    return this->cost > rhs.cost;
  }
};

double calculate_bandwidth_cost( Cost );
vector<Cost> find_costs( int, const RoutingTable& );

int main( int argc, char const *argv[] ) {
	ifstream fin("ospf.in");
	if ( fin ) {
		ofstream fout("ospf.out");
    fout << setprecision( 2 ) << fixed;

    bool first_routing_table = true;
		int router_count = 0;
		fin >> router_count;
		while ( 0 != router_count ) {
			RoutingTable routing_table( router_count + 1, RoutingTable::value_type( router_count + 1, 0 ) );
      if ( first_routing_table ) {
        first_routing_table = false;
      }
      else {
        fout << "\n*****" << endl;
      }
      
			for ( int router = 1; router <= router_count; ++router ) {
				for ( int connected_router = 1; connected_router <= router_count; ++connected_router ) {
					long long bps = 0;
					fin >> bps;
					routing_table[router][connected_router] = calculate_bandwidth_cost( bps );
				}
			}

			int from_router = 0;
      bool first_from_router = true;
			fin >> from_router;
			while ( from_router != 0 ) {
        auto costs = find_costs( from_router, routing_table );

        if ( first_from_router ) {
          first_from_router = false;
        }
        else {
          fout << endl;
        }
        fout << from_router << ": ";

        bool first_to_router = true;
				for ( int to_router = 1; to_router <= router_count; ++to_router ) {
					if ( to_router == from_router ) {
						continue;
					}

          if ( first_to_router ) {
            first_to_router = false;
          }
          else {
            fout << " ";
          }
          fout << to_router << "(" << costs[to_router] << ")";
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

inline double calculate_bandwidth_cost( Cost bandwidth ) {
	return (0 == bandwidth ? 0 : 100000000.0 / bandwidth);
}

vector<Cost> find_costs( int from_router_id,
                         const RoutingTable& routing_table ) {
	vector<int> parents;
  vector<Cost> distances;
  vector<bool> visited( false, routing_table.size() );
	priority_queue<RouterCost, vector<RouterCost>, greater<RouterCost> > cheapest;

  // scans a router, finding any shorter paths from it to other routers
  auto scan = [&]( int router_id ) {
    auto adjacent_router_costs = routing_table[router_id];
    for ( size_t connected_router_id = 1;
          connected_router_id < adjacent_router_costs.size();
          ++connected_router_id )
      {
        Cost cost = adjacent_router_costs[connected_router_id];
        // if connected_router really is connected
        if ( 0 != cost && !visited[connected_router_id] ) {
          // if we've come across a shorter path then re-assign the connected
          // router's parent & distance
          Cost total_cost = distances[router_id] + cost;
          if ( total_cost < distances[connected_router_id] ) {
            distances[connected_router_id] = total_cost;
            parents[connected_router_id] = router_id;
            cheapest.push( RouterCost( connected_router_id, total_cost ) );
          }
        }
      }
  };

  // router with id of 0 is never referenced but we need something here to
  // keep the parallel vector indices aligned with the router ids
  distances.push_back( 0 );
  parents.push_back( 0 );

  // initialize arrays
	for ( int router_id = 1; (size_t)router_id < routing_table.size(); ++router_id ) {
    parents.push_back( 0 );
    visited.push_back( false );
 
    // initialize distances for all routers to infinity except the first router
    if ( router_id == from_router_id ) {
      distances.push_back( 0 );
    }
    else {
      distances.push_back( numeric_limits<Cost>::max() );
    }
	}

    cheapest.push( RouterCost( from_router_id, distances[from_router_id] ) );
  // perform dijkstra's algorithm
  while ( !cheapest.empty() ) {
    // select shortest distance
    auto router_distance = cheapest.top(); cheapest.pop();
    // scan it
    if ( !visited[router_distance.router_id] ) {
      visited[router_distance.router_id] = true;
      scan( router_distance.router_id );
    }
  }

	return distances;
}
