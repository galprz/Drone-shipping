var messageHandlers = {}

WsClient = function(ip,port){
  this.ip = ip;
  this.port = port.toString()
}

WsClient.prototype.connect = function(){
  var messageHandlers = {}
  messageHandlers["StatusTypes.STATE"] = function(stateID){
    moveTo(parseInt(stateID))
  }
  messageHandlers["LogTypes.INFO"] = function(logMessage){console.info(logMessage)}
  messageHandlers["LogTypes.DEBUG"] = function(logMessage){
      if(logMessage.indexOf("{") >= 0){
          var str = logMessage.substr(logMessage.indexOf("{"))
          var json = JSON.parse(str);
          jQuery.each(json, function(i, val) {
            $("#" + i).html(val);
          });
      }
      console.debug(logMessage)
  }
  messageHandlers["LogTypes.WARNING"] = function(logMessage){console.warn(logMessage)}
  messageHandlers["LogTypes.CRITICAL"] = function(logMessage){console.error(logMessage)}
  messageHandlers["CMDTypes.PING"] = function(logMessage){console.info("Connectivity ok")}


  this.socket =  new WebSocket("ws://" + this.ip + ":" + this.port );
  this.socket.onopen = function()
  {
    console.log("connected");
  };

  this.socket.onmessage = function (evt)
  {
    var received_msg = evt.data;

    try
    {
        received_msg = JSON.parse(received_msg)
    }
    catch(e)
    {
      console.error("Got none json message from server");
      return;
    }

    if(!received_msg.type || !(messageHandlers.hasOwnProperty(received_msg.type)))
    {
     // console.error("Got Illegal message from server");
    }
    if(messageHandlers[received_msg.type]){
        messageHandlers[received_msg.type](received_msg.body);
    }
  };

  this.socket.onclose = function()
  {
    console.log("Connection is closed...");
  };
}

WsClient.prototype.send = function(msg)
{
  this.socket.send(msg)
  console.log("Message isent : " + msg);
}
