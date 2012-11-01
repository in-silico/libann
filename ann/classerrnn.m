function [E] = classerrnn(nn,X,R)
	%CLASSERRNN Calculates the error function for multiple classes.

    Y = classnn(nn,X);
    s = R(:).*log(Y(:));
    E = -sum(s);
end
