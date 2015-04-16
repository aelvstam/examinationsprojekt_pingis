%---------------------|
% By Peter Johansson  |
% and Andreas Elvstam |
%---------------------|
%-----------------------|
% Input variables:      |
% a:  Arduino-object    |
% bv: Setpoint          |
% Kp: Proportional gain |
% Ti: Time integral     |
% Td: Time derivative   |
% N:  Number of samples |
% dT: Sample time in ms |
%-----------------------|
%--------------------------|
% Output variables:        |
% h: Ball level (altitude) |
% t: Time                  |
% u: PWM-signal            |
% e: Error value           |
%--------------------------|
function [h,t,u,e] = arduino_plot4(a,bv,Kp,Ti,Td,N,dT)
h = zeros(1, N);
t = zeros(1, N);
u = zeros(1, N);
e = zeros(1, N);

% 10-bit setpoint calculation for 8-bit sending
if(bv > 767 && bv < 1024)
    input = bv - 768;
    multiplier = 3;
end
if(bv > 511 && bv < 768)
    input = bv - 512;
    multiplier = 2;
end
if(bv > 255 && bv < 512)
    input = bv - 256;
    multiplier = 1;
end
if(bv < 256 && bv >= 0)
    input = bv;
    multiplier = 0;
end   

% None float number before send
Kp_decimal = decimal(Kp);
Kp = Kp * 10^(Kp_decimal);
Ti_decimal = decimal(Ti);
Ti = Ti * 10^(Ti_decimal);
Td_decimal = decimal(Td);
Td = Td * 10^(Td_decimal);

if(bv > 1023 || Kp > 255 || Ti > 255 || Td > 255)
    disp('Values are too high!');
    return
end

if(bv < 0 || Kp < 0 || Ti < 0 || Td < 0)
    disp('Values are too low!');
    return
end

% Removes data from the input output buffer
flushinput(a);
flushoutput(a);

% Ready to send. Dec: 5
fwrite(a, 5);

% Arduino input
fwrite(a, multiplier);
fwrite(a, input);
fwrite(a, Kp_decimal);
fwrite(a, Kp);
fwrite(a, Ti_decimal);
fwrite(a, Ti);
fwrite(a, Td_decimal);
fwrite(a, Td);
fwrite(a, dT);
pause(0.1);

% dT in sec before calculation
dT = (dT / 1000) * 4;

% Start sample loop
for i = 1:N
    % Removes data from the input output buffer
    flushinput(a);
    flushoutput(a);
     
    start = cputime; 
    
    % Ready to receive. Dec: 6
    fwrite(a, 6);
    
    % Arduino output
    h_multiplier = fread(a, 1);
    h_temp = fread(a, 1);
    h(i) = number(h_multiplier,h_temp);
    
    u_multiplier = fread(a, 1);
    u_temp = fread(a, 1);
    u(i) = number(u_multiplier,u_temp);
    
    e_multiplier = fread(a, 1);
    e_temp = fread(a, 1);
    e(i) = number(e_multiplier,e_temp);
    
    t(i)= i;
    
    % Online-plot
    plot(t,h,'k-',t,u,'m:',t,e,'g-',t,bv,'r-');
    
    elapsed=cputime-start;
    ok=(dT-elapsed);
    pause(ok);
end

% End plot
plot(t,h,'k-',t,u,'m:',t,e,'g-',t,bv,'r-');
xlabel('samples k')
ylabel('level h, pwm u, error e')
title('step response ramp')
legend('h ', 'u ', 'e ')

%-------------------------------------------|
% Function that calculates a positive or a  |
% negative 10-bit number from 8-bit sending |
%-------------------------------------------|
function [newInput] = number(multiplier,input)
if(multiplier < 5)
    newInput = (256 * multiplier) + input;
end
if(multiplier > 4)
    newInput = (256 * (5 - multiplier)) - input;
end

%-------------------------------------------------|
% Function that calculates the number of decimals |
%-------------------------------------------------|
function [number_of_decimals] = decimal(num)
number_of_decimals = 0;
num = num - fix(num);
while(num ~= 0)
    num = num * 10;
    number_of_decimals = number_of_decimals + 1;
    num = num - fix(num);
    a = str2double(num2str(num));  
    if(num < 0.001 || a == 1)
        return;
    end
end