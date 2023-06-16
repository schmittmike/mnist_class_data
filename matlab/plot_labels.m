output_count = 4; % number of possible outputs (labels)
input_count = 3; % number of possible inputs (statistics)

% i set up symlinks in the current directory so that I wouldn't have to copy
% these back and forth every time they changed, you could also just give this
% the path to the actual data.
data_train = importdata("stat_train.dat", "\t");
data_test = importdata("stat_test.dat", "\t");

train_stats = {};

figure(1);
map =  [0 .8 .8
	.5 0 0
	1 0 .5
	0 0 0];
colormap(map);
for i = 1:length(data_train(:,1)) % for each line in the training data
	for j = 1:output_count % check which is true output
		if data_train(i, 3+j) == 1
			train_stats{end+1, 1} = [data_train(i, 1) ...
                                                 data_train(i, 2) ...
                                                 data_train(i, 3) ...
                                                 20 ...
						 j];
			%fprintf("data1: %f\n", data_train(i, 1));
			%fprintf("data2: %f\n", data_train(i, 2));
			%fprintf("data3: %f\n", data_train(i, 3));
		end
	end
end

train_stats = cell2mat(train_stats);
scatter3(train_stats(:, 1), ...
	 train_stats(:, 2), ...
	 train_stats(:, 3), ...
	 train_stats(:, 4), ...
	 train_stats(:, 5));


%% LEFTOVER FROM 2D: SHOULD RECREATE BUT BETTER
% plot the test data and color each label differently

%figure(2);
%hold on;
%grid on;
%title("test data");

test_stddev_ones = [];
test_stddev_zeros = [];
test_avg_ones = [];
test_avg_zeros = [];
for i = 1:length(data_test(:, 1))
	if data_test(i, 4) == 1
                test_stddev_ones(end + 1) = data_test(i, 1);
                test_avg_ones(end + 1) = data_test(i, 2);
	else
                test_stddev_zeros(end + 1) = data_test(i, 1);
                test_avg_zeros(end + 1) = data_test(i, 2);
	end
end

%scatter(test_stddev_ones, test_avg_ones, "ro");
%scatter(test_stddev_zeros, test_avg_zeros, "bo");


%% PLOT THE NEURAL MODEL OUTPUT
% get a value out of the model for each reasonable input, and plot in scatter and 3d

n = 50;
x1 = linspace(0.5e4, 3e4, n);
x2 = linspace(0, 300, n);
y = zeros(1, n);
p = zeros(2, n^2);
v = zeros(n, n, 2);

hold on;
for i = 1:n
        for j = 1:n
                y = class_try1([x1(i), x2(j)]);
                if y(1) > 0.55
                                %scatter(x1(i), x2(j), "ro");
                else    if y(1) < .9 && y(1) > 0.1
                                %scatter(x1(i), x2(j), "blackx");
                        else
                                %scatter(x1(i), x2(j), "go");
                        end
                end
                p(1, (n-1)*i + j) = x1(i);
                p(2, (n-1)*i + j) = x2(j);
                v(i, j, 1) = y(1);
                v(i, j, 2) = y(2);
        end
end

%figure(3);
%hold off;
%surfc(x2, x1, v(:, :, 1))
%title("probability to be a 0");
%figure(4);
%hold off;
%surfc(x2, x1, v(:, :, 2))
%title("probability to be a 1");
