// Jeff Ruhl
// Broken Clock
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <set>
#include <vector>
#include <bitset>
#include <algorithm>
#include <iterator>
#include <cmath>
using namespace std;

typedef set<int> LedComponents;
typedef vector<LedComponents> ClockDigits;
typedef set<set<int> > AmbiguousDigitSets;
typedef unsigned long ulong;

AmbiguousDigitSets find_ambiguous_digits( const LedComponents& );
template<typename T> string set_to_string( const set<T>& );
template<typename T> ulong set_to_ulong( const set<T> & );

//ClockDigits clock_digits = {
//  {1,2,3,5,6,7},
//  {3,6},
//  {1,3,4,5,7},
//  {1,3,4,6,7},
//  {2,3,4,6},
//  {1,2,4,6,7},
//  {1,2,4,5,6,7},
//  {1,3,6},
//  {1,2,3,4,5,6,7},
//  {1,2,3,4,6}
//};
 ClockDigits clock_digits;
 void init_clock_digits() {
    int zero[] = {1,2,3,5,6,7};
    clock_digits.push_back( ClockDigits::value_type( zero, zero+6 ) );
    int one[] = {3,6};
    clock_digits.push_back( ClockDigits::value_type( one, one+2 ) );
    int two[] = {1,3,4,5,7};
    clock_digits.push_back( ClockDigits::value_type( two, two+5 ) );
    int three[] = {1,3,4,6,7};
    clock_digits.push_back( ClockDigits::value_type( three, three+5 ) );
    int four[] = {2,3,4,6};
    clock_digits.push_back( ClockDigits::value_type( four, four+4 ) );
    int five[] = {1,2,4,6,7};
    clock_digits.push_back( ClockDigits::value_type( five, five+5 ) );
	 int six[] = {1,2,4,5,6,7};
	 clock_digits.push_back( ClockDigits::value_type( six, six+6 ) );
    int seven[] = {1,3,6};
    clock_digits.push_back( ClockDigits::value_type( seven, seven+3 ) );
    int eight[] = {1,2,3,4,5,6,7};
    clock_digits.push_back( ClockDigits::value_type( eight, eight+7 ) );
    int nine[] = {1,2,3,4,6};
    clock_digits.push_back( ClockDigits::value_type( nine, nine+5 ) );
 }

int main( int argc, char const *argv[] ) {
   int result = 0;
   init_clock_digits();

   ifstream fin("led.in");
   if ( fin ) {
      ofstream fout("led.out");

      string line;
      getline( fin, line );
      while ( line != "0" ) {
         stringstream stream( line );
         int led_component;
         LedComponents led_components;

         stream >> led_component;
         while ( 0 != led_component ) {
            led_components.insert( led_component );
            stream >> led_component;
         }

         auto ambiguous_sets = find_ambiguous_digits( led_components );
         fout << "Case <" << set_to_string( led_components ) << ">:";
			int count = 1;
         for ( auto ambiguous_set = ambiguous_sets.begin(); ambiguous_set != ambiguous_sets.end(); ++ambiguous_set ) {
				if ( count == 1 ) {
					fout << ' ';
				}

				fout << '{' << set_to_string( *ambiguous_set ) << '}';
				if ( count != ambiguous_sets.size() ) {
					fout << ' ';
					count++;
				}
         }
			fout << endl;

         getline( fin, line );
      }

      fout.close();
      fin.close();
   }
   else {
      result = 1;
      cout << "File 'led.in' could not be opened" << endl;
   }

   return result;
}

AmbiguousDigitSets find_ambiguous_digits( const LedComponents& broken_led_components ) {
   AmbiguousDigitSets results;
   // multimap holds multiple numbers to be displayed under a single led layout
   multimap<ulong, int> broken_clock_digits;

   int number = 0;
   for ( auto digit = clock_digits.begin(); digit != clock_digits.end(); ++digit ) {
      LedComponents broken_component;
      set_difference( digit->begin(), digit->end(), broken_led_components.begin(), broken_led_components.end(),
         inserter( broken_component, broken_component.end() ) );
      broken_clock_digits.insert( pair<ulong, int>( 
         set_to_ulong( broken_component ), number ) );
      ++number;
   }

   ulong prev_key = -1;
   for ( auto digit_number = broken_clock_digits.begin(); digit_number != broken_clock_digits.end(); ++digit_number ) {
      // multimap iterator passes through all stored key/value pairs,
      // all we need is the keys
      if ( digit_number->first != prev_key ) {
         if ( 1 < broken_clock_digits.count( digit_number->first ) ) {
            AmbiguousDigitSets::value_type ambiguous_digits;
            auto range = broken_clock_digits.equal_range( digit_number->first );
            for ( auto iter = range.first; iter != range.second; iter++ ) {
               ambiguous_digits.insert( iter->second );
            }

            results.insert( ambiguous_digits );
         }
      }
      prev_key = digit_number->first;
   }

   return results;
}

template<typename T>
string set_to_string( const set<T>& s ) {
   stringstream result( stringstream::in | stringstream::out );

   for ( auto elem = s.begin(); elem != s.end(); ++elem ) {
      result << *elem << ',';
   }

   string str = result.str();
   return str.erase( str.size() - 1, 1 );
}

template<typename T>
ulong set_to_ulong( const set<T> & s ) {
   ulong result = 0;
   for ( auto elem = s.begin(); elem != s.end(); ++elem ) {
      result += pow( 2, *elem );
   }

   return result;
}
