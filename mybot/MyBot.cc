#include <cassert>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <cstring>
#include <cmath>
#include <iomanip>
#include <set>
#include "PlanetWars.h"
using namespace std;

#define debug(x) if (debugMode) fout << x

// Variables starting with capitals denote constants.

//------------------------------------------------------------------------------
// Globals
//------------------------------------------------------------------------------

// Constants ---------------------
const int MIN_PLANET_SHIPS = 20; // Minimum # of ships that must remain on planet



// Variables ---------------------
ofstream fout("debug2.txt");
int planetWarsTurn = 0;
PlanetWars* gPlanetWars = NULL;
bool debugMode = false;


//------------------------------------------------------------------------------
// Classes
//------------------------------------------------------------------------------

// Comparison object to order fleets by arrival time
struct compareFleets {
  bool operator()(Fleet* a, Fleet* b) {
    return a->TurnsRemaining() < b->TurnsRemaining();
  }
};

// A future planet. only a few properties will change. the id is stored as the
// index of this planet within its data structure
struct FuturePlanet {
  int owner;
  int numShips;
  FuturePlanet(int Owner, int NumShips) : owner(Owner), numShips(NumShips) {}
  FuturePlanet() : owner(-1), numShips(-1) {}
};

// Should actually be multiset<const Fleet&, ... (or even <Fleet&), but got compile errors. why?
typedef multiset<Fleet*, compareFleets> IncomingFleets;


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
  vector <FuturePlanet> planetsAdjusted;
  vector <IncomingFleets> fleetsToPlanet;
  vector <int> shipsLeaving;
  FuturePlanet adjustPlanetShips(const IncomingFleets&, const Planet&);
 private:
  void factorInAllFleets();
};

//------------------------------------------------------------------------------
// Prototypes
//------------------------------------------------------------------------------
void outputIntVector(vector<int>);


//------------------------------------------------------------------------------
// DEBUG FUNCTIONS
//------------------------------------------------------------------------------
void outputPlanet(const Planet& p) {
  debug("id=" << p.PlanetID() << " own=" << p.Owner() << " #ships=" <<
      p.NumShips() << " GRate=" << p.GrowthRate() << endl);
}

void printIndices(int size) {
  if (!debugMode) {
    return;
  }

  for (int i = 0; i < size; ++i) {
    debug ( setw(4) << left << i);
  }
  debug (" <-- indices" << endl);
}

void outputIntVector(vector<int> v) {
  if (!debugMode) {
    return;
  }

  // print values
  for (vector<int>::iterator it = v.begin(); it != v.end(); ++it) {
    debug ( setw(4) << left << *it);
  }
  debug ( endl);
}

void outputFuturePlanets(vector <FuturePlanet> v) {
  if (!debugMode) {
    return;
  }
  int size = v.size();
  printIndices(size);

  for (int i = 0; i < v.size(); ++i) {
    debug(setw(4) << v[i].numShips);
  }

  debug(" <-- #ships " << endl);

  for (int i = 0; i < v.size(); ++i) {
    debug(setw(4) << v[i].owner);
  }
  debug(" <-- owner" << endl);
}


//------------------------------------------------------------------------------
// CLASS METHODS
//------------------------------------------------------------------------------
PwState::PwState(const PlanetWars& pw) {
  Fleets = pw.Fleets();
  MyFleets = pw.MyFleets();
  MyFleetsSize = MyFleets.size();
  Planets = pw.Planets();
  MyPlanets = pw.MyPlanets();
  MyPlanetsSize = MyPlanets.size();
  NumPlanets = pw.NumPlanets();
  fleetsToPlanet.resize(NumPlanets + 2);
  shipsLeaving.resize(NumPlanets + 2, 0);
  planetsAdjusted.resize(NumPlanets + 2);
  factorInAllFleets();
}

