function w = linreg(X,y)
    Xi = pinv(X);
    w = Xi*y;
end
