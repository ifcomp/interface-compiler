var TypeConversion = {};
TypeConversion.toJSON = {};
TypeConversion.toJS = {};

TypeConversion.toJSON['Byte']        = function(value) { return value; }
TypeConversion.toJSON['Uint16']      = function(value) { return value; }
TypeConversion.toJSON['Uint32']      = function(value) { return value; }
TypeConversion.toJSON['Uint64']      = function(value) { return value; }
TypeConversion.toJSON['Boolean']     = function(value) { return value; }
TypeConversion.toJSON['TimeStamp']   = function(value) { new Date(value).getTime() / 1000; }
TypeConversion.toJSON['String']      = function(value) { return value; }
TypeConversion.toJSON['Uuid']        = function(value) { return value; }

TypeConversion.toJSON['Buffer']      = function(value) {  
                                            var array = [];
                                            for(var entry in buffer) {
                                                array.push(entry;
                                            }; 
                                            return array;
                                        ;}

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