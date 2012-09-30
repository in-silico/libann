function [ ta ] = predict( X,w )
%UNTITLED5 Summary of this function goes here
%   Detailed explanation goes here
    n = size(X,1);
    X=[ones(n,1) X];
    ta = (sigmoid(X*w)>0.5);

end

