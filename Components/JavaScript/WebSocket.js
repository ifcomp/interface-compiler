everbase.rpc = everbase.rpc || { }

everbase.rpc.WebSocket = function(hostAddress) {
    everbase.rpc.WebSocket._hostAddress = hostAddress;
    everbase.rpc.WebSocket._connect.call(this);
}

everbase.rpc.WebSocket._hostAddress = { };

everbase.rpc.WebSocket.prototype._messageHandler = function(jsonMessage) { };

everbase.rpc.WebSocket.prototype.setMessageHandler = function(handler) {
    this._messageHandler = handler;
}

everbase.rpc.WebSocket.prototype.send = function(jsonMessage) {
    var message = JSON.stringify(jsonMessage);
    if(!this._socketOpen) {
      this._queuedMessages.push(message);
    } 
    else {
      this._ws.send(message);
    }
};

everbase.rpc.WebSocket._connect = function() { 
    this._ws = new WebSocket(everbase.rpc.WebSocket._hostAddress);
    this._ws.onopen = everbase.rpc.WebSocket._onOpen.bind(this);
    this._ws.onmessage = everbase.rpc.WebSocket._onMessage.bind(this);
    this._ws.onerror = everbase.rpc.WebSocket._onError.bind(this);
    this._socketOpen = false;
    this._queuedMessages = [];
}

everbase.rpc.WebSocket._onOpen = function(openEventArgs) {
  this._socketOpen = true;
  for (var i = 0; i < this._queuedMessages.length; i++) {
    this._ws.send(this._queuedMessages[i]);
  }
};

everbase.rpc.WebSocket._onMessage = function(msgEventArgs) {
    var message = JSON.parse(msgEventArgs.data);
    this._messageHandler(message);
};

everbase.rpc.WebSocket._onClose = function(closeEventArgs) { 
    setTimeout(everbase.rpc.WebSocket._connect(this), 10*1000);
};

everbase.rpc.WebSocket._onError = function(errorEventArgs) { };


