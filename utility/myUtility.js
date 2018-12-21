client = null;
connected = false;


// called when the client connects
function onConnect(context) {
  // Once a connection has been made, make a subscription and send a message.
  console.log("Client Connected");
  subscribe("Temperature", 0);
  subscribe("Humidity", 0);
  subscribe("PH", 0);
  subscribe("Motor", 0);
  subscribe("Copony", 0);
  subscribe("Light", 0);
  connected = true;
}

function onFail(context) {
  console.log("Failed to connect");
  connected = false;
}

// called when the client loses its connection
function onConnectionLost(responseObject) {
  if (responseObject.errorCode !== 0) {
    console.log("Connection Lost: " + responseObject.errorMessage);
  }
  connected = false;
}

// called when a message arrives
function onMessageArrived(message) {
    var topic = message.destinationName;
    var payload = message.payloadString;
    var qos = message.qos;
  console.log('Message Recieved: Topic: ', topic, '. Payload: ', payload, '. QoS: ', qos);
  console.log(message);

  if((topic == "Motor")){
    if((payload =="1")&&(document.getElementById("switch1").checked == false)) {
      document.getElementById("switch1").checked = true;
    }
    else if((payload =="2")&&(document.getElementById("switch1").checked == true)){
      document.getElementById("switch1").checked = false;
    }
  }
  
   if((topic == "Copony")){
    if((payload =="3")&&(document.getElementById("switch2").checked == false)) {
      document.getElementById("switch2").checked = true;
    }
    else if((payload =="4")&&(document.getElementById("switch2").checked == true)){
      document.getElementById("switch2").checked = false;
    }
  }
  
   if((topic == "Light")){
    if((payload =="5")&&(document.getElementById("switch3").checked == false)) {
      document.getElementById("switch3").checked = true;
    }
    else if((payload =="6")&&(document.getElementById("switch3").checked == true)){
      document.getElementById("switch3").checked = false;
    }
  }

  if(topic == "Temperature") {
    document.getElementById("temp1").innerHTML = payload;
  }
    if(topic == "Humidity") {
    document.getElementById("temp2").innerHTML = payload;
  }

  var messageTime = new Date().toISOString();
  // Insert into History Table
  var table = document.getElementById("incomingMessageTable").getElementsByTagName('tbody')[0];
  var row = table.insertRow(0);
  row.insertCell(0).innerHTML = message.destinationName;
  row.insertCell(1).innerHTML = safe_tags_regex(message.payloadString);
  row.insertCell(2).innerHTML = messageTime;
  row.insertCell(3).innerHTML = message.qos;


  if(!document.getElementById(message.destinationName)){
      var lastMessageTable = document.getElementById("lastMessageTable").getElementsByTagName('tbody')[0];
      var newlastMessageRow = lastMessageTable.insertRow(0);
      newlastMessageRow.id = message.destinationName;
      newlastMessageRow.insertCell(0).innerHTML = message.destinationName;
      newlastMessageRow.insertCell(1).innerHTML = safe_tags_regex(message.payloadString);
      newlastMessageRow.insertCell(2).innerHTML = messageTime;
      newlastMessageRow.insertCell(3).innerHTML = message.qos;

  } else {
      // Update Last Message Table
      var lastMessageRow = document.getElementById(message.destinationName);
      lastMessageRow.id = message.destinationName;
      lastMessageRow.cells[0].innerHTML = message.destinationName;
      lastMessageRow.cells[1].innerHTML = safe_tags_regex(message.payloadString);
      lastMessageRow.cells[2].innerHTML = messageTime;
      lastMessageRow.cells[3].innerHTML = message.qos;
  }

}

function connect(){

    var host="192.168.1.9";

    var port=8000;
    if(path.length > 0){
      client = new Paho.MQTT.Client(host, Number(port), path, clientId);
    } else {
      client = new Paho.MQTT.Client(host, Number(port), clientId);
    }
    console.info('Connecting to Server: Hostname: ', hostname, '. Port: ', port, '. Path: ', client.path, '. Client ID: ', clientId);

    // set callback handlers
    client.onConnectionLost = onConnectionLost;
    client.onMessageArrived = onMessageArrived;


    var options = {
      invocationContext: {host : hostname, port: port, path: client.path, clientId: clientId},
      timeout: timeout,
      keepAliveInterval:keepAlive,
      cleanSession: cleanSession,
      useSSL: tls,
      onSuccess: onConnect,
      onFailure: onFail
    };



    if(user.length > 0){
      options.userName = user;
    }

    if(pass.length > 0){
      options.password = pass;
    }

    // connect the client
    client.connect(options);
}

function disconnect(){
    console.info('Disconnecting from Server');
    client.disconnect();
    connected = false;
}


function publish(topic, message){
    console.info('Publishing Message: Topic: ', topic, '. QoS: ' + qos + '. Message: ', message);
    newMessage = new Paho.MQTT.Message(message);
    newMessage.destinationName = topic;
    newMessage.qos = Number(qos);
    newMessage.retained = retain;
    client.send(newMessage);
}


function subscribe(topic, qos){
    console.info('Subscribing to: Topic: ', topic, '. QoS: ', qos);
    client.subscribe(topic, {qos: Number(qos)});
}

function clearHistory(){
    var table = document.getElementById("incomingMessageTable");
    //or use :  var table = document.all.tableid;
    for(var i = table.rows.length - 1; i > 0; i--)
    {
        table.deleteRow(i);
    }

}


// Just in case someone sends html
function safe_tags_regex(str) {
   return str.replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;');
}

function checkSwitch(num){
  var statusSwitch = document.getElementById("switch"+num).checked;
  switch(num){
    case 1:
        if(statusSwitch){
          publish("Motor", "1");
        }
        else publish("Motor", "2");
        break;
    case 2:
        if(statusSwitch){
          publish("Copony", "3");
        }
        else publish("Copony","4");
        break;
    case 3:
        if(statusSwitch){
          publish("Light", "5");
        }
        else publish("Light", "6");     
}
}
