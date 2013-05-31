function StompConnect() {

    var url = $("#url").val();
    var username = $("#username").val();
    var password = $("#password").val();
    var destinationAudio = $("#destinationAudio").val();
    var destinationProcessed = $("#destinationProcessed").val();

    console.log("url = " + url + " dest = " + destinationAudio + " dest proc = " + destinationProcessed + " uname = " + username + " pass = " + password);
    StompConnect.client = Stomp.client(url);
    StompConnect.connected = false;
    StompConnect.audioView = new AudioView();
    

    var onconnect = function (frame) {
        StompConnect.client.subscribe(destinationAudio, function (message) {
            var value = parseInt(message.body);
            StompConnect.audioView.onAudio(value);
        });

        StompConnect.client.subscribe(destinationProcessed, function (message) {

        });
        //enable disabled menu
        $("#Connect").addClass('disabled');
        $("#Disconnect").removeClass('disabled');
        StompConnect.connected = true;

    }

    var onError = function (frame) {
        if (StompConnect.connected) {
            StompConnect.connected = false;
            console.log("WebSocket Connection Error =" + frame);
            alert("WebSocket Connection Error " + frame);
            stompDisconnect();
        }
    };

    StompConnect.client.connect(username, password, onconnect, onError);
    window.location = "#close";
    return false;
}

function stompDisconnect() {
    StompConnect.connected = false;
    StompConnect.client.disconnect(function () {
        $("#Connect").removeClass('disabled');
        $("#Disconnect").addClass('disabled');

        console.log("Disconnected ");
        window.location = "#close";
    });
    AudioView.prototype.close();
}
