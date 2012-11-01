function [E] = classerrlr(X,W,R)
%CLASSERRNN Calculates the error function for multiple classes.

    Y = classlr(X,W);

	%Error fuction Binary Classif with One vs All
	%M = size(X,1);
	%E = 1/M * sum(R(:).*log(Y(:)) - (1-R(:)).*log(1-Y(:)));

	%Error function Multiclassif with Softmax
    %%%E = -sum(t(:).*log(Y(:)));
    s = R(:).*log(Y(:));
    E = -sum(s);
end
