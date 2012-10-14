close all;
clear all;

load('class4');
K = 4;
N = size(X,1);
m = 50;
ix = randperm(N);
ixt = ix(1:m);
ixv = ix(m+1:end);

Xt = X(ixt,:);
Yt = Y(ixt,:);

nn = createnn(2,3,4);
compnn = @(nn1,X1)(classnn(nn1,X1));
errnn = @(nn1,X1,R1)(classerrnn(nn1,X1,R1));
wv=[nn.W(:);nn.V(:)];
[E,grad] = nngrad(wv,nn,Xt,Yt,compnn,errnn);
nn = nntrain(nn,X,Y,400,compnn,errnn);
y = classnn(nn,X);

ym = max(y')'-0.01;
Yp = (Y >= (repmat(ym,1,4)));
errors = sum(sum(Y ~= Yp))/2;
printf("Error: %f\n",errors/N);
