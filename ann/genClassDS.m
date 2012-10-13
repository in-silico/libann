close all;
clear all;
D = 2; %input dims
K = 4; %number of classes
ncenters = 2; %number of centers per class
npts = 50; %number of points per center
s1 = 6; %variance of the centers around 0
s2 = 0.8; %variance of the points around each center

X = [];
Y = [];

hold on;
colors="rxbog+y*";
for k = 1:K
    y = zeros(1,K);
    y(k) = 1;
    y = repmat(y,npts,1);
    centers = normrnd(0,s1,ncenters,D);
    for c = 1:ncenters
	x = normrnd(repmat(centers(c,:),npts,1),s2*ones(npts,D));
	%if k==1
	%    plot(x(:,1),x(:,2),'rx');
	%else
	%    plot(x(:,1),x(:,2),'bo');
	%end
	plot(x(:,1),x(:,2),colors(2*k-1:2*k));
	X = [X;x];
	Y = [Y;y];
    end
end
