

var classInstanceHandles = { }

var TypeConversion = {};
TypeConversion.toJSON = {};
TypeConversion.toJS = {};

TypeConversion.toJSON['Byte']        = function(value) { return value; }
TypeConversion.toJSON['UInt16']      = function(value) { return value; }
TypeConversion.toJSON['UInt32']      = function(value) { return value; }
TypeConversion.toJSON['UInt64']      = function(value) { return value; }
TypeConversion.toJSON['Boolean']     = function(value) { return value; }
TypeConversion.toJSON['Timestamp']   = function(value) { new Date(value).getTime() / 1000; }
TypeConversion.toJSON['String']      = function(value) { return value; }
TypeConversion.toJSON['Uuid']        = function(value) { return value; }

TypeConversion.toJSON['Buffer']      = function(value) {  
                                            var array = [];
                                            for(var entry in buffer) {
                                                array.push(entry);
                                            }; 
                                            return array;
                                        ;}

TypeConversion.toJSON['ConstBuffer'] = function(value) { 
                                            var array = [];
                                            for(var entry in buffer) {
                                                array.push(entry);
                                            }; 
                                            return array;
                                        };

TypeConversion.toJSON['Vector'] = 
function(values, valuesType) {
    if (valuesType.length != 1) {
        throw new Error('Unknown vector type.')
    };
    var array = [];
    for (var i = 0; i < values.length; i++) {
        array.push(TypeConversion.ToJSON[valuesType](values[i]));
    }; 
    return array;
}

TypeConversion.toJSON['List'] = 
function(values, valuesType) {
    if (valuesType.length != 1) {
        throw new Error('Unknown list type.')
    };
    var array = [];
    for (var i = 0; i < values.length; i++) {
        array.push(TypeConversion.ToJSON[valuesType](values[i]));
    }; 
    return array;
}

TypeConversion.toJSON['Set'] = 
function(values, valuesType) {
    if (valuesType.length != 1) {
        throw new Error('Unknown set type.')
    };
    var array = [];
    for (var i = 0; i < values.length; i++) {
        array.push(TypeConversion.ToJSON[valuesType](values[i]));
    }; 
    return array;
}

TypeConversion.toJSON['Map'] = 
function(values, valuesType) {
    if (valuesType.length != 2) {
        throw new Error("Unkonwn map type.")
    }
    else if (valuesType[0] != 'String') {
        throw new Error("Key has to be a string.")
    };
    var map = { };
    for (var key in values) {
        map[key] = TypeConversion.toJSON[valuesType[1]](values[key]);
    };
    return map; 
}

TypeConversion.toJS['Byte']        = function(value) { return value; }
TypeConversion.toJS['UInt16']      = function(value) { return value; }
TypeConversion.toJS['UInt32']      = function(value) { return value; }
TypeConversion.toJS['UInt64']      = function(value) { return value; }
TypeConversion.toJS['Boolean']     = function(value) { return value; }
TypeConversion.toJS['Timestamp']   = function(value) { return new Date(value*1000); }
TypeConversion.toJS['String']  = function(value) { return value; }
TypeConversion.toJS['Uuid']        = function(value) { return value; }
TypeConversion.toJS['Buffer']      = function(value) { return new Uint8Array(value); }
TypeConversion.toJS['ConstBuffer'] = function(value) { return new Uint8Array(value); }

TypeConversion.toJS['Vector'] = 
function(values, valuesType) {
    if (valuesType.length != 1) {
        throw new Error('Unknown vector type.')
    };
    var array = [];
    for (var i = 0; i < values.length; i++) {
        array.push(TypeConversion.toJS[valuesType](values[i]));
    }; 
    return array;
}

TypeConversion.toJS['List'] = 
function(values, valuesType) {
    if (valuesType.length != 1) {
        throw new Error('Unknown list type.')
    };
    var array = [];
    for (var i = 0; i < values.length; i++) {
        array.push(TypeConversion.toJS[valuesType](values[i]));
    }; 
    return array;
}

TypeConversion.toJS['Set'] = 
function(values, valuesType) {
    if (valuesType.length != 1) {
        throw new Error('Unknown set type.')
    };
    var array = [];
    for (var i = 0; i < values.length; i++) {
        array.push(TypeConversion.toJS[valuesType](values[i]));
    }; 
    return array;
}

TypeConversion.toJS['Map'] = 
function(values, valuesType) {
    if (valuesType.length != 2) {
        throw new Error("Unkonwn map type.")
    }
    else if (valuesType[0] != 'String') {
        throw new Error("Key has to be a string.")
    }; 
 
    var map = { };
    for (var key in values) {
        map[key] = TypeConversion.toJS[valuesType[1]](values[key]);
    };
    return map; 
}
