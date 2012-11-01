function Y = classlr(X, W)
%CLASSLR calculates the output for the multi classification linear regression
%given by the softmax function or the sigmoid function.

	X = [ones(size(X,1), 1) X];

	%Testing sigmoid for binary classif
	%Y = sigmoid(X*W);

	%Softmax for multiple classes
    Y = exp(X*W);
    Y = Y ./ repmat(sum(Y,2),1,size(Y,2));
end
