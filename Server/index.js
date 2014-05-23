var sp = require('serialport'), express = require('express');

var signals = require('../Sender/lib.json');

var serialPort;

var app = express();

app.get('/:signal', function(req, res) {
    var signal = signals[req.params.signal];
    if (signal) {
        if (serialPort) {
            serialPort.pipe(res);
            var msg = amplifySignal(signal, 2) + '\n';
            serialPort.write(msg, function() {
                setTimeout(function() {
                    res.end();
                }, 2000);
            });
        } else {
            res.send('serial port error');
        }
    } else {
        res.send('bad signal name');
    }
});

initSerialPort();
app.listen(1234);

function initSerialPort() {
    sp.list(function(err, ports) {
        var portName, port;
        for (var i = 0; i < ports.length; i++) {
            port = ports[i];
            if (port.manufacturer === 'FTDI') {
                portName = port.comName;
                break;
            }
        }
        if (portName) {
            port = new sp.SerialPort(portName, { baudrate: 9600 });
            port.on('open', function(error) {
                if (!error) {
                    setTimeout(function() {
                        serialPort = port;
                    }, 1500);
                }
            });
        }
    });
}

function amplifySignal(signal, times) {
    var ret = [];
    signal = signal.replace(/,/g, '');
    for (var i = 0; i < times; i++) {
        ret.push(signal);
    }
    return ret.join('p');
}