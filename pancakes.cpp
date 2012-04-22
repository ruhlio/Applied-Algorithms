// Jeff Ruhl
// Flipping Pancakes
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cstring>
#include <vector>
#include <set>
#include <stack>
#include <queue>
using namespace std;

struct Pancake
{
	int size;
	bool is_burned_side_up;
};

class PancakeStack
{
public:
	PancakeStack( const string& input ) {
		_string_representation = input;

		for ( auto pancake = input.begin(); pancake != input.end(); ++pancake ) {
			if ( isupper( *pancake ) ) {
				Pancake bird = { *pancake - 65, false };
				_pancakes.push_back( bird );
			}
			else {
				Pancake bird = { *pancake - 97, true };
				_pancakes.push_back( bird );
			}
		}
	}

	PancakeStack flip( int howMany ) const {
		PancakeStack pancake_stack = *this;
		vector<Pancake> flipped_pancakes;
		for ( int i = 0; i < howMany; ++i ) {
			Pancake pancake = pancake_stack._pancakes.back();
			pancake_stack._pancakes.pop_back();
			flipped_pancakes.push_back( pancake );
		}

		for ( auto pancake = flipped_pancakes.begin(); pancake != flipped_pancakes.end(); ++pancake ) {
			pancake->is_burned_side_up = !pancake->is_burned_side_up;
			pancake_stack._pancakes.push_back( *pancake );
		}

		pancake_stack.update_string_representation();
		return pancake_stack;
	}

	bool is_presentable() const {
		bool result = true;
		int prev_pancake_size = 0;
		for ( auto pancake = _pancakes.begin(); pancake != _pancakes.end(); ++pancake ) {
			if ( pancake->is_burned_side_up || pancake->size < prev_pancake_size ) {
				result = false;
				break;
			}

			prev_pancake_size = pancake->size;
		}

		return result;
	}

	size_t size() const {
		return _pancakes.size();
	}

	bool operator<( const PancakeStack& other ) const {
		return this->_string_representation < other._string_representation;
	}

private:
	deque<Pancake> _pancakes;
	string _string_representation;

	void update_string_representation() {
		stringstream str( stringstream::in | stringstream::out );
		
		for ( auto pancake = _pancakes.begin(); pancake != _pancakes.end(); ++pancake ) {
			if ( pancake->is_burned_side_up ) {
				str << (char)(pancake->size + 97);
			}
			else {
				str << (char)(pancake->size + 65);
			}
		}

		_string_representation = str.str();
	}

};

template<typename T>
struct Node
{
	T value;
	int depth;
};

int find_minimum_flips_to_make_pancake_stack_presentable( const PancakeStack& pancake_stack ) {
	queue<Node<PancakeStack> > stacks;
	set<PancakeStack> found_stacks;
	Node<PancakeStack> first_node = { pancake_stack, 0 };
	stacks.push( first_node );

	while ( !stacks.empty() ) {
		auto node = stacks.front(); stacks.pop();
		auto& stack = node.value;
		if ( node.value.is_presentable() ) {
			return node.depth;
		}
		else {
			for ( int pancakes_to_flip = 1; pancakes_to_flip <= stack.size(); ++pancakes_to_flip ) {
				Node<PancakeStack> new_node = { stack.flip( pancakes_to_flip ), node.depth + 1 };
				if ( found_stacks.end() == found_stacks.find( new_node.value ) ) {
					stacks.push( new_node );
					found_stacks.insert( new_node.value );
				}

			}

		}

	}

}

int main( int argc, char const *argv[] ) {
	ifstream fin( "pancakes.in" );
	if ( fin ) {
		ofstream fout( "pancakes.out" );

		string line;
		getline( fin, line );
		while ( "0" != line ) {
			PancakeStack pancake_stack( line );
			int flip_count = find_minimum_flips_to_make_pancake_stack_presentable( pancake_stack );
			fout << flip_count << endl;

			getline( fin, line );
		}

		fout.close();
		fin.close();
	}
	else {
		cout << "Couldn't open file 'pancakes.in'" << endl;
	}
}
