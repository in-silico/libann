function [nn] = loadnn(file) 
    load(file);
    nn.V = V;
    nn.W = W;
end
