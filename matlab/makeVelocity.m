%% Veclocity curve for data.
clear

% Constants
g = 21.937; % mph/s = 1g


%%
% Need 10 seconds of acceleration with some peak accleration to reach 45
% mph. Start with a max acceleration, fit accel curve to cubic spline

alen = 5; % seconds
amax = 0.13*g; % mph/s

% Define acceleration curve.
taspec = alen*[0; 0.1; 0.95; 1];
afun = @(t) ppval(pchip(taspec, amax*[0; 1; 1; 0]), t);

tt = linspace(0, alen, 200);
plot(tt, afun(tt))


%%
% Integrate the acceleration to get velocity.

dt = alen/numel(tt);
vv = cumsum(afun(tt))*dt;

plot(tt, vv)


%%
% Integrate velocity for distance.

ss = cumsum(vv)*dt/60; % 1/60 hr/sec

plot(tt, ss);
