#include "state.h"
#include "ground.h"
#include "point.h"
#include "case_type.h"
#include <iostream>

bool frozen_deadlock( std::set<Point>&       block_list,  // list of boxes already cheacked
                      Point&                 dir,         // direction of check
                      const Point&           location,    // location of the box being checked
                      const Point&           at,          // location of @
                      const Point&           moved_box,   // location of the box being pushed
                      bool                   vert_block,  // true iff the box is blocked vertically
                      bool                   horz_block,  // true iff the box is blocked horizionally 
                      std::set<Point>&       boxes){      // all boxes
    // Returns true iff the bo at location location is blocked in
    // both vertical and horizational direction.


    // left and right are the directions left and right
    // relative to DIR[i]
    // left operator:  [ [0,-1], [ 1,0] ]
    // right operator: [ [0, 1], [-1,0] ] 
    Point right( dir.j ,-dir.i);
    Point left(  -dir.j, dir.i);

    Point dir_list[3] =  {dir,right,left};

    char surround[3];

    // fill the surround array
    for ( int iter=0 ; iter!=3 ; iter++ ){
        if ( location+dir_list[iter]==at ){
            // location+dir_list[iter] is the where the player is currently at
            surround[iter] = EMPTY;
        }
        else if ( location+dir_list[iter]==moved_box ){
            // location+dir_list[iter] is the place where the box was just move
            surround[iter] = BOX;
        }
        else{
            if (ground(location+dir_list[iter])==WALL){
                surround[iter] = WALL;
            }
            else if ( boxes.count(location+dir_list[iter]) != 0 ){
                surround[iter] = BOX;
            }
            else{
                surround[iter] = EMPTY;
            }
        }
    }

    if (  ( surround[0]==EMPTY && vert_block==false ) ||
          ( surround[1]==EMPTY && surround[2]==EMPTY) ){
          // Not blocked in either vertical or horizional direction 
          // so no deadlock here
          return false;
    }

    // Vertical cheack:

    if ( vert_block==false ){
        Point next_point = location+dir;

        if ( surround[0]==WALL || block_list.count(next_point)!=0 ){
            // vertical dir blocked by wall or is alread in block_list
            vert_block = true;
        }
        else if ( surround[0] == BOX ){
            block_list.insert(location);
            // if left_point is blocked from both directions then location is blocked from left
            vert_block = frozen_deadlock(block_list ,dir ,next_point,at ,moved_box ,true ,false ,boxes );
        }
    }

   // Horizional cheack:

   if ( horz_block==false ){
       Point right_point = location+right;
       Point left_point  = location+left;
       if (  surround[1]==WALL                ||
             block_list.count(right_point)!=0 ||
             surround[2]==WALL                ||
             block_list.count(left_point)!=0  ){
           // horizional dir is blocked by wall or is already on the list
           horz_block = true;
       }
       if ( horz_block==false && surround[1]==BOX ){
           block_list.insert(right_point);
           // if left_point is blocked from both directions then location is blocked from right
           horz_block = frozen_deadlock(block_list ,right ,right_point, at, moved_box ,true ,false ,boxes );
       }
       if ( horz_block==false && surround[2]==BOX ){
           block_list.insert(left_point);
           // if left_point is blocked from both directions then location is blocked from left
           horz_block = frozen_deadlock(block_list ,left ,left_point, at, moved_box ,true ,false ,boxes );
       }
   }
   return horz_block && vert_block;
}


