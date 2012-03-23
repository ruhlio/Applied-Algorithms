// Jeff Ruhl
// Bowling
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <exception>
#include <algorithm>
using namespace std;

#define MESSAGE_INVALID "INVALID"
#define STRIKE 'X'
#define SPARE '/'
#define MAX_SCORE 10
#define FRAMES_PER_GAME 10


class Exception : std::exception
{
	const char* _what;

public:
	Exception( const char* what ) : _what( what ) {}

	const char* what() const throw() { return _what; }
};

inline int is_num( char c ) {
	return (c >= 48 && c <=57);
}

inline int char_to_int( char c ) {
	return c - 48;
}

typedef vector<char> ScoreVector;

// this is terrible code
int calculate_final_score( const ScoreVector& score_sheet ) {
	int final_score = 0;

	auto get_score = [&]( int index ) -> int {
		switch ( score_sheet[index] ) {
		case STRIKE:
			return MAX_SCORE;
		case SPARE:
			if ( STRIKE == score_sheet[index-1] || SPARE == score_sheet[index-1] ) {
				throw Exception(MESSAGE_INVALID);
			}
			return MAX_SCORE - char_to_int( score_sheet[index - 1] );
		default:
			return char_to_int( score_sheet[index] );
		}
	};

	int frames_played = 0;
	for ( size_t i = 0; i < score_sheet.size(); ) {
		int score_1 = 0, score_2 = 0;

		switch ( score_sheet[i] ) {
		case STRIKE:
			// check for end of score sheet
			if ( i + 2 >= score_sheet.size() ) {
				i += 2;
				break;
			}

			// strike must be alone
			if ( STRIKE != score_sheet[i+1] && STRIKE == score_sheet[i+2] ) {
				throw Exception(MESSAGE_INVALID);
			}
			score_1 = get_score( i + 1 ); score_2 = get_score( i + 2 );
			if ( is_num( score_sheet[i+1] ) && is_num( score_sheet[i+2] ) && MAX_SCORE <= score_1 + score_2 ) {
				throw Exception(MESSAGE_INVALID);
			}
			final_score += MAX_SCORE + score_1 + score_2;
			i += 1;
			frames_played++;
			// check for end of score sheet
			if ( i + 2 >= score_sheet.size() ) {
				i += 2;
			}

			break;
		// can only end with a spare
		case SPARE:
			throw Exception(MESSAGE_INVALID);
		// digit
		default: {
			// following a strike as the bowl of the last frame there should be two more bowls
			// following neither a strike or spare there should be nothing
			if ( 0 != i && i + 1 == score_sheet.size() && (STRIKE == score_sheet[i-1] || SPARE != score_sheet[i-1]) ) {
				throw Exception(MESSAGE_INVALID);
			}
			// check for end of score sheet
			if ( i + 1 >= score_sheet.size() ) {
				i += 1;
				break;
			}

			int score = char_to_int( score_sheet[i] );
			switch ( score_sheet[i+1] ) {
			// a strike is always by itself
			case STRIKE:
				throw Exception(MESSAGE_INVALID);
			case SPARE:
				// if last frame there should be another bowl
				if ( i + 2 >= score_sheet.size() ) {
					throw Exception(MESSAGE_INVALID);
				}
				final_score += MAX_SCORE + get_score( i + 2 );
				break;
			default:
				int total_score = score + char_to_int( score_sheet[i + 1] );
				if ( MAX_SCORE <= total_score ) {
					throw Exception(MESSAGE_INVALID);
				}
				final_score += total_score;
				break;
			}

			i+= 2;
			frames_played++;
		}break;

		}

		if ( frames_played > FRAMES_PER_GAME ) {
			throw Exception(MESSAGE_INVALID);
		}
	}

	if ( frames_played < FRAMES_PER_GAME ) {
		throw Exception(MESSAGE_INVALID);
	}
	return final_score;
}

int main( int argc, char* argv[] ) {
	ifstream fin("bowling.in");
	if ( fin ) {
		ofstream fout("bowling.out");

		string line;
		while ( getline( fin, line ) && "*" != line ) {
			stringstream stream( line );
			ScoreVector score_sheet;
			ScoreVector::value_type score;

			for ( int i = 0; i < line.length(); i++ ) {
				stream >> score;
				score_sheet.push_back( score );
			}

			try {
				int final_score = calculate_final_score( score_sheet );
				fout << final_score << endl;
			}
			catch ( const exception& ex ) {
				fout << ex.what() << endl;
			}
		}

		fout.close();
		fin.close();
	}
	else {
		cout << "Could not open file 'bowling.in'" << endl;
	}

	return 0;
}
