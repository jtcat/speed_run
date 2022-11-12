clear
close
clc

table = load("speed_run.txt");
n = table(1:end,1); % Tamanho da estrada
t = table(1:end,2); % Tempo para determinar a solução

% Plot original dos dados
plot(n,t)
legend('Curva original')
title('Tempo em função do tamanho')
xlabel('Tamanho da estrada')
ylabel('Tempo para determinar a solução')

% Aproximação
hold on
f = fit(n,t,'exp1');
plot(f,n,t)