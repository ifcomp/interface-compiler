var processes = { };
var classInstanceHandles = { }
var socketOpen = false;
var queuedMessages = [];

var host = 'ws://' + location.host;
var ws = new WebSocket(host);

function onOpen(openEventArgs) {
  socketOpen = true;
  for (var i = 0; i < queuedMessages.length; i++) {
    WebSocket.prototype.send.call(ws, queuedMessages[i]);
  }
};

function onMessage(msgEventArgs) {
    var message = JSON.parse(msgEventArgs.data);
    routeMessage(message);
};

function routeMessage(message) {
    if (message[0] === 'response') {
        processResponse(message);
    }
    else if (message[0] === 'event') {
        processEvent(message);
    }
    else {
        console.log('Unknown response type.');
    };
};

function processResponse(response) {
    var responseId = response[2];
    if (responseId in processes)
    { 
        var responseVal = response[3];
       	var responseType = processes[responseId][1];

	if(responseType != '')
	{
	        var responseParamTypes = processes[responseId][2];
        	var conversionedResult = TypeConversion.toJS[responseType](responseVal, responseParamTypes);
	        processes[responseId][0](conversionedResult);
	}
	else
	{
	        processes[responseId][0]();
	}

        delete processes[responseId];
    }
    else {
        console.log('Response couldn\'t be mapped')
    }
};

function processEvent(event) {
    try 
    {
        var eventName = event[1];
        var eventValues = event[2];
        var conversionedEvent = { };
        conversionedEvent = TypeConversion.toJS[eventName](eventValues);
        Everbase.EventManager.fireEvent(conversionedEvent);
    }
    catch (e){
        throw Error(e.name + ': ' + e.message);
    }
};

function onClose(closeEventArgs) { };

function onError(errorEventArgs) { };

ws.send = function(message) {
    if(!socketOpen) {
      queuedMessages.push(message);
    } else {
      WebSocket.prototype.send.call(ws, message);
    }
};

ws.onopen = onOpen;
ws.onmessage = onMessage;
ws.onclose = onClose;
ws.onerror = onError;

