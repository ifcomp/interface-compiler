var processes = { };
var classInstanceHandles = { }

var host = 'ws://localhost:3000';
var ws = new WebSocket(host);

ws.onopen = onOpen;
ws.onmessage = onMessage;
ws.onclose = onClose;
ws.onerror = onError;

function onOpen(openEventArgs) { };

function onMessage(msgEventArgs) {
    var message = JSON.parse(msgEventArgs.data);
    routeMessage(message);
};

function routeMessage(message) {
    if (message[0] === 'response') {
        processResponse(message);
    }
    else if (message[0] === 'event') {
        console.log('Event recieved.');
    }
    else {
        console.log('Unknown response type.');
    };
};

function processResponse(response) {
    console.log('Response recieved.');
    var responseId = response[2];
    if (responseId in processes) { 
        var responseVal = response[3];
        console.log(responseVal);
        var conversionedResult;
        var responseType = processes[responseId][1];
        conversionedResult = TypeConversion.toJS[responseType](responseVal);

        processes[responseId][0](conversionedResult); 
        delete processes[responseId];
    }
    else {
        console.log('Respones couldn\'t be mapped')
    }
};

function onClose(closeEventArgs) { };

function onError(errorEventArgs) { };




