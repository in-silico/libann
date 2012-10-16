function [E,grad] = nngrad(wv,nn,X,R,compnn,errnn)
    D = size(X,2);
    N = size(X,1);
    K = size(nn.V,1);
    H = size(nn.W,1);

    splitp = H*(D+1);
    w = wv(1:splitp);
    v = wv(splitp+1:end);
    nn.W = reshape(w,H,D+1);
    nn.V = reshape(v,K,H+1);
    W = nn.W;
    V = nn.V;
   
    [Y,Z] = compnn(nn,X);

    Zt = [ones(N,1) Z];
    deltaV = nngradV(Y,R,Zt,V); 

    Xt = [ones(N,1) X];
    deltaW = nngradW(Y,R,Z,V,Xt);

    E = errnn(nn,X,R);
    grad = [deltaW(:);deltaV(:)];
end
