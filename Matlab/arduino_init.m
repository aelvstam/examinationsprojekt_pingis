function a = arduino_init()

% a: Arduino-object

delete(instrfind);
clear a;

% Arduino-object a=serial('COMxx')
%a=serial('/dev/tty.usbmodemfa131');
a=serial('COM16');
set(a, 'BaudRate', 115200);
set(a, 'DataBits', 8);
set(a, 'StopBits', 1);
set(a, 'Parity', 'none');
set(a, 'Terminator', 10);
fopen(a);