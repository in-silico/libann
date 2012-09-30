close all;
clear;
load('ejemplo_class_dos');
t = (t+1)/2; %set second class to 0 instead of -1
[ Xt,Xv,Tt,Tv ] = split(X, t, 0.2);
[w,x0,x1] = genCompParams(Xt,Tt);
plot(x0(:,1),x0(:,2),'rx');
hold on;
plot(x1(:,1),x1(:,2),'bo');

%plot Boundary
tmp = linspace(-3,3,200);
tmpy = (-w(1) - w(2)*tmp)/w(3);
plot(tmp,tmpy,'g');

%Error
ta = predict(Xv,w);
error = sum(ta ~= Tv)
