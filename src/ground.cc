#include <iostream>
#include <cassert>
#include "ground.h" 
#include <cstdlib>

using std::cout;
using std::endl;

Ground ground;

Ground::Ground()
{
	player.i = -1;
	player.j = -1;
	ground_size.i = 0;
	ground_size.j = 0;
}

bool Ground::isEmpty(const Point &p)
{
	return (ground(p)== EMPTY || ground(p) == 'X' || ground(p) == 'D'
			|| ground(p) =='T');
}

void Ground::findTunnel()
{
	std::cout << "Searching for tunnel" << std::endl;

	//////////////////////////////////////////////
	//First Pass detects this three configurations:
	//////////////////////////////////////////////
	//D: # Dead because in a corner
	//  #D
	//
	//X: # Dead because tree directions are blocked
	//  #X#
	//
	//T: #T# a tunnel
	for (int i = 0; i < ground_size.i; i++)
	{
		for (int j = 0; j<ground_size.j ; j++)
		{
			if (__ground[i]->at(j) == WALL || __ground[i]->at(j)==GOAL)
			{
				continue;
			}
			//Tunnel -->one direction is blocked
			//Dead end --> three directions are blocked
			Point dir[2] = {UP, LEFT};
			bool twoDirBlocked = false;
			bool oneDirBlocked = false;
			bool dead = false;

			for (int k= 0; k<2; k++)
			{
				Point main_dir = Point(i,j)+dir[k];
				Point inv_dir = Point(i,j)-dir[k];
				bool main_block = isOut(main_dir) || (ground(main_dir)==WALL);
				bool inv_block = isOut(inv_dir) || (ground(inv_dir)==WALL);
				if (main_block && inv_block)
					twoDirBlocked = true;
				else if (main_block || inv_block)
				{
					if (oneDirBlocked)
						dead = true;
					else
						oneDirBlocked = true;
				}

			}

			if (twoDirBlocked && oneDirBlocked)
				__ground[i]->replace(j,1,1,'X');
			else if (dead)
				__ground[i]->replace(j,1,1,'D');
			else if (twoDirBlocked)
				__ground[i]->replace(j,1,1,'T');
		}
	}
	//////////////////////////////////////////////
	//Second Pass: detect dead tunnels
	//////////////////////////////////////////////
	//--> A tunnel near a X is dead:
	//   # 
	//  #X#
	//  #T#
	for (int i = 0; i<ground_size.i; i++)
	{
		for (int j = 0; j<ground_size.j; j++)
		{
			if (__ground[i]->at(j) == 'X')
			{
				propagate_dead(Point(i,j));
			}
		}
	}
	std::cout << "En of tunnel search" << std::endl;
	
	//////////////////////////////////////////////
	//Third Pass: detect dead lines
	//////////////////////////////////////////////
	//
	//--> ##################
	//    #D----------------D(or X)
	//The (-) are dead-ends because we can only push
	//the box to dead-ends from this position (There
	//should be no goals).
	//
	for (int i = 0; i<ground_size.i; i++)
	{
		for (int j = 0; j<ground_size.j; j++)
		{
			if (__ground[i]->at(j) == 'X' || __ground[i]->at(j) == 'D')
			{
				//Seek propag and block dir.
				for (int k = 0; k<4; k++)
				{
					if ( isOut(Point(i,j)+DIR[k]) || ground(Point(i,j)+DIR[k]) == WALL)//This will the blocked direction
					{
						//DIR[k+1] is orthogonal to DIR[k]
						if (!isOut(Point(i,j)+DIR[(k+1)%4]) && ground(Point(i,j)+DIR[(k+1)%4]) != WALL)
						{
							propagate_line(Point(i,j), DIR[k],DIR[k+1]);
						}
						else if (!isOut(Point(i,j)+DIR[(k-1)%4]) && ground(Point(i,j)+DIR[(k-1)%4]) != WALL)
						{
							propagate_line(Point(i,j), Point(DIR[k]), DIR[(k-1)%4]);
						}
					}
				}

			}
		}
	}
}

