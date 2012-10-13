function [E] = classerrnn(nn,X,R)
    Y = classnn(nn,X);
    s = R(:).*log(Y(:));
    E = sum(s);
end
