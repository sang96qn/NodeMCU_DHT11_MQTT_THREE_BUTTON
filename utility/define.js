var hostname = "localhost";
var port = 8000;
var clientId = makeid();
var path = "/mqtt";
var qos = 2;
var retain = true;
var tls = false;
var user = "";
var pass = "";
var keepAlive = 60;
var timeout = 3;
var cleanSession = false;

var Motor = "Motor";
var Copony = "Copony";
var Light = "Light";
var Temperature  = "Temperature";
var Humidity  = "Humidity";
var PH  = "PH";

function makeid()
{
    var text = "";
    var possible = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

    for( var i=0; i < 5; i++ )
        text += possible.charAt(Math.floor(Math.random() * possible.length));

    return text;
}