void PwState::factorInAllFleets() {
  // Distribute fleets to their respective target planets
  for (vector<Fleet>::iterator it = Fleets.begin(); it != Fleets.end(); ++it) {
    fleetsToPlanet[it->DestinationPlanet()].insert(&(*it));
  }

  // Now calculate adjusted num ships for each planet
  for (vector<Planet>::iterator it = Planets.begin(); it != Planets.end();
      ++it) {
    int pid = it->PlanetID();
    debug(setw(4) << left << it->NumShips());
    planetsAdjusted[pid] = adjustPlanetShips(fleetsToPlanet[pid], *it);
  }

  debug(" <-- #ships currently" << endl);
  outputFuturePlanets(planetsAdjusted);
}

//------------------------------------------------------------------------------
// Game Functions
//------------------------------------------------------------------------------
// Initialize a PlanetWars game
void initGame() {
}


// Resolves a three-way battle between neutrals, opponent, and my fleets when
// they all arrive at the same turn. Uses the algorithm outlined at
//    http://code.google.com/p/ai-contest/issues/detail?id=64
// Returns a pointer to the victor
int* resolveBattle(int& a, int& b, int& c, int* oldVictor) {
  // Find the strongest and second strongest forces.
  // Another way of finding middle: min(max(a, b), min(max(a, c), max(b, c)))
  int *strongest, *middle;
  if (a >= b && a >= c) {
    strongest = &a;
    middle = b > c ? &b : &c;
  } else if (b >= c) {
    strongest = &b;
    middle = a > c ? &a : &c;
  } else {
    strongest = &c;
    middle = a > b ? &a : &b;
  }

  int victors = *strongest - *middle;
  a = b = c = 0;
  *strongest = victors;
  return *strongest == 0 ? oldVictor : strongest;
}

// TODO: refactor a bit to take advantage of the fact taht this fucntion's a
//    PwState method
// Adjusts a planet's ship count when we send a new fleet to it
// @param fleets fleets going into the planet
// @param planet the planet of concern
// @return number of ships left on planet at the end of it all. positive means ops/neutrals, negative means me
FuturePlanet PwState::adjustPlanetShips(const IncomingFleets& fleets,
    const Planet& planet) {
  // THIS is actually horribly named. prevFleet should be current fleet, and
  // local variable f should be future fleet
  IncomingFleets::iterator prevFleet = (IncomingFleets::iterator)NULL;
  int ops = (planet.Owner() > 1 ? planet.NumShips() : 0);
  int mes = (planet.Owner() == 1 ?
      planet.NumShips() - shipsLeaving[planet.PlanetID()] : 0);
  int neutrals = (planet.Owner() == 0 ? planet.NumShips() : 0);
  int* planetOwner = (planet.Owner() == 0 ? &neutrals :
      (planet.Owner() == 1 ? &mes : &ops));
  int lastBattleTurn = 0;

  // This loop is looping onto the sentinel element as well
  for (IncomingFleets::iterator it = fleets.begin(); it != fleets.end(); ) {
    Fleet* f = *it;
    // Merge all fleets arriving at the planet this turn
    do {
      ops += f->Owner() > 1 ? f->NumShips() : 0;
      mes += f->Owner() == 1 ? f->NumShips() : 0;
      prevFleet = it++;
      f = *it;
    } while (it != fleets.end() &&
        f->TurnsRemaining() == (**prevFleet).TurnsRemaining());

    // Grow the planet
    if (planetOwner != &neutrals) {
      int turnsElapsed = (**prevFleet).TurnsRemaining() - lastBattleTurn;
      *planetOwner += turnsElapsed * planet.GrowthRate();
    }

    // Battle!
    //debug ( " resolving battle: ops =" << ops << " mes="<<mes<<"neus="<<neutrals<<endl);
    planetOwner = resolveBattle(ops, mes, neutrals, planetOwner);
    if (it != fleets.end()) {
      lastBattleTurn = (**prevFleet).TurnsRemaining();
    }
  }

  // XXX HACK: so that we can give socres when planet is empty but still owned
  *planetOwner = max(1, *planetOwner);

  // Now construct a FuturePlanet object representing this planet in the future
  int ownerId = (planetOwner == &neutrals ? 0 : (planetOwner == &mes ? 1 : 2));
  int numShips = (ownerId == 1 ? -(*planetOwner) : *planetOwner);
  return FuturePlanet(ownerId, numShips);
}

