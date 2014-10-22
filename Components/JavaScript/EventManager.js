/**
 * @brief Represents an event.
 */
everbase.Event = function() { };


/**
 * @brief Represents an observable object. An observable object can have one or more observers.
 */
everbase.EventManager = function() { };

// very inefficient implementation - its required for the dummy only anyway... so we dont care

everbase.EventManager._observers = { };
everbase.EventManager._entryCount = 0;

/**
 * @brief Fires an event.
 */
/* void */ everbase.EventManager.fireEvent = function(/*evb.Event*/ event)
{
	for( var entryId in everbase.EventManager._observers )
	{
		if( everbase.EventManager._observers[entryId].eventName === event.TYPE_NAME )
		{
			everbase.EventManager._observers[entryId].callback.call(everbase.EventManager._observers[entryId].observer, event);
		}
	}
};

/**
 * @brief Adds an entry to the event dispatch table with an observer and callback. Returns an observer entry id.
 */
/* Number */ everbase.EventManager.addObserver = function(/*Object*/ observer, /*Function*/ callback, /*Type of evb.Event*/ eventType)
{
	var entryId = everbase.EventManager._entryCount++;

	everbase.EventManager._observers[entryId] = { observer: observer, callback: callback, eventName: eventType.TYPE_NAME };

	return entryId;
};

/**
 * @brief Removes an observer entry from the event dispatch table.
 */
/* void */ everbase.EventManager.removeObserver = function(/*Number*/ entryId)
{
	delete everbase.EventManager._observers[entryId];
};