// ################
// D-->(looking for a D in this direction)
// Here dir_block = UP and dir_propag = RIGHT
bool Ground::propagate_line(const Point& here, const Point& dir_block, const Point& dir_propag)
{
	Point next = here + dir_propag;
	Point to_check = next + dir_block;


	if (isOut(next) || ground(next) == 'D' || ground(next) == 'X')
	{
		if (!isOut(here))
			__ground[here.i]->at(here.j) = 'D';
		return true;
	}
	else if (ground(next) == GOAL)
	{
		return false;
	}

	if (isOut(to_check) || ground(to_check) == WALL || ground(to_check) == 'X')
	{
		if (propagate_line(next, dir_block, dir_propag))
		{
			__ground[here.i]->at(here.j) = 'D';
			return true;
		}
	}
	else
	{
		return false;
	}
}

void Ground::propagate_dead(const Point& p)
{
	for (int k= 0; k <4 ; k++)
	{
		Point n = p+DIR[k];
		if (isOut(n)) continue;
		if (ground(n)=='T')
		{
			__ground[n.i]->at(n.j) = 'X';
			propagate_dead(n);
		}
	}
}




bool Ground::rec_getPath(Point from, Point to, string& result, bool** visited_points, const State& state)
{
	visited_points[from.i][from.j] = true;

	//We have arrived!
	if (from.i == to.i && from.j == to.j)
	{
		return true;
	}

	//Check the four directions
	Point next;

	//Down
	next.i = from.i + 1;
	next.j = from.j;
	if (!isOut(next) && isPassable(next, state) && !visited_points[next.i][next.j]) {
		if (rec_getPath(next, to, result, visited_points, state))
		{
			result.append("D");
			return true;
		}
	}
	
	//Up
	next.i = from.i - 1;
	next.j = from.j;
	if (!isOut(next) && isPassable(next, state) && !visited_points[next.i][next.j]) {
		if (rec_getPath(next, to, result, visited_points, state))
		{
			result.append("U");
			return true;
		}
	}

	//Left
	next.i = from.i;
	next.j = from.j-1;
	if (!isOut(next) && isPassable(next, state) && !visited_points[next.i][next.j]) {
		if (rec_getPath(next, to, result, visited_points, state))
		{
			result.append("L");
			return true;
		}
	}

	//Right
	next.i = from.i;
	next.j = from.j + 1;
	if (!isOut(next) && isPassable(next, state) && !visited_points[next.i][next.j]) {
		if (rec_getPath(next, to, result, visited_points, state))
		{
			result.append("R");
			return true;
		}
	}

	//We couldn't find a route from this point
	return false;
}

//Add a row to the Ground (rows must be added from top to bottom)
bool Ground::addRow(const string& col) {

	string * col_to_save = new string(col);
	int new_size = (col.size());
	if (ground_size.j != 0)
	{
		assert(new_size == ground_size.j);
	}
	else
	{
		ground_size.j = new_size;
	}

	///////////////////////////////////////////
	//		 PLAYER							 //
	///////////////////////////////////////////
	//If we haven't found the player yet, try to find him
	if (player.i == -1)
	{
		bool player_is_on_goal = false;

		//Try to find the player
		//(the player can be on a goal
		player.j = col_to_save->find(P_ON_GOAL);

		if (player.j != string::npos)
		{
			(*col_to_save)[player.j] = GOAL;
			player.i = __ground.size();
		}
		else
		{
			//Or the player can be on an empty case
			player.j = col_to_save->find(PLAYER);
			if (player.j != string::npos)
			{
				(*col_to_save)[player.j] = EMPTY;
				player.i = __ground.size();
			}
		}
	}
	
	///////////////////////////////////////////
	//		 BOXES							 //
	///////////////////////////////////////////
	//Are there boxes in this row ?
	Point tmp;
	tmp.j = -1;
	while ((tmp.j = col_to_save->find(BOX, tmp.j+1))!=string::npos)
	{
		tmp.i = __ground.size();
		boxes.insert(tmp);	
		(*col_to_save)[tmp.j] = EMPTY;
	}

	tmp.j = -1;
	while ((tmp.j = col_to_save->find(B_ON_GOAL, tmp.j+1))!=string::npos)
	{
		tmp.i = __ground.size();
		boxes.insert(tmp);	
		(*col_to_save)[tmp.j] = GOAL;
	}
	
	///////////////////////////////////////////
	//		 GOAL							 //
	///////////////////////////////////////////
	tmp.j = -1;
	while ((tmp.j = col_to_save->find(GOAL, tmp.j+1))!=string::npos)
	{
		tmp.i = __ground.size();
		goals.push_back(tmp);	
	}

	__ground.push_back(col_to_save);
	ground_size.i++;
	return false;
}

