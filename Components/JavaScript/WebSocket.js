var processes = { };
var classInstanceHandles = { }
var socketOpen = false;
var queuedMessages = [];

var host = 'ws://localhost:3000';
var ws = new WebSocket(host);

ws.onopen = onOpen;
ws.onmessage = onMessage;
ws.onclose = onClose;
ws.onerror = onError;

function onOpen(openEventArgs) {
  socketOpen = true;
  for (var i = 0; i < queuedMessages; i++) {
    console.log("Queued Message", queuedMessages[i])
    WebSocket.prototype.call.send(this, queuedMessages[i]);
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
    console.log('Response received.');
    var responseId = response[2];
    if (responseId in processes) { 
        var responseVal = response[3];
        var conversionedResult;
        var responseType = processes[responseId][1];
        var responseParamTypes = processes[responseId][2];
        conversionedResult = TypeConversion.toJS[responseType](responseVal, responseParamTypes);

        processes[responseId][0](conversionedResult); 
        delete processes[responseId];
    }
    else {
        console.log('Response couldn\'t be mapped')
    }
};

function processEvent(event) {
    try 
    {
        console.log('Event received.');
    console.log(event);
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
    console.log("WS Message", message);
    if(!socketOpen) {
      queuedMessages.push(message);
    } else {
      WebSocket.prototype.send.call(ws, message);
    }
};


