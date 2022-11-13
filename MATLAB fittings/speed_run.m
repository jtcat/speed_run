clear
close
clc

table = load("speed_run.txt");
n = table(:,1); % Road size
t = table(:,2); % Time to find a solution

% Original data plot
figure(1)
plot(n,t)
legend('Original curve')
title('Time as a function of road size')
xlabel('Road size')
ylabel('Time to find a solution (s)')

% Curve fitting
hold on
f = fit(n,t,'exp1'); % Alternatively, one can launch the Curve Fitting Toolbox
plot(f,n,t)

% Extend fitted curve on x-axis to [0,800]
x_fit = 0:800;
coefficients = coeffvalues(f); % Get fitted curve coefficients
y_fit = coefficients(1) * exp(coefficients(2)*x_fit); % Equation of fitted curve: a*exp(b*x)
figure(2)
plot(x_fit,y_fit)
xlim([0 850]) % Graph would look empty (due to exponential values) if no x-lim was applied
title('Time as a function of road size')
xlabel('Road size')
ylabel('Time to find a solution (s)')

% Print the value
fprintf('Time needed to solve the problem, with size 800: %e seconds or %e years\n',y_fit(end),y_fit(end)/3600)