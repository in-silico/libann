function [J] = errNumGrad(theta,nn,X,R,errnn)
    w = nn.W(:);
    v = nn.V(:);
    D = size(X,2);
    N = size(X,1);
    K = size(nn.V,1);
    H = size(nn.W,1);
    splitp = H*(D+1);
    nn.W = reshape(w,H,D+1);
    nn.V = reshape(v,K,H+1);

    J = errnn(nn,X,R);
end
