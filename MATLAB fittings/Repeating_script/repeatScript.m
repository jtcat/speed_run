clear
close
clc

iter=1e6;
n = load("n.mat").n;
res = zeros(100,4);

for sol = 2:4
    program_path = strcat('../../speed_run_',num2str(sol));
    for i=1:iter
        [~,cmdout] = system(program_path);
        temp = load(cmdout);
        found_zero = length(find(temp==0));
        if (found_zero == 1)
            while(found_zero == 1)
                [~,cmdout] = system(program_path);
                temp = load(cmdout);
                found_zero = length(find(temp==0));
            end
        end
        res(:,sol) = res(:,sol) + temp;
    end
    res(:,sol) = res(:,sol)./iter;
end