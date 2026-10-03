A place to write your findings and plans

## Understanding
The game draws a rectangle around the player / treasure and uses it to detect the two touching.
The game uses a Butano utility to generate text for the score display, it generates sprites that are stored in score_sprites.

looks like things that don't change during the play of the game (the size of the sprite, the location of the score) all are declared as static outside of main

the d-pad buttons can be pressed in concert with eachother (ie left and down) because they are seperate if's not else ifs


## Planning required changes

1 change speed of player 
    //completed
2 change the backdrop color 
    //completed
3 change the starting postion of the player and the dot 
    //completed
4 hitting start the game restarts
    //completed
5 make it so player loops around the screen (left to right, bot to top)
    Screen resolution is 240x160, maybe once the player is at/beyond the edge we could set their x/y position to (position * -1) ? 
    i think negative is relative to origin so i think *-1 will throw them off the screen? i might be conceptually misunderstanding
    i'm gonna make an attempt where if they cross path max-w/max-h or min-w/min-h it'll throw them into the other
    //completed
6 make a speed boost when pressing A(temp and 3 charges, resets on start)
    plan is to make an int of 3 boosts
    create a counter variable
    pressing _A will start a "counter" if boosts is greater than 0;
    pressing _A will subtract 1 from the boosts if greater than 0;
    plan to make two seperate paths for movement
    make a new speed variable to hold boost    

## Brainstorming game ideas

## Plan for implementing game
Updating the score display only when the score changes instead of every frame
(Could create a function for updating the score display and only call it after changing the score?)
