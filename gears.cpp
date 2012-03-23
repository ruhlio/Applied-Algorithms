// Jeff Ruhl
// Gearing Up

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <exception>
#include <functional>
#include <algorithm>
#include <cassert>
using namespace std;

#define GEARBOX_WIDTH 32
#define GEAR_NOT_MOVING_MESSAGE "STILL"

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
public:
	Option() {}

	bool is_empty() { return _is_empty; }
	T& get() { 
		if ( _is_empty ) {
			throw Exception("No value");
		}
		return _val; 
	}

protected:
	bool _is_empty;
	T _val;

};

template <typename T>
class Some : public Option<T>
{
public:
	Some( T& val ) {
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

template< typename T>
class Array
{
public:
	template<typename TElement>
	class Iterator
	{
	public:
		Iterator() {
			_array = nullptr;
			_row = 0;
			_col = 0;
		}

		Iterator( Array<TElement>* array, int row, int col )
			: _array( array )
		{
			_row = row;
			_col = col;
		}

		Iterator<TElement>& operator=( const Iterator<TElement>& other ) {
			if ( this != &other ) {
				_array = other._array;
				_row = other._row;
				_col = other._col;
			}

			return *this;
		}

		int row() const {
			return _row;
		}

		int col() const {
			return _col;
		}

		TElement& operator*() const {
			return _array->get( _row, _col );
		}

		TElement* operator->() const {
			return &_array->get( _row, _col );
		}

		Iterator<TElement>& operator++() {
			if ( _col == _array->width() - 1 ) {
				_col = 0;
				_row++;
			}
			else {
				_col++;
			}

			return *this;
		}

		inline bool operator==( const Iterator<TElement>& rhs ) {
			return (_array == rhs._array && _row == rhs._row && _col == rhs._col);
		}

		inline bool operator!=( const Iterator<TElement>& rhs ) {
			return !operator==( rhs );
		}

		Option< Iterator<TElement> > left() const {
			if ( 0 == _col ) {
				return None< Iterator<T> >();
			}

			return Some< Iterator<TElement> >( Iterator<TElement>( _array, _row, _col - 1 ) );
		}

		Option< Iterator<TElement> > right() const {
			if ( _array->width() - 1 == _col ) {
				return None< Iterator<TElement> >();
			}

			return Some< Iterator<TElement> >( Iterator<TElement>( _array, _row, _col + 1 ) );
		}

		Option< Iterator<TElement> > up() const {
			if ( 0 == _row ) {
				return None< Iterator<TElement> >();
			}

			return Some< Iterator<TElement> >( Iterator<TElement>( _array, _row - 1, _col ) );
		}

		Option< Iterator<TElement> > down() const {
			if ( _array->height() - 1 == _row ) {
				return None< Iterator<TElement> >();
			}

			return Some< Iterator<TElement> >( Iterator<TElement>( _array, _row + 1, _col ) );
		}

	private:
		Array<TElement>* _array;
		int _row;
		int _col;

	};

	typedef T ElementType;
	typedef Iterator<T> IteratorType;

	Array( int row_count, int column_count ) {
		_array = new T[(row_count + 1) * column_count];

		_row_count = row_count;
		_column_count = column_count;
	}

	virtual ~Array() {
		delete[] _array;
		_array = nullptr;
	}

	//Array<T>& operator=( const Array<T>& other ) {
	//	if ( this != &other ) {
	//		_array = other._array;
	//		_row_count = other._row_count;
	//		_column_count = other._column_count;
	//	}

	//	return *this;
	//}

	T& get( int row, int col ) const {
		if ( row >= _row_count ) {
			throw Exception("row out of bounds");
		}
		if ( col >= _column_count) {
			throw Exception("col out of bounds");
		}

		return _array[ (row * _column_count) + col ];
	};
	inline T& operator()( int row, int col ) const {
		return get( row, col );
	}

	int width() const {
		return _column_count;
	}

	int height() const {
		return _row_count;
	}

	IteratorType begin() {
		return Iterator<T>( this, 0, 0 );
	}

	IteratorType end() {
		return Iterator<T>( this, _row_count, 0 );
	}

	IteratorType last() {
		return Iterator<T>( this, _row_count - 1, _column_count - 1 );
	}

private:
	T* _array;
	int _row_count;
	int _column_count;

};

template< typename T >
struct Node
{
	T value;
	bool is_marked;
};

template< typename T >
struct Fraction
{
	T numerator;
	T denominator;

	inline bool operator==( const Fraction& rhs ) {
		return (numerator == rhs.numerator && denominator == rhs.denominator);
	}
	inline bool operator!=( const Fraction& rhs ) {
		return !operator==( rhs );
	}

	Fraction operator*( const Fraction& rhs ) {
		Fraction fract = { numerator * rhs.numerator, denominator * rhs.denominator };
		assert( numerator >= 0 );
		fract.simplify();
		return fract;
	}

	T gcd() const {
		T dividend, divisor, remainder;
		if ( numerator > denominator ) {
			dividend = numerator;
			divisor = denominator;
		}
		else {
			dividend = denominator;
			divisor = numerator;
		}

		while ( 0 != (remainder = dividend % divisor) ) {
			dividend = divisor;
			divisor = remainder;
		}

		return divisor;
	}

	void simplify() {
		if ( 0 == numerator || 1 == denominator ) {
			return;
		}

		T div = gcd();
		if ( 1 < div ) {
			numerator /= div;
			denominator /= div;
		}
	}

	string to_s() const {
		stringstream stream( stringstream::in | stringstream::out );

		stream << numerator;
		if ( 1 != denominator ) {
			stream << "/" << denominator;
		}

		return stream.str();
	}

};

struct GearTurn
{
	int gear;
	Fraction<long long> rpm;
	bool is_clockwise;

	GearTurn turn( int moved_gear ) {
		GearTurn gear_turn;
		int width = gear + moved_gear;

		// gears can't physically fit within provided space
		if ( GEARBOX_WIDTH < width ) {
			throw Exception( GEAR_NOT_MOVING_MESSAGE );
		}
		// gears don't touch or gear isn't moving
		else if ( GEARBOX_WIDTH > width || 0 == rpm.numerator ) {
			gear_turn.gear = moved_gear;
			gear_turn.rpm.numerator = 0;
			gear_turn.rpm.denominator = 1;
			gear_turn.is_clockwise = true;
		}
		else {
			gear_turn.gear = moved_gear;
			Fraction<long long> power_ratio = { gear, moved_gear };
			gear_turn.rpm = rpm * power_ratio;
			gear_turn.is_clockwise = !is_clockwise;
		}

		return gear_turn;
	}


};

enum FromDirection {
	FROM_NONE,
	FROM_LEFT,
	FROM_RIGHT,
	FROM_ABOVE,
	FROM_BELOW,
};

typedef Array< Node<int> > NodeArray;

template< typename T>
void depth_first_search( typename Array< Node<T> >::IteratorType& iter, FromDirection from,
	const std::function<bool(T, int, int, int)>& should_node_be_marked, 
	const std::function<void(int, int, int)>& on_node_already_marked,
	const std::function<bool(int)>& check_value ) 
{
	if ( check_value( iter->value ) ) {
		if ( !iter->is_marked ) {
			iter->is_marked = should_node_be_marked( iter->value, from, iter.row(), iter.col() );

			if ( iter->is_marked ) {
				assert( 0 <= iter->value );

				auto adjacent = iter.up();
				if ( !adjacent.is_empty() && !adjacent.get()->is_marked ) {
					depth_first_search<T>( adjacent.get(), FROM_BELOW, should_node_be_marked, on_node_already_marked, check_value );
				}

				adjacent = iter.right();
				if ( !adjacent.is_empty() ) {
					if ( adjacent.get()->is_marked ) {
						on_node_already_marked( FROM_LEFT, adjacent.get().row(), adjacent.get().col() );
					}
					else {
						depth_first_search<T>( adjacent.get(), FROM_LEFT, should_node_be_marked, on_node_already_marked, check_value );
					}
				}

				adjacent = iter.down();
				if ( !adjacent.is_empty() && !adjacent.get()->is_marked ) {
					if ( adjacent.get()->is_marked ) {
						on_node_already_marked( FROM_ABOVE, adjacent.get().row(), adjacent.get().col() );
					}
					else {
						depth_first_search<T>( adjacent.get(), FROM_ABOVE, should_node_be_marked, on_node_already_marked, check_value );
					}
				}

				adjacent = iter.left();
				if ( !adjacent.is_empty() && !adjacent.get()->is_marked  ) {
					depth_first_search<T>( adjacent.get(), FROM_RIGHT, should_node_be_marked, on_node_already_marked, check_value );
				}
			} // if ( iter->is_marked )

		} // if ( !iter->is_marked )
	} // if ( check_value( iter->value ) )

}

GearTurn turn_gearbox( NodeArray& gearbox, GearTurn input ) {
	Array<GearTurn> gear_turns( gearbox.height(), gearbox.width() );
	GearTurn& output = *gear_turns.last();
	// initiate output as not moving
	output.rpm.numerator = 0;
	output.rpm.denominator = 1;

	auto move_gear = [&]( int gear, int from, int row, int col ) -> bool {
		GearTurn gear_turn = {1, {1, 1}, false};
		switch ( from ) {
		// first gear turned, modified directly by the input
		case FROM_NONE:
			gear_turn = input;
			gear_turn.gear = gear;
			gear_turns( row, col ) = gear_turn;
			break;
		// turns from left or right are executed by axel, not gear
		case FROM_LEFT:
			gear_turn = gear_turns( row, col - 1 );
			gear_turn.gear = gear;
			gear_turns( row, col ) = gear_turn;
			break;
		case FROM_RIGHT:
			gear_turn = gear_turns( row, col + 1 );
			gear_turn.gear = gear;
			gear_turns( row, col ) = gear_turn;
			break;
		// turns from above or below are executed by the adjacent gear
		case FROM_ABOVE:
			gear_turn = gear_turns( row - 1, col ).turn( gear );
			gear_turns( row, col ) = gear_turn;
			break;
		case FROM_BELOW:
			gear_turn = gear_turns( row + 1, col ).turn( gear );
			gear_turns( row, col ) = gear_turn;
			break;
		}

		bool should_mark = true;
		// if gear was not moved then don't mark it
		if ( 0 == gear_turn.rpm.numerator ) {
			should_mark = false;
		}
		return should_mark;
	};
	auto gear_already_moved = [&]( int from, int row, int col ) {
		GearTurn gear_turn = gear_turns( row, col );
		GearTurn from_gear_turn;

		switch ( from ) {
		case FROM_LEFT:
			from_gear_turn = gear_turns( row, col - 1 );
			break;
		case FROM_RIGHT:
			from_gear_turn = gear_turns( row, col + 1 );
			break;
		case FROM_ABOVE:
			from_gear_turn = gear_turns( row - 1, col );
			break;
		case FROM_BELOW:
			from_gear_turn = gear_turns( row + 1, col );
			break;
		}

		if ( from_gear_turn.rpm != gear_turn.rpm ) {
			throw Exception(GEAR_NOT_MOVING_MESSAGE);
		}
	};
	auto is_valid_gear = [&]( int gear ) -> bool {
		return gear != -1;
	};

	depth_first_search<int>( gearbox.begin(), FROM_NONE, move_gear, gear_already_moved, is_valid_gear );

	return output;
}

int main( int argc, char *argv[] ) {
	ifstream fin("gears.in");
	if ( fin ) {
		ofstream fout("gears.out");

		int gearbox_count = 0;
		fin >> gearbox_count;
		for ( int i = 0; i < gearbox_count; i++ ) {
			int col_count, axel_count;
			fin >> col_count;
			fin >> axel_count;
			
			NodeArray gearbox( axel_count, col_count );

			// read gearbox in
			for ( auto iter = gearbox.begin(); iter != gearbox.end(); ++iter ) {
				int value = 0;
				fin >> value;

				NodeArray::ElementType node = { value, false };
				*iter = node;
			}

			GearTurn input = { 0, {1, 1}, true };
			try {
				auto output = turn_gearbox( gearbox, input );
				if ( 0 == output.rpm.numerator ) {
					throw Exception(GEAR_NOT_MOVING_MESSAGE);
				}

				fout << (output.is_clockwise ? "CW" : "CCW") << " " << output.rpm.to_s() << endl;
			}
			catch ( const exception& e ) {
				fout << e.what() << endl;
			}

		}

		fout.close();
		fin.close();
	}
	else {
		cout << "Couldn't open 'gears.in'" << endl;
	}

	return 0;
}