void issueOrder(PwState& pw, int source, int target, int numShips) {
  gPlanetWars->IssueOrder(source, target, numShips);
  debug(source << " attacks " << target << " with " << numShips << " out of " << gPlanetWars->GetPlanet(source).NumShips() << endl);

  // Now adjust our planet counters
  int dist = gPlanetWars->Distance(source, target);
  Fleet* f = new Fleet(1, numShips, source, target, dist, dist);

  pw.fleetsToPlanet[target].insert(f);
  pw.planetsAdjusted[target] = pw.adjustPlanetShips(pw.fleetsToPlanet[target],
      gPlanetWars->GetPlanet(target));

  pw.shipsLeaving[source] += numShips;
  pw.planetsAdjusted[source] = pw.adjustPlanetShips(pw.fleetsToPlanet[source],
      gPlanetWars->GetPlanet(source));
}


// Scores a planet on how desirable it is. The higher, the more desirable.
// Factors taken into account:
// - num ships currently on it
// - growth rate
// - distance
// TODO: this may be a bad formula. addition instead of multiplication, maybe?
// TODO: fiddle with all coefficients etc.
double scorePlanet(const PwState& pw, const Planet& source, const Planet&
    target) {
  int targetId = target.PlanetID();

  // If the planet already (or will) belong to us, don't consider reinforcing it
  // TODO: experiment with changing the '0' numShips constant to, say, -20
  if (pw.planetsAdjusted[targetId].owner == 1) {
    return pw.planetsAdjusted[targetId].numShips;
  }

  double numerator = target.GrowthRate();// * target.GrowthRate();
  double denominator = pw.planetsAdjusted[targetId].numShips
      + gPlanetWars->Distance(source.PlanetID(), targetId);
  // TODO epsilon comparison
  denominator = denominator == 0 ? 1.0 : denominator;

  return numerator / denominator;
}

// Finds a target (enemey, neutral, or own planet) for a planet to send fleets to
int findTarget(const PwState& pw, const Planet& source) {
  int dest = -1;
  double dest_score = -999999.0;
  vector<Planet> allPlanets = pw.Planets;
  for (int i = 0; i < allPlanets.size(); ++i) {
    const Planet& p = allPlanets[i];
    double score = scorePlanet(pw, source, p);
    if (score > dest_score) {
      dest_score = score;
      dest = p.PlanetID();
    }
  }

  if (dest != -1) {
    outputPlanet(gPlanetWars->GetPlanet(dest));
    debug("  destScore=" << dest_score << endl);
  }
  return dest;
}

// TODO: write .NumShips() function: takes into acount shipsLeaving

