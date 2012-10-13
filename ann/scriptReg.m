clear all;
close all;
M=400;
N=10;

Xt = linspace(0,2*pi,M)';
tt = [sin(Xt),cos(Xt)];

ix = randperm(M);
ix = ix(1:N);
X = Xt(ix,:);
t = tt(ix,:) + (rand(N,size(tt,2))-0.5)*0.3;

nn = createnn(1,3,2);
compnn = @(nn1,X1)(regnn(nn1,X1));
errnn = @(nn1,X1,R1)(regerrnn(nn1,X1,R1));
wv=[nn.W(:);nn.V(:)];
[E,grad] = nngrad(wv,nn,X,t,compnn,errnn);
nn = nntrain(nn,X,t,400,compnn,errnn);
y = regnn(nn,Xt);

plot(Xt,tt);
hold on;
plot(X,t,'rx');
plot(Xt,y, 'r');
