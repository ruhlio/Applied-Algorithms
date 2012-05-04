// Jeff Ruhl
// Syncing Signals
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

#define YELLOWLIGHT_SECONDS 5
#define MAX_SECONDS 86400

enum LightState {
   LS_GREEN = 0,
   LS_YELLOW,
   LS_RED,
   LS_LENGTH
};

typedef int Time;
typedef vector<Time> LightLengthVector;
typedef vector<LightState> LightStateVector;

Time find_time_until_all_lights_green( const LightLengthVector& );
void pretty_print_time( ostream&, Time );

int main( int argc, char const *argv[] ) {
   ifstream fin("sync.in");
   if ( fin ) {
      ofstream fout("sync.out");

      int light_count = 0;
      fin >> light_count;
      while ( 0 != light_count ) {
         LightLengthVector light_lengths;
         for ( int i = 0; i < light_count; ++i ) {
            int light_length = 0;
            fin >> light_length;

            light_lengths.push_back( light_length );
         }

         int time_until_all_green = find_time_until_all_lights_green( light_lengths );
         pretty_print_time( fout, time_until_all_green );

         fin >> light_count;
      }

      fout.close();
      fin.close();
   }
   else {
      cout << "Unable to open file 'sync.in'" << endl;
   }

   return 0;
}

Time find_time_until_all_lights_green( const LightLengthVector& light_lengths ) {
   // the current state for each of the lights
   LightStateVector light_states( light_lengths.size(), LS_GREEN );
   // the length of each state for each of the lights
   vector<LightLengthVector> light_states_lengths;
   // the amount of time left until each of the lights changes
   LightLengthVector remaining_light_lengths;
   // the maximum amount of time any of the lights will stay green
   // used to check that all lights have cycled at least once
   Time max_light_length = *max_element( light_lengths.begin(), light_lengths.end() );

   auto cycle_light = [&]( int light ) {
		LightState new_light_state = static_cast<LightState>( (light_states[light] + 1) % (int)LS_LENGTH );
      light_states[light] = new_light_state;
      remaining_light_lengths[light] = light_states_lengths[light][new_light_state];
   };
   
   for ( auto light_length = light_lengths.begin(); light_length < light_lengths.end(); ++light_length) {
      LightLengthVector light_states_length(LS_LENGTH, 0);
      light_states_length[LS_RED] = *light_length;
      light_states_length[LS_YELLOW] = YELLOWLIGHT_SECONDS;
      light_states_length[LS_GREEN] = *light_length - YELLOWLIGHT_SECONDS;

      light_states_lengths.push_back( light_states_length );
      remaining_light_lengths.push_back( light_states_length[LS_RED] );
   }

   // TODO: change to GCF
   const Time time_increment = 1;
   Time elapsed_time;
   for ( elapsed_time = 1; elapsed_time < MAX_SECONDS; ++elapsed_time ) {
      int green_light_count = 0;

      for ( int light = 0; light < light_lengths.size(); ++light ) {

         remaining_light_lengths[light] -= time_increment;
         // time for light state has ended, change state
         if ( 0 == remaining_light_lengths[light] ) {
            cycle_light( light );
         }

         if ( LS_GREEN == light_states[light] ) {
            green_light_count++;
         }
      }

      // all lights are green for the second time, time found
      if ( light_lengths.size() == green_light_count && elapsed_time > max_light_length ) {
         break;
      }
   }


   return elapsed_time - YELLOWLIGHT_SECONDS;
}

void pretty_print_time( ostream& stream, Time time ) {
   if ( time >= MAX_SECONDS ) {
      stream << "??:??:??" << endl;
   }
   else {
      int hours = time / 3600;
      int minutes = (time % 3600 ) / 60;
      int seconds = (time % 360 % 60);

      stream << setfill('0') << setw(2) << hours << ':' << setw(2) << minutes << ':' 
         << setw(2) << seconds << endl;
   }
}
