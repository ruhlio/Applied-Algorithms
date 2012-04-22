// Jeff Ruhl
// Lampyridae Teleportae
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <exception>
#include <cmath>
using namespace std;

class Exception : std::exception
{
	const char* _what;

public:
	Exception( const char* what ) : _what( what ) {}

	const char* what() const throw() { return _what; }

};

template <typename T>
struct Option
{
public:
	Option() {}

	bool is_empty() { return _is_empty; }

	T& get() { 
		if ( _is_empty ) {
			throw Exception("No value");
		}
		return _val; 

	}

	T& operator*() {
		return get();
	}

protected:
	bool _is_empty;
	T _val;

};

template <typename T>
struct Some : public Option<T>
{
	Some( const T& val ) {
		this->_is_empty = false;
		this->_val = val;
	}

};

template <typename T>
struct None : public Option<T>
{
	None() {
		this->_is_empty = true;
	}

};

struct Vector
{
	float x, y;

	bool operator==( const Vector& other ) const {
		return (this->x == other.x && this->y == other.y);
	}
	bool operator!=( const Vector& other ) const {
		return !operator==( other );
	}

	Vector operator+( const Vector& other ) const {
		Vector result = {this->x + other.x, this->y + other.y};
		return result;
	}
	Vector operator-( const Vector& other ) const {
		Vector result = {this->x - other.x, this->y - other.y};
		return result;
	}
	Vector operator*( float scalar ) const {
		Vector result = {this->x * scalar, this->y * scalar};
		return result;
	}

	void normalize() {
		float length = get_length();
		x /= length;
		y /= length;
	}

	float get_length() const {
		return sqrt( x * x + y * y );
	}

};

istream& operator>>( istream &in, Vector& vector ) {
	in >> vector.x;
	in >> vector.y;

	return in;
}

typedef vector<Vector> Vectors;

Option<Vector> catch_firefly( float, Vector, const Vectors& );

int main( int argc, char const *argv[] ) {
	ifstream fin("firefly.in");
	if ( fin ) {
		ofstream fout("firefly.out");

		float max_shoe_distance = 0;
		Vector chaser_start_position = {0, 0};
		Vectors firefly_positions;
		Vector firefly_position = {0, 0};
		Vector end_of_input = {-1, -1};
		int firefly_count = 1;

		fin >> max_shoe_distance;
		while ( 0 != max_shoe_distance ) {
			fin >> chaser_start_position;
			fin >> firefly_position;
			while ( firefly_position != end_of_input ) {
				firefly_positions.push_back( firefly_position );
				fin >> firefly_position;
			}

			auto result = catch_firefly( max_shoe_distance, chaser_start_position, firefly_positions );
			if ( result.is_empty() ) {
				fout << "Firefly " << firefly_count << " not caught" << endl;
			}
			else {
				Vector firefly_pos = *result;
				fout << "Firefly " << firefly_count << " caught at (" << firefly_pos.x << "," << firefly_pos.y << ")" << endl;
			}

			firefly_count++;
			firefly_positions.clear();
			fin >> max_shoe_distance;
		}

		fout.close();
		fin.close();
	}
	else {
		cout << "Could not open 'firefly.in'" << endl;
	}

}

Option<Vector> catch_firefly( float max_shoe_distance, Vector chaser_position, const Vectors& firefly_positions ) {
	Option<Vector> result = None<Vector>();

	for ( auto firefly_pos = firefly_positions.begin(); firefly_pos != firefly_positions.end(); ++firefly_pos ) {
		Vector space_from_firefly = *firefly_pos - chaser_position;
		if ( space_from_firefly.get_length() <= max_shoe_distance + 1 ) {
			result = Some<Vector>( *firefly_pos );
			break;
		}
		else {
			space_from_firefly.normalize();
			chaser_position = chaser_position + space_from_firefly * max_shoe_distance;
		}
	}

	return result;
}
