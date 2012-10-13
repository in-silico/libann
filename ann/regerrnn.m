function [E] = regerrnn(nn,X,R)
    Y = regnn(nn,X);
    delta = R-Y;
    delta = delta(:);
    E = 0.5*delta'*delta;
end
