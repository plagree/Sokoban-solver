#include "checker.h"
#include "point.h"
#include "ground.h"
#include "state.h"
#include <set>
#include <iostream>
#include <cstdlib>

bool checker(const std::string& path) {
	
	State current = ground.getInitialState();
	Point posPlayer = ground.getInitialPosPlayer();
	Point nextPosition = posPlayer;
	
	for(int i=0;i<path.size();i++) {
		if(path[i]=='U') {
			nextPosition = posPlayer + UP;

			if(ground(nextPosition) != '#') {
				posPlayer = nextPosition;
				for(std::set<Point>::iterator it = current.getBoxes().begin(); it != current.getBoxes().end(); ++it) {
					if(*it==nextPosition) {
						current = State(current,nextPosition,0);
					}
				}
			}
		}

		else if(path[i]=='D') {
			nextPosition = posPlayer + DOWN;

			if(ground(nextPosition) != '#') {
				posPlayer = nextPosition;
				for(std::set<Point>::iterator it = current.getBoxes().begin(); it != current.getBoxes().end(); ++it) {
					if(*it==nextPosition) {
						current = State(current,nextPosition,2);
					}
				}
			}
		}

		else if(path[i]=='R') {
			nextPosition = posPlayer + RIGHT;

			if(ground(nextPosition) != '#') {
				posPlayer = nextPosition;
				for(std::set<Point>::iterator it = current.getBoxes().begin(); it != current.getBoxes().end(); ++it) {
					if(*it==nextPosition) {
						current = State(current,nextPosition,3);
					}
				}
			}
		}

		else if(path[i]=='R') {
			nextPosition = posPlayer + LEFT;

			if(ground(nextPosition) != '#') {
				posPlayer = nextPosition;
				for(std::set<Point>::iterator it = current.getBoxes().begin(); it != current.getBoxes().end(); ++it) {
					if(*it==nextPosition) {
						current = State(current,nextPosition,1);
					}
				}
			}
		}
	}

	if(current.isFinal()) {
		std::cout << "Problem solved" << std::endl;
		return true;
	}

	return false;

}
