// Jeff Ruhl
// Count the Surjections
#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
using namespace std;

template< typename T >
inline T calculate_factorial( T number ) {
	// skip 1 * n, redundant
	T result = (number == 1 ? 1 : 2);
	for ( int i = 3; i <= number; i++ ) {
		result *= i;
	}

	return result;
}

template< typename T >
inline T raise_to_power_of( T base, T exponent ) {
	T result = base;
	for ( int i = 1; i < exponent; i++ ) {
		result *= base;
	}

	return result;
}

template< typename T >
T count_permutations( int n, int i ) {
	if ( 0 == i || 0 == n || i == n ) {
		return 1;
	}

	return count_permutations<T>( n - 1, i - 1 ) + count_permutations<T>( n - 1, i );
}

template<typename T>
T count_surjections( int m, int n ) {
	T result = 0;

	if ( 1 == n ) {
		result = 1;
	}
	else if ( m < n ) {
		result = 0;
	}
	else if ( m == n ) {
		result = calculate_factorial<T>( m );
	}
	else {
		T n_raised_to_m = raise_to_power_of<T>( n, m );
		
		T sum = 0;
		for ( int i = 1; i < n; i++ ) {
			sum += count_permutations<T>( n, i ) * count_surjections<T>( m, i );
		}

		return n_raised_to_m - sum;
	}

	return result;
}

int main( int argc, char* argv[] ) {
	ifstream fin( "surjections.in" );
	if ( fin ) {
		ofstream fout( "surjections.out" );

		int m = 0, n = 0;
		fin >> m;
		fin >> n;
		while ( 0 != m && 0 != n ) {
			auto surjections_count = count_surjections<long long>( m, n );
			fout << "S(" << m << "," << n << ") = " << surjections_count << endl;

			fin >> m;
			fin >> n;
		}

		fout.close();
		fin.close();
	}
	else {
		cout << "Could not open file 'surjections.in'" << endl;
	}

}
