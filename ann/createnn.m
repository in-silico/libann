function [nn] = createnn(D,H,K)
    nn.W = (rand(H,D+1)-0.5)*(2/100);
    nn.V = (rand(K,H+1)-0.5)*(2/100);
end
