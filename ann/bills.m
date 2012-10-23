close all;
clear all;

tmp = load('bills.txt');
[TS,VS,tmp] = splitSet(tmp,tmp(:,1),0.9,0.1);

K = length(unique(tmp(:,1)));
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

hiddenNeurons = [12,15,20];
iters = [600,1000];
%nn = createnn(size(X,2),hiddenNeurons,K);
compnn = @(nn1,X1)(classnn(nn1,X1));
errnn = @(nn1,X1,R1)(classerrnn(nn1,X1,R1));
%wv=[nn.W(:);nn.V(:)];
%nn = nntrain(nn,X,R,2000,compnn,errnn);
[nns,jval,jt] = nnmfit(X,R,Xv,Rv,2,iters,hiddenNeurons,errnn,compnn);
%jval
[r,c] = find( jval == min(min(jval)) )
nn = nns(r,c);
y = classnn(nn,X);


terrors=calcError(y,R,K,0.01);
%ym = max(y')'-0.01;
%Yp = (y >= (repmat(ym,1,K)));
%terrors = sum(sum(R ~= Yp))/2;

yv = classnn(nn,Xv);
verrors=calcError(yv,Rv,K,0.01);
%ym = max(yv')'-0.01;
%Yp = (yv >= (repmat(ym,1,K)));
%verrors = sum(sum(Rv ~= Yp))/2;

printf("Training Error: %f, Val. error: %f\n",terrors/N,verrors/Nv);
