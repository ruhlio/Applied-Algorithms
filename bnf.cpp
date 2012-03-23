// Jeff Ruhl
// Backus-Naur Form

#include <fstream>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <exception>
#include <regex>
#include <algorithm>
#include <map>
#include <cstdlib>
using namespace std;

class Exception : std::exception
{
	const char* _what;

public:
	Exception( const char* what ) : _what( what ) {}

	const char* what() const throw() { return _what; }
};

template <typename T>
class Option
{
protected:
	bool _is_empty;
	T _val;

public:
	bool is_empty() { return _is_empty; }
	T get() { 
		if ( _is_empty ) {
			throw Exception("No value");
		}
		return _val; 
	}

};

template <typename T>
class Some : public Option<T>
{
public:
	Some( T val ) {
		this->_is_empty = false;
		this->_val = val;
	}

};

template <typename T>
class None : public Option<T>
{
public:
	None() {
		this->_is_empty = true;
	}

};

typedef map<string, int> VariableMap;
Option<int> eval( const string& );

VariableMap variables;

int main( int argc, char *argv[] ) {
	ifstream fin("bnf.in");
	if ( fin ) {
		ofstream fout("bnf.out");

		string line;
		while ( getline( fin, line ) && "HALT" != line ) {
			stringstream output( stringstream::in | stringstream::out );
			output << setiosflags( ios::left ) << setw(31) << line;
			try {
				auto result = eval( line );
				if ( !result.is_empty() ) {
					output << result.get();
				}
			}
			catch ( const exception& ex ) {
				output << ex.what();
			}
			
			string trimmed_output = output.str();
			trimmed_output.erase( trimmed_output.find_last_not_of( ' ' ) + 1 );
			fout << trimmed_output << endl;
		}

		fout.close();
		fin.close();
	}
	else {
		cout << "Couldn't open file 'bnf.in'" << endl;
	}

	return 0;
}

regex variable( "^([xyz]).*" );
regex op( "^([+-]).*" );
regex num( "^([0-9])+.*" );

void bad_syntax_error() {
	throw Exception("Bad syntax");
}

void undefined_var_error( const string& var_name ) {
	throw Exception( (var_name + " is undefined").c_str() );
}

int eval_term( const char** pos ) {
	int result = 0;
	smatch match;
	string input = *pos;
	if ( regex_match( input, match, variable ) ) {
		string var = match[1].str();
		*pos += var.length();
		if ( variables.end() != variables.find( var ) ) {
			result = variables[var];
		}
		else { undefined_var_error( var ); }
	}
	else if ( regex_match( input, match, num ) ) {
		string num = match[1].str();
		*pos += num.length();
		result = atoi( num.c_str() );
	}
	else { bad_syntax_error(); }

	return result;
}

int eval_expression( const char* pos ) {
	smatch match;

	int lhs = eval_term( &pos );

	string input = pos;
	while ( regex_match( input, match, op ) ) {
		char op = match[1].str()[0];
		pos++;
		if ( '+' == op ) {
			lhs += eval_term( &pos );
		}
		else {
			lhs -= eval_term( &pos );
		}
		input = pos;
	}
	
	return lhs;
}

void eval_assignment( const char* pos ) {
	smatch match;
	string input = pos;
	if ( regex_match( input, match, variable ) ) {
		string var = match[1].str();
		pos += var.length();

		if ( '=' == *pos ) {
			variables[var] = eval_expression( ++pos );
		}
		else { bad_syntax_error(); }
	}
	else { bad_syntax_error(); }
}

Option<int> eval( const string& line ) {
	regex print_statement("^PRINT\\(([xyz])\\)");

	// strip whitespace
	string clean_line = regex_replace( line, regex("\\s"), string("") );

	smatch match;
	if ( regex_match( clean_line, match, print_statement ) ) {
		string var = match[1].str();
		if ( variables.end() == variables.find( var ) ) { undefined_var_error( var ); } 
		return Some<int>( variables[var] );
	}

	eval_assignment( clean_line.c_str() );
	return None<int>();
}
