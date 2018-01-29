var SerialPort = require('serialport');
var port = new SerialPort('/dev/tty.SLAB_USBtoUART');

// The open event is always emitted
port.on('open', function() {
  // open logic
  port.write('main screen turn on', function(err) {
  	if (err) {
    	return console.log('Error on write: ', err.message);
  	}
  		console.log('message written');
	});
});
  
// Open errors will be emitted as an error event
port.on('error', function(err) {
  console.log('Error: ', err.message);
})

// Switches the port into "flowing mode"
// port.on('data', function (data) {
//   	console.log('Data is ', data);
//   	port.write('main screen turn on', function(err) {
//   		if (err) {
//     		return console.log('Error on write: ', err.message);
//   		}
//   			console.log('message written');
// 		});
// });
 
// Read data that is available but keep the stream from entering "flowing mode"
// port.on('readable', function () {
//   console.log('Data:', port.read());
// });

port.on('open', function () {
	port.write('1234\r\n');
});
