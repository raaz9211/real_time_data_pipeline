const WebSocket = require('ws');
const wss = new WebSocket.Server({port: 3000});

wss.on('connection', ws => {
    console.log('Client connected');
    ws.on('message', message => {
        console.log('Message from client:', message.toString());
        ws.send(`Echo: ${message}`);
    });
});
