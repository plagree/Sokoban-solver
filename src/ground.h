#ifndef __Ground__H
#define __Ground__H

#include <string>
#include <vector>
#include <set>
#include "point.h"
#include "state.h"
#include "case_type.h"
#include "checker.h"


using std::string;
using std::vector;
using std::set;

//This class stores the position of the different objects.
//The unmovable object such as walls and goals are stored in
//a vector of vector.
//The position of the player and of the 
class Ground {

	public:
		////////////////////////
		// Building the ground
		////////////////////////
		//Build an empty Ground
		Ground();
		~Ground();

		//Add a row at the end of the Ground
		bool addRow(const string& col);

		void display();

		void findTunnel();
		bool isEmpty(const Point& p);

		State getInitialState();

		bool isBlocked(const Point& here);
        bool isPassable(const Point& here, const State& state); //!!!
        bool isDeadend(const Point& here); //!!!
		bool getPath(Point from, Point to, string& result, const State& state);
		bool rec_getPath(Point from, Point to, string& result, bool** visited_points, const State& state);
    
		bool propagate_line(const Point& here, const Point& dir_block, const Point& dir_propag);
		const Point& getSize() {
			return ground_size;
		};

		bool isOut(const Point& here);
		char operator()(const Point& here);
    
        Point getNextCell(const Point& from, const Point& to, const State& state); //!!!
        string addDirectionLetter(const Point& from, const Point& next); //!!!
        string findPath(const Point& from, const Point& to, const State& state); //!!!
        void explorePath(const Point& from, const Point& to, const State& state, const Point& start); //!!!
        int calcManhattDist(const Point& a, const Point& b);

		Point getInitialPosPlayer();

		vector<Point> getGoals();
        Point getPlayer();

		void propagate_dead(const Point& p);

	private:
		//Current player position
		Point player;
		Point Ground_size;
		vector<string *> __ground;
		vector<Point> goals;
        vector<Point> deadends; //!!!
        vector<Point> tempPath; //!!!
		set<Point> boxes;
		Point ground_size;
		vector<State *> next_states;
};

extern Ground ground;

#endif /*__Ground__H*/