State Ground::getInitialState()
{
	return State(boxes, player);
}

bool Ground::getPath(Point from, Point to, string& result, const State& state)
{
	bool ** visited_points = (bool**)malloc(ground_size.i*sizeof(bool*));
	for (int i = 0; i<ground_size.i; i++)
	{
		visited_points[i] = (bool*)malloc(ground_size.j*sizeof(bool));
		for (int j = 0; j <ground_size.j; j++)
			visited_points[i][j] = false;
	}
	
	std::string reversed_result;
	bool path_found = rec_getPath(from, to, reversed_result, visited_points, state);
	if (!path_found)
	{
		std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
		std::cout << "************ERROR****************" << std::endl;
		std::cout << " IMPOSSIBLE TO FIND A PATH" << std::endl;
		std::cout << " IT SHOULD NEVER HAPPEN" << std::endl;
		std::cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
	}

	//We must reversed the string !
	int str_size = reversed_result.size();
	for (int i = str_size-1; i>-1; i--)
	{
		const char c = reversed_result[i];
		result.push_back(c);
	}

	for (int i = 0; i<ground_size.i; i++)
		free(visited_points[i]);
	free(visited_points);

	return true;
}


void Ground::display()
{
	std::vector<string *>::iterator it;
	std::cout << "------ DISPLAY MAP -------" << std::endl;
	for (it = __ground.begin(); it != __ground.end(); it++)
	{
		std::cout << *(*it) << std::endl;
	}

	std::cout << std::endl;
	std::cout << "Goals :" << std::endl;
	std::vector<Point>::iterator it_g;
	for (it_g = goals.begin(); it_g!=goals.end(); it_g++)
	{
		std::cout << it_g->i << " " << it_g->j << std::endl;
	}

}

Point Ground::getPlayer()
{
    return player;
}

bool Ground::isOut(const Point& here)
{
	bool out= (here.i < 0 || here.j <0 || here.i > ground_size.i-1 || here.j > __ground[here.i]->size()-1);
	return out;
}

bool Ground::isBlocked(const Point& here)
{
	char type = (*(__ground[here.i]))[here.j];
	return (type == WALL);
}

char Ground::operator()(const Point& here)
{
	return __ground[here.i]->at(here.j);
}
	
Point Ground::getInitialPosPlayer()
{
	return player;
}

vector<Point> Ground::getGoals() {
	return goals;
}

bool Ground::isDeadend(const Point& here)
{
    
    bool isDeadend = false;
    int i = 0;
    while(i < deadends.size() && isDeadend == false)
    {
        if (here == deadends[i]) isDeadend = true;
        i++;
    }
    
    return isDeadend;
}

bool Ground::isPassable(const Point& here, const State& state)
{
    bool hasBeenVisited = false;
    for(int i = 0; i < tempPath.size(); i++){
        if(here == tempPath[i]){
            hasBeenVisited = true;
            break;
        }
    }
    
    bool passable = (!isOut(here) && !isBlocked(here) && state.getBoxesRef().find(here) == state.getBoxesRef().end() && !hasBeenVisited && !isDeadend(here));
    
	return passable;
}

