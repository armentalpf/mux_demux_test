# mux_demux_test
Test 74150 and 74154 multiplexor and decoder ICs
This is a simple programm to test 74150 and 74154 chips using Arduino uno board.
Use terminal to select address port to test and to write data to be trasmited from 74154 to 74150.
Control signals A to D are share for both devices.
74154 Output Q0 should to be connected to 74150 input I0 and so on for all outputs and inputs.
Serial configuration for Monitor: 9600bauds, not parity, 8 bits, CRLF terminator.
