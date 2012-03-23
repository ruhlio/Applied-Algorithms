// Anagrams
// Jeff Ruhl
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <regex>
#include <set>
using namespace std;

#define FILE_END "<end>"

typedef multiset<char> CharMultiSet;

bool is_anagram( string, string  );

int main( int argc, char *argv[] ) {
	ifstream in("anagrams.in");
	if ( in ) {
		ofstream out("anagrams.out");

		string line1, line2;
		while ( getline( in, line1 ) && FILE_END != line1 && getline( in, line2 ) && FILE_END != line2 ) {
			if ( is_anagram( line1, line2 ) ) {
				out << "YES" << endl;
			}
			else {
				out << "NO" << endl;
			}
		}

		out.close();
		in.close();
	}
	else {
		cout << "File 'anagrams.in' not found" << endl;
	}


	return 0;
}

bool is_anagram( string phrase1, string phrase2  ) {
	CharMultiSet phrase1_set, phrase2_set;

	auto clean_string = []( const string& str ) -> string {
		string result;

		regex irrelevant_chars("[^a-zA-Z]");
		result = regex_replace( str, irrelevant_chars, string("") );

		transform( result.begin(), result.end(), result.begin(), ::tolower );

		return result;
	};

	auto fill_set = []( const string& phrase, CharMultiSet& set ) {
		for_each( phrase.begin(), phrase.end(), [&]( char c ) {
			set.insert( c );
		});
	};

	phrase1 = clean_string( phrase1 );
	phrase2 = clean_string( phrase2 );
	
	fill_set( phrase1, phrase1_set );
	fill_set( phrase2, phrase2_set );

	return phrase1_set == phrase2_set;
}
