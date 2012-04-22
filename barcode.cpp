// Jeff Ruhl
// Interleaved 2 of 5
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <deque>
#include <map>
using namespace std;

#define NARROW_BAR 'N'
#define WIDE_BAR 'W'
#define NARROW_SPACE 'n'
#define WIDE_SPACE 'w'

typedef map<int, string> BarcodeMap;

BarcodeMap get_barcode_map(); 
string convert_int_to_barcode( long long, const BarcodeMap& );
string convert_number_to_barcode( string, const BarcodeMap& );
char strip_to_bar( char );
char strip_to_space( char );

int main( int argc, char const *argv[] ) {
	ifstream fin("barcode.in");
	if ( fin ) {
		ofstream fout("barcode.out");

		BarcodeMap barcode_map = get_barcode_map();
		string number;
		getline( fin, number );
		while ( "0" != number ) {
			string barcode = convert_number_to_barcode( number, barcode_map );
			fout << barcode << endl;

			getline( fin, number );
		}

		fout.close();
		fin.close();
	}
	else {
		cout << "Couldn't open 'barcode.in'" << endl;
	}

	return 0;
}

BarcodeMap get_barcode_map() {
	BarcodeMap barcode_map;

	barcode_map[0] = "nnwwn";
	barcode_map[1] = "wnnnw";
	barcode_map[2] = "nwnnw";
	barcode_map[3] = "wwnnn";
	barcode_map[4] = "nnwnw";
	barcode_map[5] = "wnwnn";
	barcode_map[6] = "nwwnn";
	barcode_map[7] = "nnnww";
	barcode_map[8] = "wnnwn";
	barcode_map[9] = "nwnwn";

	return barcode_map;
}

string convert_number_to_barcode( string number, const BarcodeMap& barcode_map ) {
	if ( 0 != number.size() % 2 ) {
		number = "0" + number;
	}

	stringstream barcode( stringstream::in | stringstream::out );
	barcode << NARROW_BAR << NARROW_SPACE << NARROW_BAR << NARROW_SPACE;
	for ( auto digit1 = number.begin(), digit2 = digit1 + 1; ; ) {
		auto digit1_barcode = barcode_map.at( *digit1 - 48 );
		auto digit2_barcode = barcode_map.at( *digit2 - 48 );
		for ( auto digit1_barcode_strip = digit1_barcode.begin(), digit2_barcode_strip = digit2_barcode.begin();
				digit1_barcode_strip != digit1_barcode.end();
				++digit1_barcode_strip, ++digit2_barcode_strip )
		{
			barcode << strip_to_bar( *digit1_barcode_strip ) << strip_to_space( *digit2_barcode_strip );
		}

		if ( digit1 + 2 != number.end() ) {
			 digit1 += 2, digit2 += 2;
		}
		else {
			break;
		}
	}
	barcode << WIDE_BAR << NARROW_SPACE << NARROW_BAR;

	return barcode.str();
}

string convert_int_to_barcode( long long number, const BarcodeMap& barcode_map ) {
	deque<int> digits;

	// chop off the last digit of 'number' at each iteration
	while ( number != 0 ) {
		int digit = number % 10;
		number /= 10;

		digits.push_front( digit );
	}
	// add an extra 0 to the front is there aren't an even number of digits
	// required due to digits being encoded in pairs
	if ( 0 != digits.size() % 2 ) {
		digits.push_front( 0 );
	}

	stringstream barcode( stringstream::in | stringstream::out );
	barcode << NARROW_BAR << NARROW_SPACE << NARROW_BAR << NARROW_SPACE;
	for ( auto digit1 = digits.begin(), digit2 = digit1 + 1; digit1 != digits.end(); digit1 += 2, digit2 += 2) {
		auto digit1_barcode = barcode_map.at( *digit1 );
		auto digit2_barcode = barcode_map.at( *digit2 );
		for ( auto digit1_barcode_strip = digit1_barcode.begin(), digit2_barcode_strip = digit2_barcode.begin();
				digit1_barcode_strip != digit1_barcode.end();
				++digit1_barcode_strip, ++digit2_barcode_strip )
		{
			barcode << strip_to_bar( *digit1_barcode_strip ) << strip_to_space( *digit2_barcode_strip );
		}
	}
	barcode << WIDE_BAR << NARROW_SPACE << NARROW_BAR;

	return barcode.str();
}

char strip_to_bar( char strip ) { return toupper( strip ); }
char strip_to_space( char strip ) { return strip; }
