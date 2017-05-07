var util  = require('util');
var app = require('http').createServer(handler);
var io = require('socket.io')(app);
var fs = require('fs');
app.listen(3484);

function handler (req, res) {
    fs.readFile(__dirname + '/index.html',
        function (err, data) {
            if (err) {
                res.writeHead(500);
                return res.end('Error loading index.html');
            }

            res.writeHead(200);
            res.end(data);
        });
}
function ParseJson(jsondata) {
    try {
        return JSON.parse(jsondata);
    } catch (error) {
        return null;
    }
}

io.on('connection', function (socket) {

    console.log("Connected");
    socket.emit('welcome', { message: 'Connected !!!!' });
    socket.on('connection', function (data) { 
    });

    socket.on('led', function(data){
        console.log( data );
        io.sockets.emit('led', { message: data });
    });

    socket.on( 'receivedValue', function( data ){ 
        var gelenData=data.message.split("#"); 
        io.sockets.emit("giden",{ sicaklik : gelenData[0], hareket : gelenData[1],
            mesafe : gelenData[2], alev : gelenData[3]  });
        console.log( gelenData );
    } );

});




