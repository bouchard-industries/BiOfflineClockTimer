# BiOfflineClockTimer
This is an offline clock timer for games made with Cocos2d-x but it can be used with other frameworks with small modifications. 
For example, you could use this for games that have lives that make you wait a specific amount of time until the lives refill 
(like Candy Crush or most match 3 games). If you close the app and lauch the app again after a while, the clock will calculate the 
difference and continue where you should be. You can set the amount of loops it can do. This loop can be used for the amount of lives 
your game has for example. It will keep track of the loops even if you close the app. With the right modifications, this code can have 
many purpose. It can be used for other applications if you need a timer that needs to loop a specific amount of times even if the app is 
closed. Cocos2d-x specific code will be marked.


If you would like to see this in action, go to the Apple app store or Google play store and search for Munchie Match. Download the game and once you are in the level selection screen you will see lives at the top right. If you play the game and lose notice the change in lives and time counting down. Try closing the app and launching again after some time. You will notice the difference. 
