clearvars
close
clc

iter=1e4;
n = load("n.mat").n;
res = zeros(100,4);

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