var processes = { };
var classInstanceHandles = { }

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
        var value;
        var responseType = processes[response.id][2];
        if (response.value in classInstanceHandles)
        {
            processes[response.id](classInstanceHandles[response.value]); 
        }
        else {
            TypeConversion.toJS[responseType](response.value);
        }

        processes[response.id](value); 
        delete processes[response.id];
    }
    else {
        Console.log('Respones couldn\'t be mapped')
    }
};

function onClose(closeEventArgs) { };

function onError(errorEventArgs) { };

TypeConversion = {};
TypeConversion.toJson = {};
TypeConversion.toJS = {};

TypeConversion.toJSON['Byte']        = function(value) { return value; }
TypeConversion.toJSON['Uint16']      = function(value) { return value; }
TypeConversion.toJSON['Uint32']      = function(value) { return value; }
TypeConversion.toJSON['Uint64']      = function(value) { return value; }
TypeConversion.toJSON['Boolean']     = function(value) { return value; }
TypeConversion.toJSON['TimeStamp']   = function(value) { return new Date(value*1000); }
TypeConversion.toJSON['String']      = function(value) { return value; }
TypeConversion.toJSON['Uuid']        = function(value) { return value; }

TypeConversion.toJSON['Buffer']      = function(value) { { 
                                                                var array = [];
                                                                for(var entry in buffer) {
                                                                    array.push(entry;
                                                                }; 
                                                                return array;
                                                            };}

TypeConversion.toJSON['ConstBuffer'] = function(value) { 
                                                                var array = [];
                                                                for(var entry in buffer) {
                                                                    array.push(entry;
                                                                }; 
                                                                return array;
                                                            };

TypeConversion.toJSON['Vector']      = function(value) { return value; }
TypeConversion.toJSON['List']        = function(value) { return value; }
TypeConversion.toJSON['Set']         = function(value) { return value; }
TypeConversion.toJSON['Map']         = function(value) { return value; }

TypeConversion.toJS['Byte']        = function(value) { return value; }
TypeConversion.toJS['Uint16']      = function(value) { return value; }
TypeConversion.toJS['Uint32']      = function(value) { return value; }
TypeConversion.toJS['Uint64']      = function(value) { return value; }
TypeConversion.toJS['Boolean']     = function(value) { return value; }
TypeConversion.toJS['TimeStamp']   = function(value) { return new Date(value*1000); }
TypeConversion.toJS['String']      = function(value) { return value; }
TypeConversion.toJS['Uuid']        = function(value) { return value; }
TypeConversion.toJS['Buffer']      = function(value) { return new Uint8Array(value); }
TypeConversion.toJS['ConstBuffer'] = function(value) { return new Uint8Array(value); }
TypeConversion.toJS['Vector']      = function(value) { return value; }
TypeConversion.toJS['List']        = function(value) { return value; }
TypeConversion.toJS['Set']         = function(value) { return value; }
TypeConversion.toJS['Map']         = function(value) { return value; }


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