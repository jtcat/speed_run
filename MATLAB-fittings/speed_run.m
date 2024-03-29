clear
close
clc

table = load("Path_to_file.txt"); % Change to appropriate file
n = table(:,1); % Road size
t = table(:,2); % Time to find a solution

% Curve fitting
figure(1)
f = fit(n,t,'exp1'); % Alternatively, one can launch the Curve Fitting Toolbox
plot(f,n,t,'b.')
xlabel('Road size')
ylabel('Time to find a solution (s)')
legend('Valores originais','Reta ajustada','Location','northwest')

% Extend fitted curve on x-axis to [0,800]
x_fit = 0:800;
coefficients = coeffvalues(f); % Get fitted curve coefficients
y_fit = coefficients(1) * exp(coefficients(2)*x_fit); % Equation of fitted curve: a*exp(b*x)
figure(2)
plot(x_fit,y_fit)
xlim([0 850]) % Graph would look empty (due to exponential values) if no x-lim was applied
xlabel('Road size')
ylabel('Time to find a solution (s)')

% Print the value
fprintf('Time needed to solve the problem, with size 800: %e seconds or %e years\n', y_fit(end), y_fit(end)/(3600*24*30*12))