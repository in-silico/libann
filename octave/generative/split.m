function [ Xt,Xv,Tt,Tv ] = split(X, t, tper)
%UNTITLED4 Summary of this function goes here
%   Detailed explanation goes here
    Xt = [];
    Xv = [];
    Tt = [];
    Tv = [];
    for i = [0,1]
        tmp = X(t==i,:);
        n = size(tmp,1);
        nt = uint32(n*tper);
        rp = randperm(n);        
        Xt = [Xt;tmp(1:nt,:)];
        Xv = [Xv;tmp(nt+1:end,:)];
        Tt = [Tt;i*ones(nt,1)];
        Tv = [Tv;i*ones(n-nt,1)];
    end        

end

