function plotDataPoints(X, idx, K)
%PLOTDATAPOINTS plots data points in X, coloring them so that those with the same
%index assignments in idx have the same color
%   PLOTDATAPOINTS(X, idx, K) plots data points in X, coloring them so that those 
%   with the same index assignments in idx have the same color

	D = size(X,2);
	if D~=2 && D~=3
		error('To plot data must be 2D or 3D');
	end

	% Create palette
	palette = hsv(K + 1);
	colors = palette(idx, :);

	% Plot the data
	if D==2
		scatter(X(:,1), X(:,2), 15, colors);
	else
		scatter3(X(:,1), X(:,2), X(:,3), 10, colors);
	end

end
