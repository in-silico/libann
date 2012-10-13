function [Y,Z] = classnn(nn,X)
    W = nn.W;
    V = nn.V;

    N = size(X,1);
    Xr = [ones(N,1) X];
    U1 = Xr*W';
    Z = sigmoid(U1);
    Zr = [ones(N,1) Z];
    U2 = Zr*V';

    %Apply softmax to U2
    Y = exp(U2);
    Y = Y ./ repmat(sum(Y,2),1,size(Y,2));
end
