var classInstanceHandles = { }

everbase.rpc = everbase.rpc || { }

everbase.rpc.jsonEncoding = everbase.rpc.jsonEncoding || { }

everbase.rpc.jsonEncoding._conversions = everbase.rpc.jsonEncoding._conversions || { }

everbase.rpc.jsonEncoding._conversions['Byte'] = 
everbase.rpc.jsonEncoding._conversions['UInt16'] = 
everbase.rpc.jsonEncoding._conversions['UInt32'] = 
everbase.rpc.jsonEncoding._conversions['UInt64'] = 
everbase.rpc.jsonEncoding._conversions['Boolean'] = 
everbase.rpc.jsonEncoding._conversions['String'] = 
everbase.rpc.jsonEncoding._conversions['Uuid'] = {
    encode: function(value) { return value },
    decode: function(value) { return value }
};

everbase.rpc.jsonEncoding._conversions['Timestamp'] = {
    encode: function(value) { return new Date(value).getTime() / 1000; },
    decode: function(value) { return new Date(value*1000); }
};

everbase.rpc.jsonEncoding._conversions['Vector'] = 
everbase.rpc.jsonEncoding._conversions['List'] = 
everbase.rpc.jsonEncoding._conversions['Set'] = {
    encode: function(values, valuesType) {
        if (valuesType.length !== 1) {
            throw new Error('Unknown container type.')
        }
        var array = [];
        for (var i = 0; i < values.length; i++) {
            array.push(everbase.rpc.jsonEncoding._conversions[valuesType[0]].encode(values[i]));
        }
        return array;
    },
    decode: function(values, valuesType) {
        if (valuesType.length !== 1) {
            throw new Error('Unknown container type.')
        }
        var array = [];
        for (var i = 0; i < values.length; i++) {
            array.push(everbase.rpc.jsonEncoding._conversions[valuesType[0]].decode(values[i]));
        }
        return array;
    }
};

everbase.rpc.jsonEncoding._conversions['Map'] = {
    encode: function(values, valuesType) {
        if (valuesType.length !== 2) {
            throw new Error('Unkonwn map type.')
        }
        else if (valuesType[0] !== 'String') {
            throw new Error('Key has to be a string.')
        }
        var map = { };
        for (var key in values) {
            map[key] = everbase.rpc.jsonEncoding._conversions[valuesType[1]].encode(values[key]);
        }
        return map; 
    },
    decode: function(values, valuesType) {
        if (valuesType.length !== 2) {
            throw new Error('Unkonwn map type.')
        }
        else if (valuesType[0] !== 'String') {
            throw new Error('Key has to be a string.')
        }
        var map = { };
        for (var key in values) {
            map[key] = everbase.rpc.jsonEncoding._conversions[valuesType[1]].decode(values[key]);
        }
        return map; 
    }
};