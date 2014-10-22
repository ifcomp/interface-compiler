everbase.webclient = everbase.webclient || { };

everbase.webclient.connection = new everbase.rpc.WebSocket('ws://' + location.host);

everbase.webclient.processes = { };

everbase.webclient._routeMessage = function(message) {
    if (message[0] === 'response') {
        everbase.webclient._processResponse(message);
    }
    else if (message[0] === 'event') {
        everbase.webclient.processEvent(message);
    }
    else {
        throw Error('Unknown response type.');
    }
}

everbase.webclient._processResponse = function(response) {
    console.log('Response received.');
    var responseId = response[2];
    if (responseId in everbase.webclient.processes) { 
        var responseVal = response[3];
        var conversionedResult;
        var responseType = everbase.webclient.processes[responseId][1];
        if(responseType !== '')
        {
            var responseParamTypes = everbase.webclient.processes[responseId][2];
            conversionedResult = everbase.rpc.jsonEncoding._conversions[responseType].decode(responseVal, responseParamTypes);
            everbase.webclient.processes[responseId][0](conversionedResult); 
            console.log(conversionedResult);
        }
        else
        {
            everbase.webclient.processes[responseId][0]();
        }
        delete everbase.webclient.processes[responseId];
    }
    else {
        throw Error('Response couldn\'t be mapped');
    }
}

everbase.webclient._processEvent = function(event) {
    try 
    {
        var eventName = event[1];
        var eventValues = event[2];
        var conversionedEvent = { };
        conversionedEvent = everbase.rpc.jsonEncoding._conversions[eventName].decode(eventValues);
        everbase.EventManager.fireEvent(conversionedEvent);
    }
    catch (e) {
        throw Error(e.name + ': ' + e.message);
    }
};

everbase.webclient.connection.setMessageHandler(everbase.webclient._routeMessage);
