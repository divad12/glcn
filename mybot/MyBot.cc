#include <iostream>
#include <algorithm>
#include <fstream>
#include "PlanetWars.h"
using namespace std;

#define debug(x) fout << x << endl

// Variables starting with capitals denote constants.
// TODO: take argument specifying whether we want debug mode

//------------------------------------------------------------------------------
// Prototypes
//------------------------------------------------------------------------------
void outputIntVector(vector<int>);


//------------------------------------------------------------------------------
// Global Variables
//------------------------------------------------------------------------------
ofstream fout("debug.txt");
int planetWarsTurn = 0;


//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

// Planet Wars turn-specific variables. We cache since some getters are time
// consuming O(n) ops.
class PwState {
 public:
  PwState(const PlanetWars& pw);
  vector <Fleet> MyFleets;
  vector <Fleet> Fleets;
  int MyFleetsSize;
  vector <Planet> MyPlanets;
  vector <Planet> Planets;
  int MyPlanetsSize;
  int NumPlanets;
  // Adjusted planet ships size taking into account incoming fleets. Negatives
  // indicate me owning it.
  vector <int> planetShipsWithFleets;
 private:
  vector<int> fleetsAdjustPlanetShips();
};

PwState::PwState(const PlanetWars& pw) {
  Fleets = pw.Fleets();
  MyFleets = pw.MyFleets();
  MyFleetsSize = MyFleets.size();
  Planets = pw.Planets();
  MyPlanets = pw.MyPlanets();
  MyPlanetsSize = MyPlanets.size();
  NumPlanets = pw.NumPlanets();
  planetShipsWithFleets = fleetsAdjustPlanetShips();
}

// TODO actually do proper simulation: turn-by-turn simulation, growth rates,
// etc.
vector<int> PwState::fleetsAdjustPlanetShips() {
  //vector <vector<Fleet*> > planetAttackers;
  //planetAttackers.resize(NumPlanets + 10);
  vector <int> enemiesToPlanet;
  enemiesToPlanet.resize(NumPlanets + 2, 0);
  vector <int> meToPlanet;
  meToPlanet.resize(NumPlanets + 2, 0);
  vector <int> ret;
  ret.resize(NumPlanets + 2, 0);

  // Distribute fleets to their respective targets
  for (vector<Fleet>::iterator it = Fleets.begin(); it != Fleets.end(); ++it) {
    if (it->Owner() == 1) {
      meToPlanet[it->DestinationPlanet()] += it->NumShips();
    } else {
      enemiesToPlanet[it->DestinationPlanet()] += it->NumShips();
    }
  }

  // Now calculate adjusted num ships for each planet
  for (vector<Planet>::iterator it = Planets.begin(); it != Planets.end();
      ++it) {
    int id = it->PlanetID();
    int ops = enemiesToPlanet[id] + (it->Owner() > 1 ? it->NumShips() : 0);
    int mes = meToPlanet[id] + (it->Owner() == 1 ? it->NumShips() : 0);
    int planet = (it->Owner() == 0 ? it->NumShips() : 0);
    fout << " planet id=" << id << " ops=" << ops << " mes=" << mes << " planet=" << planet << endl;
    if (ops + mes <= planet) {
      ret[id] = planet - ops - mes;
    } else if (ops == mes) {
      ret[id] = 0;
    } else {
      if (it->Owner() == 0) {
        ret[id] = ops + mes - planet;
        ret[id] *= (mes > ops ? -1 : 1);
      } else {
        ret[id] = ops - mes;
      }
    }
  }

  outputIntVector(ret);
  return ret;
}


//------------------------------------------------------------------------------
// Functions
//------------------------------------------------------------------------------


// DEBUG FUNCTIONS --------------
void outputPlanet(const Planet& p) {
  debug("id=" << p.PlanetID() << " own=" << p.Owner() << " #ships=" <<
      p.NumShips() << " GRate=" << p.GrowthRate());
}

void outputIntVector(vector<int> v) {
#ifdef debug
  for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    fout << *it << " ";
  }
  fout << endl;
#endif
}



// Initialize a PlanetWars game
void initGame() {
}

// Scores a planet on how desirable it is. The higher, the more desirable.
// Factors taken into account:
// - num ships currently on it TODO tweak multiplier
// - growth rate TODO tweak multiplier
// TODO: this may be a bad formula. addition instead of multiplication, maybe?
// TODO: take into account incoming fleets
// TODO: take into account distance
double scorePlanet(const PwState& pw, const Planet& planet) {
  double numerator = planet.GrowthRate();// * planet.GrowthRate();
  double denominator = pw.planetShipsWithFleets[planet.PlanetID()];
  // TODO epsilon comparison
  denominator = denominator == 0 ? 1.0 : denominator;

  return numerator / denominator;
}


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
  debug("\n--- Turn " << ++planetWarsTurn);

  // Cached pw
  PwState pw(planetWars);

  // (1) Do nothing if our fleets count exceeds
  if (pw.MyFleetsSize >= pw.MyPlanetsSize * 2) {
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
  vector<Planet> not_my_planets = pw.Planets;
  for (int i = 0; i < not_my_planets.size(); ++i) {
    const Planet& p = not_my_planets[i];
    double score = scorePlanet(pw, p);
    if (score > dest_score) {
      dest_score = score;
      dest = p.PlanetID();
    }
  }

  if (dest != -1) {
    outputPlanet(planetWars.GetPlanet(dest));
    debug("  destScore=" << dest_score);
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
  initGame();
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
