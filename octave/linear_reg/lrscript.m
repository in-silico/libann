y = load('mul.time');
n = length(y);
x = [1:n]';
plot(x,y,'rx');
hold on;
X=[ones(n,1) x x.^2];
w = linreg(X,y);

ny = X*w;
plot(x,ny);
