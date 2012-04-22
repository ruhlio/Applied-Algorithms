// Jeff Ruhl
// Jungle Roads

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <cstdlib>
#include <queue>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

struct Path;
typedef vector<Path> PathVector;
typedef priority_queue<Path, PathVector, greater<Path> > PathPriorityQueue;

PathVector create_minimal_spanning_tree( PathPriorityQueue& );

struct Path 
{
	char from;
	char to;
	short cost;

};

bool operator>( const Path& lhs, const Path& rhs ) {
	return lhs.cost > rhs.cost;
}

int main( int argc, char* argv[] ) {
	ifstream fin("jungle.in");
	if ( fin ) {
		ofstream fout("jungle.out");
		string line;

		while ( getline( fin, line ) ) {
			PathPriorityQueue paths;
			int road_count = 0;

			road_count = atoi( line.c_str() );
			if ( 0 == road_count ) {
				break;
			}

			for ( int i = 0; i < road_count - 1; i++ ) {
				char village = 0;
				int path_count = 0;
				
				getline( fin, line );
				stringstream stream( line );

				stream >> village;
				stream >> path_count;
				for ( int j = 0; j < path_count; j++ ) {
					char connected_village = 0;
					short path_cost = 0;

					stream >> connected_village;
					stream >> path_cost;
					Path path = { village, connected_village, path_cost };
					paths.push( path );
				}

			}

			auto tree = create_minimal_spanning_tree( paths );
			int total_cost = 0;
			for_each( tree.begin(), tree.end(), [&]( const Path& path ) {
				total_cost += path.cost;
			});
			fout << total_cost << endl;
		}

		fout.close();
		fin.close();
	}
	else {
		cout << "Could not open file 'jungle.in'" << endl;
	}

	return 0;
}

// the STL map is not bidirectional, so two complementary maps are used
// insertion and deletion times are doubled, but finding is kept at O(log(n)) 
//	instead of the O(n) time that would be required for a single map

// key is the set that the vertex belongs to, value is the vertex name
typedef multimap< char, char > PathSets;
// key is the vertex name, value is the set that the vertex belongs to
typedef map< PathSets::mapped_type, PathSets::key_type > RPathSets;

PathVector create_minimal_spanning_tree( PathPriorityQueue& paths ) {
	PathVector tree;
	PathSets path_sets;
	RPathSets rpath_sets;

	auto union_find = [&]( const Path& path ) -> bool {

		// vertices without a set are assigned to a set of themself
		if ( rpath_sets.end() == rpath_sets.find( path.to ) ) {
			path_sets.insert( RPathSets::value_type( path.to, path.to ) );
			rpath_sets[path.to] = path.to;
		}
		if ( rpath_sets.end() == rpath_sets.find( path.from ) ) {
			path_sets.insert( RPathSets::value_type( path.from, path.from ) );
			rpath_sets[path.from] = path.from;
		}

		bool result = false;
		// in the same set
		if ( rpath_sets[path.from] == rpath_sets[path.to] ) {
			result = true;
		}
		// union the 'to' set to the 'from' set
		else {
			result = false;

			char from_set_name = rpath_sets[path.from];
			char to_set_name = rpath_sets[path.to];

			auto to_set_iter = path_sets.equal_range( to_set_name );
			vector<char> to_set;

			for_each( to_set_iter.first, to_set_iter.second, [&]( const PathSets::value_type& pair ) {
				to_set.push_back( pair.second );
			});

			// erase the vertices keyed under the old set
			path_sets.erase( to_set_iter.first, to_set_iter.second );

			// change the set of the vertices in the reverse map
			// add vertices back to forward map under new set
			for_each( to_set.begin(), to_set.end(), [&]( char to_vertex ) {
				rpath_sets[to_vertex] = from_set_name;
				path_sets.insert( PathSets::value_type( from_set_name, to_vertex ) );
			});

		}

		return result;
	};

	while ( !paths.empty() ) {
		Path path = paths.top(); paths.pop();

		if ( !union_find( path ) ) {
			tree.push_back( path );
		}

	}

	return tree;
}
