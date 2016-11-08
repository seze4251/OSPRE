function [r_error ] = PositionVelocityError4( pixPerDeg, error_vec )
% Cameron Maywood, Anthony Torres, Seth Zegelstein
% Date Created: 9/27/2016
% Date Modified: 10/3/2016
%
% PositionVelocityError
%
% Inputs
% pixels: The number of pixels the image sensor contains 1x1
% FOV: The field of view of the camera (degree) 1x2
% plots: A integer,0 if plots are not desired, 1 if position/veloicty plots
% are desired, 2 if best position method plot is desired 3 if all plots are
% desired 1x1
% pixPerDeg: 0 if we want to use the calculation for pixels per degree 1x1 or,
% a 1 x 2 to skip calculation and use manually computed pixel to degree
% values
% plotStr: If plots == 1, A 1 x 3 cell array with titles for position (plotStr{1}) and
% velocity (plotStr{2}), which position method (plotStr{3}) graphs, else
% plotStr = 0
% error_vec - A vector of the pixel errors that is allowed for image
% proccessing
%
% Outputs
% minAccuracy: The minimum degree accuracy required from image proccessing
% to satsify both position and velocity requirments
% validSolution: Boolean, 0 if solution can meet error requirments, -1 if
% soultion cannot meet error requirments
%
% Description: Calculates the error in position and velocity, plots the
% results if requested and returns the highest amount of image proccesing
% error allowed to still meet requirments
%
% Assumptions: NONE

% Determine pixels per degree
px_deg_x = pixPerDeg(1);
px_deg_y = pixPerDeg(1);

% Determine the Degree error from a given pixel per degree
[ ~, ~, sigma_eff ] = pixel2DegError( px_deg_x, px_deg_y, error_vec);

% Determine Position Error (CAMERON FUNCTION HERE)
% Outputs required

alpha = 5;                      % Sample error in alpha, deg
beta = 5;                       % Sample error in beta, deg

[Rerror_rangeEarth, range] = Earth_Range_Position_Error(alpha, beta, sigma_eff);
Rerror_rangeMoon = Moon_Range_Position_Error(alpha, beta, sigma_eff);

% Determine Minimum Position Error  (ANTHONY FUNCTION 1 HERE)
% Outputs required
Rerror_Angles = Angles_Position_Error(alpha, beta, sigma_eff);

[r_error, ~] = calcRminErr(Rerror_rangeEarth, Rerror_rangeMoon, Rerror_Angles);


figure;
hold on
row = 1;
plot(range, Rerror_rangeEarth(row,:),'b')
plot(range, Rerror_rangeMoon(row,:),'r')   
plot(range, Rerror_Angles(row,:),'g')
plot(range,1000*ones(length(range)),'k--')
legend('Earth Ranging', 'Moon Ranging', 'Angles');
xlabel('Range from Earth (km)');
ylabel('Position Error (km)');
str_tit = sprintf('Position Error vs Range from Earth with Pixel Error of %1.2f Pixels with Sony Camera',error_vec(row));
title(str_tit)
ylim([0 1400])

end

