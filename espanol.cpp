// Ordenación en Español
// Jeff Ruhl
#include <iostream>
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <algorithm>
#include <cstring>
using namespace std;

typedef list<string> StringList;
typedef map<const char*, int> CharValueMap;

const char* chars[] = {
	"a","b","c","ch","d","e","f","g","h","i","j","k","l","ll","m","n","~n","o","p","q","r","rr","s","t","u","v","w","x","y","z"
};
int chars_length = 30;
CharValueMap double_char_values;
CharValueMap single_char_values;

// split the alphabet into separate maps so double chars can be
// searched for before single chars
void init_char_value_maps() {
	for ( size_t i = 0; i < chars_length; i++ ) {
		if ( 1 == strlen( chars[i] ) ) {
			single_char_values[chars[i]] = i;
		}
		else {
			double_char_values[chars[i]] = i;
		}
	}
}

// gets value of char in str at specified pos, looking at double
// chars before single chars
int get_char_value_at( const string& str, int &pos ) {
	int value = 0;

	for_each ( double_char_values.begin(), double_char_values.end(), [=, &value, &pos]( CharValueMap::value_type pair ) {
		if ( 0 == strcmp( pair.first, str.substr(pos, 2).c_str() ) ) {
			value = pair.second;
			pos++; // increment to skip next char
			return;
		}
	});
	if ( 0 < value ) return value;

	for_each ( single_char_values.begin(), single_char_values.end(), [=, &value]( CharValueMap::value_type pair ) {
		if ( pair.first[0] == str[pos] ) {
			value = pair.second;
			return;
		}
	});
	return value;
}

bool compare_espanol_string( string a, string b ) {
	int aValue = 0;
	int bValue = 0;
	for ( int aIndex = 0, bIndex = 0; aIndex < a.length() && bIndex < b.length(); aIndex++, bIndex++ ) {
		aValue = get_char_value_at( a, aIndex );
		bValue = get_char_value_at( b, bIndex );

		if ( aValue < bValue ) return true;
		if ( bValue < aValue ) return false;
	}
	// a matches b, but is shorter
	if ( a.length() < b.length() ) return true;

	// equivalent put b first in order to maintain weak strict ordering
	return false;
}

int main( int argc, char *argv[] ) {
	init_char_value_maps();

	ifstream fin("espanol.in");
	if ( fin ) {
		ofstream fout("espanol.out");

		int line_count;
		fin >> line_count;

		StringList lines;
		string line;
		// skip first line, will be blank for >> operation
		getline( fin, line );
		for ( int i = 0; i < line_count; i++ ) {
			getline( fin, line );
			lines.push_back( line );
		}

		lines.sort( compare_espanol_string );

		while ( !lines.empty() ) {
			fout << lines.front() << endl;
			lines.pop_front();
		}

		fout.close();
		fin.close();
	}
	else {
		cout << "Input file not found." << endl;
	}

	return 0;
}
