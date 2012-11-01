function [J] = errNumGrad(theta,nn,X,R,errnn)
	%ERRNUMGRAD Calculates the error function given by errnn for 
	%the neural network.

    D = size(X,2);
    N = size(X,1);
    K = size(nn.V,1);
    H = size(nn.W,1);
    splitp = H*(D+1);
    w = theta(1:splitp);
    v = theta(splitp+1:end);
    nn.W = reshape(w,H,D+1);
    nn.V = reshape(v,K,H+1);

    J = errnn(nn,X,R);
end