// Returns whether we've attacked a target
bool findAndEngageTarget(PwState& pw, const Planet& source) {
  int sourceId = source.PlanetID();
  // Check if we have enough ships to send out a fleet
  if (source.NumShips() - pw.shipsLeaving[sourceId] < MIN_PLANET_SHIPS * 2) {
    return false;
  }

  int targetId = findTarget(pw, source);
  const Planet& target = gPlanetWars->GetPlanet(targetId);
  if (targetId < 0 || targetId == sourceId) {
    return false;
  }

  // TODO: This here fleet size selection should be A LOT more sophisticated.
  // - should be different against enemies than neutrals
  // - should take into account enemy growth rate
  // - why 1/2? have a smarter default
  int halfForce = (source.NumShips() - pw.shipsLeaving[sourceId]) / 2;
  int numAttackingShips;
  int futureOwner = target.Owner(); //pw.planetsAdjusted[targetId].owner;
  int futureNumShips = pw.planetsAdjusted[targetId].numShips;
  if (futureOwner == 0) {
    numAttackingShips = min(futureNumShips + 1, halfForce);
  } else if (futureOwner == 1) {
    numAttackingShips = halfForce;
  } else {
    // This hsould be a lot more sophisticated. for example, find (last enemey fleet's remaining turns - my distance)*growth rate
    //numAttackingShips = min(futureNumShips + gPlanetWars->Distance(sourceId, targetId) * target.GrowthRate() + 1, halfForce);
    numAttackingShips = halfForce;
  }

  if (numAttackingShips <= 0) {
    return false;
  }

  // If after sending out this fleet, we lose this planet, don't attack!
  // TODO: this could be a little more sophisticated/generalized
  if (pw.planetsAdjusted[sourceId].numShips + numAttackingShips >= 0) {
    return false;
  }

  // If attacking a neutral and it won't fall, don't attack.
  // TODO: this should be revised to consider consolidated attacks, future
  // fleets that will be sent out this turn, etc.
//      if (gPlanetWars->GetPlanet(targetId).Owner() == 0 && pw.planetsAdjusted[targetId] - numAttackingShips >= 0) {
//        continue;
//      }

  // Now find a planet and maybe send half of ships from this planet to that.
  issueOrder(pw, sourceId, targetId, numAttackingShips);
  return true;
}

// This function is where your code goes. The PlanetWars object contains
// the state of the game, including information about all planets and fleets
// that currently exist. Inside this function, you issue orders using the
// pw.IssueOrder() function. For example, to send 10 ships from planet 3 to
// planet 8, you would say pw.IssueOrder(3, 8, 10).
//
// There is already a basic strategy in place here. You can use it as a
// starting point, or you can throw it out entirely and replace it with your
// own. Check out the tutorials and articles on the contest website at
// http://www.ai-contest.com/resources.
void DoTurn() {
  planetWarsTurn++;
  // Do nothing for the first turn to see what enemy does?
  // This is OK, probably an improvement
  // TODO: if this works, extend so that we don't do anything until enemy first
  // sends fleet
  if (planetWarsTurn == 1) {
    //return;
  }
  debug("\n--- Turn " << planetWarsTurn << endl);

  // Cached pw
  const PlanetWars& planetWars = *gPlanetWars;
  PwState pw(planetWars);

  // TODO: huh? why should this even be necessary? play with the constant here
  // This is sometimes helpful and sometimes not. A little less helpful than
  // not. Vary this according to our sjjtrength and turn #. Also consider
  // sending out simultaneous fleets at the beginning.
  if (pw.MyFleetsSize >= pw.MyPlanetsSize * 2 && planetWarsTurn > 10) {
    return;
  }

  // Attack! Each planet makes its own decision.
  // TODO: find a good order to loop in. maybe loop in order of strength?
  //     or, loop in proximity order to first target planet
  vector<Planet> myPlanets = planetWars.MyPlanets();
  for (vector<Planet>::iterator it = myPlanets.begin(); it != myPlanets.end();
      ++it) {
    do {
      findAndEngageTarget(pw, *it);
    } while (0);
  }
}


void unitTest();

// This is just the main game loop that takes care of communicating with the
// game engine for you. You don't have to understand or change the code below.
int main(int argc, char *argv[]) {
  if (argc == 2 && strcmp(argv[1], "debug") == 0) {
    debugMode = true;
  }
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
        gPlanetWars = &pw;
        //unitTest();
        DoTurn();
        pw.FinishTurn();
      } else {
        map_data += current_line;
      }
      current_line = "";
    }
  }
  return 0;
}


