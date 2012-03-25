// Jeff Ruhl
// Broken Clock
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <set>
#include <bitset>
using namespace std;

typedef bitset<8> LedComponents;
typedef set<set<int> > AmbiguousDigitSets;

AmbiguousDigitSets find_amiguous_digits( const LedComponents& );

int main( int argc, char const *argv[] ) {
   int result = 0;

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
            led_components.set( led_component );
            stream >> led_component;
         }
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

AmbiguousDigitSets find_amiguous_digits( const LedComponents& led_components ) {
   AmbiguousDigitSets results;



   return results;
}
