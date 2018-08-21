# BiOfflineClockTimer
This is an offline clock timer for games made with Cocos2d-x but it can be used with other frameworks with small modifications. For example, you could use this for games that have lives that make you wait a specific amount of time until the lives refill (like Candy Crush or most match 3 games). If you close the app and lauch the app again after a while, the clock will calculate the difference and continue where you should be. You can set the amount of loops it can do. This loop can be used for the amount of lives your game has for example. It will keep track of the loops even if you close the app. With the right modifications, this code can have many purpose. It can be used for other applications if you need a timer that needs to loop a specific amount of times even if the app is closed. Cocos2d-x specific code will be marked.



# Cocos2d-x Example

// create clock 	

time_t now = time(0);
int timeNowInt = now;

int secondsTillLoop = 60;
int clockId = 1;
int loopAmounts = 5;

m_clock = new BiOfflineClockTimer(timeNowInt, secondsTillLoop, clockId, loopAmounts);

// Losing a life. Send signal.

EventCustom event("Event_Custom_Life_Lost");
_eventDispatcher->dispatchEvent(&event);

// Listen to that signal
EventListenerCustom* _listenerLifeLost = EventListenerCustom::create("Event_Custom_Life_Lost", [=](EventCustom* event) {
    if (!m_clock->IsClockTicking())
		{
		    m_clock->StartClock(false);
		}
});
	_eventDispatcher->addEventListenerWithFixedPriority(_listenerGameOver, 1);
