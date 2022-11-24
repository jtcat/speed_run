clearvars
close
clc

iter=1e4;
n = load('n.mat').n;
res = zeros(100,4);

% Repeat and average values
for sol = 2:4
    program_path = strcat('../../speed_run_',num2str(sol));
    for i=1:iter
        [~,cmdout] = system(program_path);
        cmdout = splitlines(cmdout);
        cmdout = split(cmdout(1:100));
        cmdout = str2double(cmdout(:,4));
        res(:,sol) = res(:,sol) + cmdout;
    end
    res(:,sol) = res(:,sol)./iter;
end

% Plot values and fit curves
for sol = 2:4
    figure(sol)
%     plot(n,res(:,sol))
%     hold on
    f = fit(n,res(:,sol),'poly1');
    plot(f,n,res(:,sol),'b-')
%     title(['Solution ',num2str(sol)])
    xlabel('Road size')
    ylabel('Time to find a solution (s)')
    legend('Valores originais', 'Reta ajustada', 'Location','northwest')
    ylim([0 1e-5])
    coefficients = coeffvalues(f);
    fprintf('Solution %d: %e * x + %e\n', sol, coefficients(1), coefficients(2))
end