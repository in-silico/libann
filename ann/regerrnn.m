function [E] = regerrnn(nn,X,R)
    Y = regnn(nn,X);
    delta = R-Y;
    E = 0.5*delta'*delta;
end
