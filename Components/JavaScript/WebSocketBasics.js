var processes = { };
var host = 'ws://localhost:3000';
var ws = { };

ws.onopen = onOpen;
ws.onmessage = onMessage;
ws.onclose = onClose;
ws.onerror = onError;

typeConversion.toJSON['Caching'] = function(value)

function onOpen(openEventArgs) { };

function onMessage(msgEventArgs) {
    var message = JSON.parse(msgEventArgs.data);
    routeMessage(message);
};

function routeMessage(message) {
    if (response['type'] === 'response') {
        processResponse(response);
    }
    else if (response['type'] === 'event') {
        Console.log('Event recieved.');
    }
    else {
        Console.log('Unknown response type.');
    };
};

function processResponse(response) {
    Console.log('Response recieved.');
    if (response.id in processes) { 
        var value = typeConversion[response](response.value);        

        processes[response.id](response.value); 
        delete processes[response.id];
    }
    else {
        Console.log('Respones couldn\'t be mapped')
    }
};

function onClose(closeEventArgs) { };

function onError(errorEventArgs) { };


TypeConversion.toJSON['Everbase::Primitives::Byte']        = function(value) { return value; }
TypeConversion.toJSON['Everbase::Primitives::Uint16']      = function(value) { return value; }
TypeConversion.toJSON['Everbase::Primitives::Uint32']      = function(value) { return value; }
TypeConversion.toJSON['Everbase::Primitives::Uint64']      = function(value) { return value; }
TypeConversion.toJSON['Everbase::Primitives::Boolean']     = function(value) { return value; }
TypeConversion.toJSON['Everbase::Primitives::TimeStamp']   = function(value) { return value.getTime()/1000; }
TypeConversion.toJSON['Everbase::Primitives::String']      = function(value) { return value; }
TypeConversion.toJSON['Everbase::Primitives::Uuid']        = function(value) { return value; }
TypeConversion.toJSON['Everbase::Primitives::Buffer']      = function(value) { return new Uint16Array(value); }
TypeConversion.toJSON['Everbase::Primitives::ConstBuffer'] = function(value) { return new Uint16Array(value); }
TypeConversion.toJSON['Everbase::Primitives::Vector']      = function(value) { return value; }
TypeConversion.toJSON['Everbase::Primitives::List']        = function(value) { return value; }
TypeConversion.toJSON['Everbase::Primitives::Set']         = function(value) { return value; }
TypeConversion.toJSON['Everbase::Primitives::Map']         = function(value) { return value; }


var uuid = (function () {
    function s4() {
        return Math.floor((1 + Math.random()) * 0x10000)
            .toString(16)
            .substring(1);
        }
    return function () {
        return s4() + s4() + '-' + s4() + '-' + s4() 
             + '-' + s4() + '-' + s4() + s4() + s4();
    };
})();