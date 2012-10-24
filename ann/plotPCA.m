function [S, sel, Sret] = plotPCA(X, idx, D)
%PLOTPCA Plots the D [2|3] first principal components of the dataset X 
%   [sel, Sret] = plotPCA(X, idx, D ) Calculates the PCA and projection
%   of the dataset X and
%   Returns the selected values to plot and the retained variance
%

	K = size(unique(idx),1);

	%Sample data to plot
	sel = randperm(size(X,1))';
	%sel = sel(1:200);

	%Subtract the mean to use PCA
	[X_norm, mu, sigma] = featureNormalize(X);

	% PCA and project the data to 2D
	[U, S, Sret] = pca(X_norm, D);
	Z = projectData(X_norm, U, D);
	printf('PCA Retained Variance: %f\n', Sret);

	% Plot in 2D
	figure;
	plotDataPoints(Z(sel, :), idx(sel)+1, K);
	pause;
end
