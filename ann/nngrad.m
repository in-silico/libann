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
   
    %{
    [Y,Z] = compnn(nn,X);

    Zt = [ones(N,1) Z];
    deltaV = nngradV(Y,R,Zt,V); 

    Xt = [ones(N,1) X];
    deltaW = nngradW(Y,R,Z,V,Xt);
    %}
    [a3,Z,a2,a1,z3,z2] = compnn(nn,X);
    d3 = a3 - R;
    d2 = d3 * V(:,2:end) .* sigmoidGradient(z2);

    deltaV = d3'*a2;
    deltaW = d2'*a1;

    E = errnn(nn,X,R);
    grad = [deltaW(:);deltaV(:)];
end
