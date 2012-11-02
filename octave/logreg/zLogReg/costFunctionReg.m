function [J, grad] = costFunctionReg(W, X, t, K, lambda)
%COSTFUNCTIONREG Compute cost and gradient for logistic regression with regularization
%   J = COSTFUNCTIONREG(W, X, y, lambda) computes the cost of using
%   W as the parameter for regularized logistic regression and the
%   gradient of the cost.

	% Number of training examples
	[M,D] = size(X);

	W = reshape(W, D+1, K); 
	
	J = classerrlr(X,W,t);
	regJ = lambda * sum(sum(W(2:end,:).^2));
	%regJ = regJ / (2*M);
	J = J + regJ;
	
	h = classlr(X,W);
	X = [ones(M,1) X];
	%Gradient for binary classif
	%grad = 1/M * X'*(h-t);
	%regG = lambda/M * W(2:end,:);	
	%Gradient for multiclass
	grad = X'*(h-t);
	regG = lambda/2 * W(2:end,:);
	
	grad(2:end,:) = grad(2:end,:) + regG;

	grad = grad(:);
end
