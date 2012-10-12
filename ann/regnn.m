function [Y,Z] = regnn(nn,X)
    W = nn.W;
    V = nn.V;

    N = size(X,1);
    Xr = [ones(N,1) X];
    U = Xr*W';
    Z = sigmoid(U);
    Zr = [ones(N,1) Z];
    Y = Zr*V';
end
