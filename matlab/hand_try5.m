%%**********************************
%% filename: hand_try5.m
%% Department of Electronics
%% Carleton University
%%********************************

function [output] = hand_try5(input)

%input scaling
x(1)=-1.0+(2.0)*(input(1)-(5514.88)) / ((28691.4) - (5514.88));
x(2)=-1.0+(2.0)*(input(2)-(22.0)) / ((239.0) - (22.0));

%calculating hidden neurons
z(1) = 1.0 / ( 1.0 + exp(-1.0 * (-3.90524+x(1)*(1.0154)+x(2)*(-1.90856))));
z(2) = 1.0 / ( 1.0 + exp(-1.0 * (-2.97126+x(1)*(1.02579)+x(2)*(0.786421))));
z(3) = 1.0 / ( 1.0 + exp(-1.0 * (-3.26237+x(1)*(-0.0478322)+x(2)*(0.591173))));
z(4) = 1.0 / ( 1.0 + exp(-1.0 * (-3.23043+x(1)*(0.163303)+x(2)*(0.576706))));
z(5) = 1.0 / ( 1.0 + exp(-1.0 * (-4.86683+x(1)*(6.32204)+x(2)*(-16.1359))));

%calculating output neurons
y(1) = 0.00333253+z(1)*(-0.979725)+z(2)*(0.270113)+z(3)*(-0.10409)+z(4)*(-0.0754795)+z(5)*(1.06785);

%output scaling
output(1) = 0.0+(y(1)-(0.0))*((1.0) - (0.0))/((1.0)-(0.0));
