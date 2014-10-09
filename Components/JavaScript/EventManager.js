// very inefficient implementation - its required for the dummy only anyway... so we dont care


Everbase.EventManager._observers = { };
Everbase.EventManager._entryCount = 0;

/**
 * @brief Fires an event.
 */
/* void */ Everbase.EventManager.fireEvent = function(/*evb.Event*/ event)
{
	for( var entryId in Everbase.EventManager._observers )
	{
		if( Everbase.EventManager._observers[entryId].eventName == event.TYPE_NAME )
		{
			Everbase.EventManager._observers[entryId].callback.call(Everbase.EventManager._observers[entryId].observer, event);
		}
	}
};

/**
 * @brief Adds an entry to the event dispatch table with an observer and callback. Returns an observer entry id.
 */
/* Number */ Everbase.EventManager.addObserver = function(/*Object*/ observer, /*Function*/ callback, /*Type of evb.Event*/ eventType)
{
	var entryId = Everbase.EventManager._entryCount++;

	Everbase.EventManager._observers[entryId] = { observer: observer, callback: callback, eventName: eventType };

	return entryId;
};

/**
 * @brief Removes an observer entry from the event dispatch table.
 */
/* void */ Everbase.EventManager.removeObserver = function(/*Number*/ entryId)
{
	delete Everbase.EventManager._observers[entryId];
};
