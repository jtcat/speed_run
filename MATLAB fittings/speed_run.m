clear
close
clc

table = load("speed_run.txt");
n = table(1:end,1); % Road size
t = table(1:end,2); % Time to find a solution

% Original data plot
plot(n,t)
legend('Original curve')
title('Time as a function of road size')
xlabel('Road size')
ylabel('Time to find a solution (s)')

% Curve fitting
hold on
f = fit(n,t,'exp1');
plot(f,n,t)