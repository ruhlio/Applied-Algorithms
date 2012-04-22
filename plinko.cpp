// Jeff Ruhl
// Plinko Plus
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node;
typedef vector<Node> Path;
typedef vector<Path> Paths;
typedef vector<int> Row;
typedef vector<Row> Rows;
typedef vector<int> Values;

struct Node
{
   int value;
   int pos_from_left;
};

int find_greatest_path_value( const Rows& );
int find_greatest_path_value( const Rows::const_iterator&, 
   const Rows::const_iterator&, int );
int get_path_value( const Path& );
string path_to_string( const Path& );

int main( int argc, char *argv[] ) {
   ifstream fin("plinko.in");
   if ( fin ) {
      ofstream fout("plinko.out");

      int rowCount = 0;
      fin >> rowCount;
      while ( 0 != rowCount ) {
         Rows rows;
         int path_value = 0;
         for (int row_index = 1; row_index <= rowCount; ++row_index) {
            Row row;
            for ( int i = 0; i < row_index; ++i ) {
               fin >> path_value;
               row.push_back( path_value );
            }
            rows.push_back( row );
         }

         int greatest_path_value = find_greatest_path_value( rows );
         fout << greatest_path_value << endl;

         fin >> rowCount;
      }

      fout.close();
      fin.close();
   }
   else {
      cout << "Couldn't open file 'plinko.in'" << endl;
   }

   return 0;
}

int find_greatest_path_value( const Rows& rows ) {
	// extra column of 0s added to max_values to avoid checking for out of bounds
   Rows max_values( rows.size(), Row( rows.size() + 1, 0 ) );

	// rows is 0 indexed, max_values columns are 1 indexed
   max_values[0][1] = rows[0][0];
   for ( size_t rows_index = 1; rows_index < rows.size(); rows_index++ ) {
      Row row = rows[rows_index];
      for ( size_t row_index = 1; row_index <= row.size(); row_index++ ) {
         max_values[rows_index][row_index] = 
            row[row_index - 1] + max( max_values[rows_index - 1][row_index - 1], 
                                  max_values[rows_index - 1][row_index] );
      }
   }

   Row last_max_value_row = *(max_values.end() - 1);
   int greatest_value = *max_element( last_max_value_row.begin(), last_max_value_row.end() );

   return greatest_value;
}

// int find_greatest_path_value( const Rows& rows ) {
//    return find_greatest_path_value( rows.begin(), rows.end(), 0 );
// }

// int find_greatest_path_value( 
//    const Rows::const_iterator& row_iter,
//    const Rows::const_iterator& end,
//    int offset )
// {
//    // base case
//    if ( row_iter == end ) {
//       return 0;
//    }

//    int value = row_iter->at( offset );
//    // represents the values of the two nodes below ([0] = left, [1] = right)
//    int child_values[2];
   
//    #pragma omp parallel for
//    for ( int i = 0; i < 2; i++ ) {
//       child_values[i] = find_greatest_path_value( row_iter + 1, end, offset + i );
//    }

//    return value + max( child_values[0], child_values[1] );
// }

// int find_greatest_path_value( const Rows& rows ) {
//    // need to determine what position (left->right) the path was in
//    // position required to determine adjacent nodes (the two possibilities diagonally below)
//    Paths* paths = new Paths();
//    // all paths begin from the single choice of the first row
//    {
//       Path path { {rows[0][0], 0} };
//       paths->push_back( path );
//    }

//    // from here add to paths every possible path
//    // because last row has no adjacent nodes it is skipped
//    int row = 0;
//    for ( auto row_iter = rows.begin(); row_iter != (rows.end() - 1); ++row_iter ) {
//       Paths* new_paths = new Paths();
//       for ( Path path : *paths ) {
//          auto next_row_iter = row_iter + 1;
//          int pos_from_left = path.back().pos_from_left;

//          // remove the old path, add the two new possible paths from it
//          Path leftPath = path;
//          leftPath.push_back( { next_row_iter->at( pos_from_left ),
//             pos_from_left } );
//          Path rightPath = path;
//          rightPath.push_back( { next_row_iter->at( pos_from_left + 1),
//             pos_from_left + 1 } );
//          new_paths->push_back( leftPath ); new_paths->push_back( rightPath );
//       }
//       delete paths;
//       paths = new_paths;
//       row++;
//    }

//    int max_path_value = 0;
//    for ( Path path : *paths ) {
//       int path_value = get_path_value( path );
//       max_path_value = max( path_value, max_path_value );

//       //cout << path_to_string( path ) << ", value: " << path_value << endl;
//    }
//    delete paths;

//    return max_path_value;
// }

int get_path_value( const Path& path ) {
   int total_value = 0;
   for ( Node node: path ) {
      total_value += node.value;
   }

   return total_value;
}

string path_to_string( const Path& path ) {
   stringstream output( stringstream::in | stringstream:: out );

   for ( Node node : path ) {
      output << node.value << "->";
   }

   // trim trailing '->'
   string str = output.str();
   str.resize( str.size() - 2 );
   return str;
}
