// Postfix Expressions
// Jeff Ruhl
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <stack>
#include <cctype>
#include <cstdlib>
using namespace std;

typedef map<char, string> DefinitionMap;
typedef stack<float> OperandStack;

struct BinaryOperands
{
	float l;
	float r;
};

void define( char, const string& );
float eval( const string&, OperandStack* operands = nullptr );

float op_sub( float , float  );
float op_div( float , float  );
BinaryOperands op_swap( float , float  );
BinaryOperands op_dup( float  );

DefinitionMap operator_defs;

string get_line_content( const string& line ) {
	int start = line.find( '"' );
	int end = line.rfind( '"' );

	return line.substr( start + 1, end - start - 1 );
}

int main( int argc, char* argv[] ) {
	ifstream in("postfix.in");
	if ( in ) {
		ofstream out("postfix.out");

		string line;
		while ( getline( in, line ) ) {
			switch ( line[0] ) {
			case 'D':
				define( line[1], get_line_content( line ) );
				break;
			case 'E':
				out << fixed << setprecision( 3 ) << eval( get_line_content( line ) ) << endl;
				break;
			case 'Q':
				goto ALL_DONE;
			}
		}
		ALL_DONE:

		out.close();
		in.close();
	}
	else {
		cout << "Could not open file 'postfix.in'" << endl;
	}

	return 0;
}

void define( char operator_name, const string& content ) {
	operator_defs[operator_name] = content;
}

float eval( const string& content, OperandStack* operands ) {		
	bool created_operands_stack = false;
	if ( nullptr == operands ) {
		operands = new OperandStack();
		created_operands_stack = true;
	}

	auto get_binary_operands = [&operands]() -> BinaryOperands {
		BinaryOperands ops;
		// pop in reverse order
		ops.r = operands->top(); operands->pop();
		ops.l = operands->top(); operands->pop();
		return ops;
	};

	string raw_operand;
	for ( int i = 0; i < content.length(); i++ ) {
		// parse operand, if any
		while ( isdigit( content[i] ) ) {
			raw_operand += content[i++];
		}
		if ( !raw_operand.empty() ) {
			// set iterator back one, gets incremented one too much during while loop
			i--;
			operands->push( (float)atoi( raw_operand.c_str() ) );
			raw_operand.clear();
		}
		// otherwise excecute operator
		else {
			BinaryOperands ops;
			BinaryOperands result;
			switch ( content[i] ) {
			case '-':
				ops = get_binary_operands();
				operands->push( op_sub( ops.l, ops.r ) );
				break;
			case '/':
				ops = get_binary_operands();
				operands->push( op_div( ops.l, ops.r ) );
				break;
			case '%':
				ops = get_binary_operands();
				result = op_swap( ops.l, ops.r );
				operands->push( result.l ); operands->push( result.r );
				break;
			case '!':
				result = op_dup( operands->top() ); operands->pop();
				operands->push( result.l ); operands->push( result.r );
				break;
			// ignore spaces
			case ' ':
				break;
			// user defined operator
			default:
				string oper = operator_defs[content[i]];
				operands->push( eval( oper, operands ) );
			}
		}

	}

	float result = operands->top();
	operands->pop();
	if ( created_operands_stack ) delete operands;
	return result;
}

float op_sub( float l_operand, float r_operand ) {
	return l_operand - r_operand;
}

float op_div( float l_operand, float r_operand ) {
	if ( 0.0f == r_operand ) return 0.0f;

	float result = l_operand / r_operand;
	return result;
}

BinaryOperands op_swap( float l_operand, float r_operand ) {
	BinaryOperands result = { r_operand, l_operand };
	return result;
}

BinaryOperands op_dup( float operand ) {
	BinaryOperands result = { operand, operand };
	return result;
}
