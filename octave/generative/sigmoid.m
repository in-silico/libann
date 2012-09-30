function [ y ] = sigmoid( x )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
    y = 1 ./ (1 + exp(-x));

end

