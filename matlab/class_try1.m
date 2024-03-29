%%**********************************
%% filename: class_try1.m
%% Department of Electronics
%% Carleton University
%%********************************

function [output] = class_try1(input)

%input scaling
x(1)=-1.0+(2.0)*(input(1)-(5514.88)) / ((28691.4) - (5514.88));
x(2)=-1.0+(2.0)*(input(2)-(22.0)) / ((239.0) - (22.0));

%calculating hidden neurons
z(1) = 1.0 / ( 1.0 + exp(-1.0 * (-2.93958+x(1)*(-1.45092)+x(2)*(0.285506))));
z(2) = 1.0 / ( 1.0 + exp(-1.0 * (-3.2944+x(1)*(0.451392)+x(2)*(-0.0731381))));
z(3) = 1.0 / ( 1.0 + exp(-1.0 * (-5.37242+x(1)*(6.97017)+x(2)*(-17.7128))));
z(4) = 1.0 / ( 1.0 + exp(-1.0 * (-3.32079+x(1)*(-0.124996)+x(2)*(0.163268))));
z(5) = 1.0 / ( 1.0 + exp(-1.0 * (-3.0792+x(1)*(1.94544)+x(2)*(0.186152))));

%calculating output neurons
y(1) = 1.01784+z(1)*(-0.559558)+z(2)*(0.603792)+z(3)*(-1.01404)+z(4)*(0.475359)+z(5)*(-0.525633);
y(2) = -0.0798467+z(1)*(0.608547)+z(2)*(0.129924)+z(3)*(1.01691)+z(4)*(0.553065)+z(5)*(0.458378);

%output scaling
output(1) = 0.0+(y(1)-(0.0))*((1.0) - (0.0))/((1.0)-(0.0));
output(2) = 0.0+(y(2)-(0.0))*((1.0) - (0.0))/((1.0)-(0.0));
