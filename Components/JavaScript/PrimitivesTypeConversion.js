var classInstanceHandles = { }

var TypeConversion = {};
TypeConversion.toJSON = {};
TypeConversion.toJS = {};

TypeConversion.toJSON['Byte']        = function(value) { return value; }
TypeConversion.toJSON['UInt16']      = function(value) { return value; }
TypeConversion.toJSON['UInt32']      = function(value) { return value; }
TypeConversion.toJSON['UInt64']      = function(value) { return value; }
TypeConversion.toJSON['Boolean']     = function(value) { return value; }
TypeConversion.toJSON['TimeStamp']   = function(value) { new Date(value).getTime() / 1000; }
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
function(values, params) {
    if (params.length != 1 &&  values.length != 1) {
        throw new Error('Unknown vector type.')
    };
    var array;
    for (var i = 0; i < values.length; i++) {
        array.push(TypeConversion.ToJSON[params[i]](values[i]);
    }; 
    return array;
}

TypeConversion.ToJSON['List'] = 
function(values, params) {
    if (params.length != 1 &&  values.length != 1) {
        throw new Error('Unknown list type.')
    };
    var array;
    for (var i = 0; i < values.length; i++) {
        array.push(TypeConversion.ToJSON[params[i]](values[i]);
    }; 
    return array;
}

TypeConversion.ToJSON['Set'] = 
function(values, params) {
    if (params.length != 1 &&  values.length != 1) {
        throw new Error('Unknown set type.')
    };
    var array;
    for (var i = 0; i < values.length; i++) {
        array.push(TypeConversion.ToJSON[params[i]](values[i]);
    }; 
    return array;
}

TypeConversion.ToJSON['Map'] = 
function(values, params) {
    if (arams.length != 2 &&  values.length != 2) {
        throw new Error("Unkonwn map type.")
    };
    var map = { };
    for (var i = 0; i < values.length; i++) {
        map[params[i]](values[i]) = TypeConversion.ToJSON[params[i]](values[i]);
    }; 
    return map; 
}

TypeConversion.toJS['Byte']        = function(value) { return value; }
TypeConversion.toJS['UInt16']      = function(value) { return value; }
TypeConversion.toJS['UInt32']      = function(value) { return value; }
TypeConversion.toJS['UInt64']      = function(value) { return value; }
TypeConversion.toJS['Boolean']     = function(value) { return value; }
TypeConversion.toJS['TimeStamp']   = function(value) { return new Date(value*1000); }
TypeConversion.toJS['String']      = function(value) { return value; }
TypeConversion.toJS['Uuid']        = function(value) { return value; }
TypeConversion.toJS['Buffer']      = function(value) { return new Uint8Array(value); }
TypeConversion.toJS['ConstBuffer'] = function(value) { return new Uint8Array(value); }

TypeConversion.toJS['Vector'] = 
function(values, params) {
    if (params.length != 1 &&  values.length != 1) {
        throw new Error('Unknown vector type.')
    };
    var array;
    for (var i = 0; i < values.length; i++) {
        array.push(TypeConversion.ToJS[params[i]](values[i]);
    }; 
    return array;
}

TypeConversion.ToJS['List'] = 
function(values, params) {
    if (params.length != 1 &&  values.length != 1) {
        throw new Error('Unknown list type.')
    };
    var array;
    for (var i = 0; i < values.length; i++) {
        array.push(TypeConversion.ToJS[params[i]](values[i]);
    }; 
    return array;
}

TypeConversion.ToJS['Set'] = 
function(values, params) {
    if (params.length != 1 &&  values.length != 1) {
        throw new Error('Unknown set type.')
    };
    var array;
    for (var i = 0; i < values.length; i++) {
        array.push(TypeConversion.ToJS[params[i]](values[i]);
    }; 
    return array;
}

TypeConversion.ToJS['Map'] = 
function(values, params) {
    if (arams.length != 2 &&  values.length != 2) {
        throw new Error("Unkonwn map type.")
    };
    var map = { };
    for (var i = 0; i < values.length; i++) {
        map[params[i]](values[i]) = TypeConversion.ToJS[params[i]](values[i]);
    }; 
    return map; 
}