function [E] = regerrnn(nn,X,R)
	%CLASSERRNN Calculates the error function for regression.
    
	Y = regnn(nn,X);
    delta = R-Y;
    delta = delta(:);
    E = 0.5*delta'*delta;
end
