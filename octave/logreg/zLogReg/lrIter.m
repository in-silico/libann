function [terr, verr, W] = lrIter(X, R, Xv, Rv, K, lambda)
%LRITER Run logistic regression for classification on the dataset [X,R]
%   [terr, verr, W] = lrIter(X, R, Xv, Rv, K, lambda) performs an iterative method 
%   for classification with logistic regression and then calclulates the error
%	onto the validation dataset [Xv, Rv]

	[M,D] = size(X);

	% Initialize parameters adding w0
	W = zeros(D + 1, K);
	W = W(:);

	%Set options for fminunc
	options = optimset('GradObj', 'on', 'MaxIter', 500);
	
	%Test of costFunctionReg [for debug]
	%[tJ, tgrad] = costFunctionReg(W, X, R, K, lambda);
    %grad2 = computeNumericalGradient(@(ww)(costFunctionReg(ww,X,R,K, lambda)),W);
	%grad2-tgrad

	%  Run fminunc to obtain the optimal weights
	[W, J] = fmincg(@(ww)(costFunctionReg(ww, X, R, K, lambda)), W, options);
	
	W = reshape(W, D+1, K);
	eps = 0.01;

	y = classlr(X,W);
	terr = calcError(y, R, K, eps);
	
	yv = classlr(Xv,W);
	verr = calcError(yv, Rv, K, eps);
end