bool isDeadlock( const Point& p , Point& dir , std::set<Point>& boxes){
    // p:     is the location of the newly moved box
    // dir:   is the direction p was pushed to 
    // boxes: set of all boxes in the current state.
    //
    //  surround is an char array of five elements
    //  it represent the following situation:
    //  
    //      513
    //      4$2
    //       @
    //  
    //  Where: 
    //      surround[0] = 1,
    //      surround[1] = 2,
    //      surround[2] = 3,
    //      surround[3] = 4,
    //      surround[4] = 5,
    //
    //  Returns:
    //      true if we can conclude that this situation involves deadlock 
    //      false otherwise
    //
    // here a will denote either $ or #

    if ( ground(p) == GOAL ){
        // The box is on a goal so it dose not mater
        // if we have deadlock.
        return false;
    }

/*
    // left and right are the directions left and right
    // relative to DIR[i]
    // left operator:  [ [0,-1], [ 1,0] ]
    // right operator: [ [0, 1], [-1,0] ] 
    Point right( dir.j,-dir.i);
    Point left(-dir.j, dir.i);

    // list of direction around
    Point dir_list[5] =  {dir,right,dir+right,left,dir+left};

    char surround[5];

    // fill the surround array
    for ( int iter=0 ; iter!=5 ; iter++ ){
        if (ground(p+dir_list[iter])==WALL){
            surround[iter] = WALL;
        }
        else if ( boxes.count(p+dir_list[iter]) != 0 ){
            surround[iter] = BOX;
        }
        else{
            surround[iter] = EMPTY;
        }
    }

    char in_pos[3];
    in_pos[0] = EMPTY; // empty space
    in_pos[1] = BOX; // a box
    in_pos[2] = WALL; // a wall
    
////
//// The following checks are now part of the method frozen_deadlock:
////
/*    
///////////////////////////////////////////////////////////////////////////////////////////////////

    if ( (surround[0] != in_pos[0]) && (surround[1] != in_pos[0]) && (surround[2] != in_pos[0]) ){
        //  aa
        //  $a
        //  @
        return true;
    }
    if ( (surround[0] != in_pos[0]) && (surround[3] != in_pos[0]) && (surround[4] != in_pos[0]) ){
        // aa
        // a$
        //  @
        
        return true;
    }
    if ( (surround[0] == in_pos[2]) && ((surround[1]==in_pos[2]) || (surround[3]==in_pos[2])) ){
        //  #          #
        //  $#   or   #$
        //  @          @
        return true;
    }
    if ( (surround[0] == in_pos[1]) && (
            (surround[1]==in_pos[2] && surround[4]==in_pos[2]) || 
            (surround[3]==in_pos[2] && surround[2]==in_pos[2])
            ) ){
        // #$          $#
        //  $#   or   #$
        //  @          @
        return true;
    }
///////////////////////////////////////////////////////////////////////////////////////////////////////







    // Now we are interested in finding at deadlocks beyond surround
    Point ground_size = ground.getSize();
    Point beyond = p+dir+dir;
    if ( (ground(p+dir)!=GOAL)&&(surround[0]==EMPTY)  &&
                        (0<=beyond.i && 0<=beyond.j && beyond.i<ground_size.i && beyond.j<ground_size.j) ){
        // beyond is inside the ground
        Point beyond_list[3] =  {beyond,beyond+right,beyond+left};
        char surround_2[3];

        //
        //   201 surround_2[]
        //   402
        //   3$1
        //    @

        // fill the surround_2 array
        for ( int iter=0 ; iter!=3 ; iter++ ){
            if (ground(beyond_list[iter])==WALL){
                surround_2[iter] = WALL;
            }
            else if ( boxes.count(beyond_list[iter]) != 0 ){
                surround_2[iter] = BOX;
            }
            else{
                surround_2[iter] = EMPTY;
            }
        }
        if ( ( (surround[1]   != in_pos[0]) &&
               (surround[2]   != in_pos[0]) &&
               (surround[4]   != in_pos[0]) &&
               (surround_2[2] != in_pos[0]) &&
               (surround_2[0] != in_pos[0])   )  ||
             ( (surround[3]   != in_pos[0]) &&
               (surround[4]   != in_pos[0]) &&
               (surround[2]   != in_pos[0]) &&
               (surround_2[1] != in_pos[0]) &&
               (surround_2[0] != in_pos[0])   )   ){
            //   aa    aa
            //  a a    a a
            //  a$  or  $a
            //   @      @
            return true;
        }

    }
*/

    std::set<Point> block_list;
    return frozen_deadlock( block_list, dir, p, p-dir, p, false, false, boxes);

    //return false;
}