int Ground::calcManhattDist(const Point& a, const Point& b)
{
    
    int distY = a.i-b.i;
    int distX = a.j-b.j;
    
    if(distX < 0) distX = distX*-1;
    if(distY < 0) distY = distY*-1;
    
    return (distX+distY);
    
}

Point Ground::getNextCell(const Point& from, const Point& to, const State& state)
{
    
    Point next = from;
    
    int distY = from.i-to.i;
    int distX = from.j-to.j;
    
    int passable[4];
    
    for (int i = 0; i<4; i++) passable[i] = ground.isPassable(from + DIR[i], state);
    
    //for (int i = 0; i<4; i++) std::cout << i << " " << passable[i] << std::endl;
    
    if     (distX < 0 && passable[3]) next = from + RIGHT;
    else if(distX > 0 && passable[1]) next = from + LEFT;
    else if(distY < 0 && passable[2]) next = from + DOWN;
    else if(distY > 0 && passable[0]) next = from + UP;
    else{
        for (int i = 0; i < 4; i++) {
            if(passable[i] == true)
            {
                next = from+DIR[i];
                break;
            }
        }
    }
    
    
    return next;
}

string Ground::addDirectionLetter(const Point& from, const Point& next)
{
    
    string letter;
    
    if(next == (from+UP)) letter = "U";
    if(next == (from+DOWN)) letter = "D";
    if(next == (from+RIGHT)) letter = "R";
    if(next == (from+LEFT)) letter = "L";
    
    return letter;
}

string Ground::findPath(const Point& from, const Point& to, const State& state)
{
    string path;
    
    if(from != to)
    {
        //std::cout << "Start finding path!" << std::endl;
        tempPath.clear();
        deadends.clear();
        tempPath.push_back(from);
        explorePath(from, to, state, from);
        
        //std::cout << "Found path with " << tempPath.size() << " steps!" << std::endl;
        
        //std::cout << "Start generating solution string!" << std::endl;
        
        for(int i = 0; i < tempPath.size()-1; i++) path += addDirectionLetter(tempPath[i],tempPath[i+1]);
    }
    else path = "";
    
    return path;
}

void Ground::explorePath(const Point& from, const Point& to, const State& state, const Point& start)
{
    //std::cout << "Last place:  " << tempPath[tempPath.size()-2].i << " " << tempPath[tempPath.size()-2].j << std::endl;
    //std::cout << "Current place:  " << from.i << " " << from.j << std::endl;
    //std::cout << "Function start: Path size:  " << tempPath.size() << " steps!" << std::endl;
    
    if(from != to)
    {
        Point next = getNextCell(from, to, state);
        
        if(next != from)
        {
            //std::cout << "Found next cell!" << std::endl;
            tempPath.push_back(next);
            //std::cout << "Function End: Path size:  " << tempPath.size() << " steps!" << std::endl;
            //std::cout << "Next place:  " << next.i << " " << next.j << std::endl;
            explorePath(next, to, state, start);
            
        }
        else
        {
            //std::cout << "This is a deadend!" << std::endl;
            if(from == start)
            {
                tempPath.clear();
                //std::cout << "We are back at the Player position! There is no pathway to this goal yet!" << std::endl;
                return;
            }
            else 
            {
                if(from == tempPath[tempPath.size()-1]){
                    //std::cout << "tempPath.pop_back()" << std::endl;
                    tempPath.pop_back();
                }
                deadends.push_back(from);
                //std::cout << "Function End: Path size:  " << tempPath.size() << " steps!" << std::endl;
                next = tempPath[tempPath.size()-1];
                //std::cout << "Back to place:  " << next.i << " " << next.j << std::endl;
                explorePath(next, to, state, start);
            }
        }
    }
    //else std::cout << "Reached aim!" << std::endl;
}

//Well not really important...
Ground::~Ground() {
	vector<string *>::iterator it;
	for (it = __ground.begin(); it != __ground.end(); it++)
		delete *it;
}
