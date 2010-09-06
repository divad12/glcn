#include <iostream>
#include <algorithm>
#include "PlanetWars.h"
using namespace std;



// Variables starting with capitals denote constants.


//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

// Planet Wars turn-specific variables. We cache since some getters are time
// consuming O(n) ops.
class PwState {
 public:
  PwState(const PlanetWars& pw);
  vector <Fleet> MyFleets;
  int MyFleetsSize;
  vector <Planet> MyPlanets;
  int MyPlanetsSize;
};

PwState::PwState(const PlanetWars& pw) {
  MyFleets = pw.MyFleets();
  MyFleetsSize = MyFleets.size();
  MyPlanets = pw.MyPlanets();
  MyPlanetsSize = MyPlanets.size();
}


//------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------




//------------------------------------------------------------------------------
// Helper Functions
//------------------------------------------------------------------------------


// The DoTurn function is where your code goes. The PlanetWars object contains
// the state of the game, including information about all planets and fleets
// that currently exist. Inside this function, you issue orders using the
// pw.IssueOrder() function. For example, to send 10 ships from planet 3 to
// planet 8, you would say pw.IssueOrder(3, 8, 10).
//
// There is already a basic strategy in place here. You can use it as a
// starting point, or you can throw it out entirely and replace it with your
// own. Check out the tutorials and articles on the contest website at
// http://www.ai-contest.com/resources.
void DoTurn(const PlanetWars& planetWars) {
  // Cached pw
  PwState pw(planetWars);

  // (1) Do nothing if our fleets count exceeds our planets count
  if (pw.MyFleetsSize >= pw.MyPlanetsSize) {
  //if (pw.MyFleetsSize >= 1) {
    return;
  }

  // (2) Find my strongest planet.
  int source = -1;
  double source_score = -999999.0;
  int source_num_ships = 0;
  vector<Planet> my_planets = planetWars.MyPlanets();
  for (int i = 0; i < my_planets.size(); ++i) {
    const Planet& p = my_planets[i];
    double score = (double)p.NumShips();
    if (score > source_score) {
      source_score = score;
      source = p.PlanetID();
      source_num_ships = p.NumShips();
    }
  }
  // (3) Find the weakest enemy or neutral planet.
  int dest = -1;
  double dest_score = -999999.0;
  vector<Planet> not_my_planets = planetWars.NotMyPlanets();
  for (int i = 0; i < not_my_planets.size(); ++i) {
    const Planet& p = not_my_planets[i];
    double score = 1.0 / (1 + p.NumShips());
    if (score > dest_score) {
      dest_score = score;
      dest = p.PlanetID();
    }
  }
  // (4) Send half the ships from my strongest planet to the weakest
  // planet that I do not own.
  if (source >= 0 && dest >= 0) {
    int num_ships = source_num_ships / 2;
    planetWars.IssueOrder(source, dest, num_ships);
  }
}

// This is just the main game loop that takes care of communicating with the
// game engine for you. You don't have to understand or change the code below.
int main(int argc, char *argv[]) {
  string current_line;
  string map_data;
  while (true) {
    int c = cin.get();
    current_line += (char)c;
    if (c == '\n') {
      if (current_line.length() >= 2 && current_line.substr(0, 2) == "go") {
        PlanetWars pw(map_data);
        map_data = "";
        DoTurn(pw);
        pw.FinishTurn();
      } else {
        map_data += current_line;
      }
      current_line = "";
    }
  }
  return 0;
}
