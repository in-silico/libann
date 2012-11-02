%Color histogram based NN classification of paper currency from an image.

close all;
clear all;

%Load the features of the bills dataset with the following format
%[classNumber F1 F2 .. F8]
%and splits it into a Training set and a Validation set
tmp = load('bills.txt');
[TS,VS,tmp1] = splitSet(tmp,tmp(:,1),0.9,0.1);

K = length(unique(TS(:,1)));
t1 = TS(:,1); t1v=VS(:,1);
X = TS(:,2:end); Xv=VS(:,2:end);
N = size(X,1); Nv=size(Xv,1);
R = zeros(N,K); Rv = zeros(Nv,K);
for n = 1:N
    R(n,t1(n)+1) = 1;
end
for n = 1:Nv
    Rv(n,t1v(n)+1) = 1;
end

%Dimensionality reduction for 2D plotting
%S = plotPCA(X, t1, 2);
lambda = 0;
for p=1:5

	%Create the phi matrix transforming the initial X values into poly features
	XtPoly = mapFPoly(X,p);
	XvPoly = mapFPoly(Xv,p);
	
	%For high order poly we need to normalize features in both the train/val
	if p > 1 
		[XtPoly, mu, sigma] = featureNormalize(XtPoly);
	
		XvPoly = bsxfun(@minus, XvPoly, mu);
		XvPoly = bsxfun(@rdivide, XvPoly, sigma);
	end

	[terr, verr] = lrIter(XtPoly, R, XvPoly, Rv, K, lambda);
	printf('***** LrIter PloyDegree[%d] TrainError: %f, ValError: %f *****\n',p,terr/N,verr/Nv);
end