void testResolveBattle() {
  int a = 20, b = 20, c = 20;
  debug ( " a = " << &a << " b = " << &b  << " c = " << &c << " \n");
  int* victor = resolveBattle(a, b, c, &c);
  debug ( "winner:" << victor << " = " << *victor << endl);

  a = 3, b = 2, c = 1;
  debug ( " a = " << a << " b = " << b  << " c = " << c << " \n");
  victor = resolveBattle(a, b, c, &c);
  debug ( "winner:" << victor << " = " << *victor << endl);

  a = 2, b = 2, c = 1;
  debug ( " a = " << a << " b = " << b  << " c = " << c << " \n");
  victor = resolveBattle(a, b, c, &c);
  debug ( "winner:" << victor << " = " << *victor << endl);


  a = 2, b = 4, c = 0;
  debug ( " a = " << a << " b = " << b  << " c = " << c << " \n");
  victor = resolveBattle(a, b, c, &c);
  debug ( "winner:" << victor << " = " << *victor << endl);


  a = 4, b = 2, c = 0;
  debug ( " a = " << a << " b = " << b  << " c = " << c << " \n");
  victor = resolveBattle(a, b, c, &c);
  debug ( "winner:" << victor << " = " << *victor << endl);

  a = 2, b = 2, c = 7;
  debug ( " a = " << a << " b = " << b  << " c = " << c << " \n");
  victor = resolveBattle(a, b, c, &c);
  debug ( "winner:" << victor << " = " << *victor << endl);

  a = 2, b = 1, c = 7;
  debug ( " a = " << a << " b = " << b  << " c = " << c << " \n");
  victor = resolveBattle(a, b, c, &c);
  debug ( "winner:" << victor << " = " << *victor << endl);

  a = 2, b = 0, c = 7;
  debug ( " a = " << a << " b = " << b  << " c = " << c << " \n");
  victor = resolveBattle(a, b, c, &c);
  debug ( "winner:" << victor << " = " << *victor << endl);

  a = 7, b = 0, c = 2;
  debug ( " a = " << a << " b = " << b  << " c = " << c << " \n");
  victor = resolveBattle(a, b, c, &c);
  debug ( "winner:" << victor << " = " << *victor << endl);

  a = 0, b = 0, c = 7;
  debug ( " a = " << a << " b = " << b  << " c = " << c << " \n");
  victor = resolveBattle(a, b, c, &c);
  debug ( "winner:" << victor << " = " << *victor << endl);

  a = 2, b = 0, c = 0;
  debug ( " a = " << a << " b = " << b  << " c = " << c << " \n");
  victor = resolveBattle(a, b, c, &c);
  debug ( "winner:" << victor << " = " << *victor << endl);

  a = 2, b = 2, c = 7;
  debug ( " a = " << a << " b = " << b  << " c = " << c << " \n");
  victor = resolveBattle(a, b, c, &c);
  debug ( "winner:" << victor << " = " << *victor << endl);
}

// TODO: change call since adjustPlanetShips now a method of PwState
void testAdjustPlanetShips() {
//  IncomingFleets fleets;
//  int pid = 0;
//  const Planet& p = gPlanetWars->GetPlanet(pid);
//  debug ( "planet owner=" << p.Owner() << "ships=" <<  p.NumShips() << endl);
//
//  // this is to remove compiler warning
//  Fleet f = Fleet (1 /*owner*/, 86 /*#ships*/, 1,pid,4, 1 /*turns*/);
//  fleets.insert(&f);
//  f = Fleet (1 /*owner*/, 86 /*#ships*/, 1,pid,4, 1 /*turns*/);
//  fleets.insert(&f);
//  f = Fleet (1 /*owner*/, 3 /*#ships*/, 1,pid,4, 1 /*turns*/);
//  fleets.insert(&f);
//  debug ( adjustPlanetShips(fleets, p));
}

// Ghetto unit testing - unit test right within the main file! no need to mock
// classes, copy-psate functions, or w/e!!!
void unitTest() {
  //testResolveBattle();
  testAdjustPlanetShips();
  debug ( "\n\n\n---- END DEBUG FILE ----------------\n\n");
  exit(0);
}

