#include <iostream>
#include <cstdlib>
#include "csocket.h"
#include "ground.h"

int main(int pArgC, char* pArgs[])
{
    if(pArgC<4)
    {
        std::cerr << "usage: " << pArgs[0] << " host port boardnum" << std::endl;
        return -1;
    }

    soko::CSocket lSocket(pArgs[1],pArgs[2]);

    lSocket.WriteLine(pArgs[3]);
    
    std::string lLine;

    //read number of rows
    lSocket.ReadLine(lLine);
    int lNumRows=atoi(lLine.c_str());
    
    //read each row
	std::cout << "Parsing rows" << std::endl;
    for(int i=0;i<lNumRows;i++)
    {
        lSocket.ReadLine(lLine);
        //here, we would store the row somewhere, to build our board
        //in this demo, we just print it
        std::cout << lLine << "\n";
		ground.addRow(lLine);
    }

	ground.findTunnel();
	std::cout << "---- Display annoted map ----" << std::endl;
	ground.display();

	std::cout << "---- Display initial state ----" <<std::endl;
	State init = ground.getInitialState();
	init.display();
	
	//------------------------------- A* -------------------------------------
	/*
	State * final_state = NULL;
	//int i = 0;
	while (final_state==NULL)
	{
		//i++;
		//if (i%1000==0)
			//std::cout << "Nodes " << i << std::endl;
		State* to_expand = State::nextStateToExpand();
		final_state = to_expand->expand();
	}
	//std::cout << " Found a solution after having expanded " 
	//	<< i << "nodes " << std::endl;

	*/
	//--------------------- IDA* based on IDDFS ------------------------------
	
	State * final_state = init.IDAStar_search(&init, &init);	
	
	//------------------------------------------------------------------------

	State * curr_state = final_state;
	std::cout << "------------" << std::endl;
	std::cout << "Final State " << std::endl;
	std::cout << "------------" << std::endl;

	while (curr_state != NULL)
	{
		curr_state = curr_state->getParentState();
	}
    
    //-------- Solution Pathfinding Test ------------
    std::string solutionPath = final_state->findSolutionString(final_state, &init);
    //------ END: Solution Pathfinding Test ---------
    
    //-------- Player Pathfinding Test -----------
    /*
    std::string pathPlayerGoal;
    std::vector<Point> goals = ground.getGoals();
    Point player = ground.getPlayer();
    
    if(!goals.empty()){
        Point testGoal = goals[0];
        std::cout << "The player is at     " << player.i << " " <<player.j << std::endl;
        std::cout << "The aimed goal is at " << testGoal.i << " " <<testGoal.j << std::endl;
        pathPlayerGoal = ground.findPath(player,testGoal);
    }
    else std::cout << "There are no goals!" << std::endl;
    */
    //------ END: Player Pathfinding Test --------
    
    //std::cout << "Path to next goal: " << pathPlayerGoal << std::endl;
    //std::cout << "Complete Solution Path: " << solutionPath << std::endl;
    //std:string pathPlayerGoal = "R";
    //lSocket.WriteLine(pathPlayerGoal);

	std::cout << "Solution path " << solutionPath << std::endl;

    //send the solution to the server
    lSocket.WriteLine(solutionPath);
    
    //read answer from the server
    lSocket.ReadLine(lLine);
    
    std::cout << lLine << std::endl;

    return 0;
}

