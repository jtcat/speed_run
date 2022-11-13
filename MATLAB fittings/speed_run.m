clear
close
clc

table = load("speed_run.txt");
n = table(1:end,1); % Road size
t = table(1:end,2); % Time to find a solution

% Original data plot
figure(1)
plot(n,t)
legend('Original curve')
title('Time as a function of road size')
xlabel('Road size')
ylabel('Time to find a solution (s)')

% Curve fitting
hold on
f = fit(n,t,'exp1');
plot(f,n,t)

% Extend fitted curve on x-axis to [0,800]
x_fit = 0:800;
coefficients = coeffvalues(f);
y_fit = coefficients(1) * exp(coefficients(2)*x_fit);
figure(2)
plot(x_fit,y_fit)
xlim([0 850])
title('Time as a function of road size')
xlabel('Road size')
ylabel('Time to find a solution (s)')
fprintf('Time needed to solve the problem, with size 800: %e seconds\n',y_fit(end))