function [a3,Z,a2,a1,z3,z2] = classnn(nn,X)
    W = nn.W;
    V = nn.V;
    N = size(X,1);

    a1 = [ones(N,1) X];
    z2 = a1 * W';
    Z = sigmoid(z2);
    a2 = [ones(N,1) Z];
    z3 = a2 * V';

    %Apply softmax to U2
    a3 = exp(z3);
    a3 =  a3 ./ repmat(sum(a3,2),1,size(a3,2));
end
