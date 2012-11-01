function [nn] = createnn(D,H,K)
	%CREATENN Creates a neural network with D and H neurons on the input and hidden
	%layer respectively and K outputs.

    nn.W = (rand(H,D+1)-0.5)*(2/100);
    nn.V = (rand(K,H+1)-0.5)*(2/100);
end
