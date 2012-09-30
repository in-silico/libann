function [ W,x0,x1 ] = genCompParams(X, t)
    x1 = X(find(t==1),:);
    x0 = X(find(t==0),:);
    mu0 = mean(x0)';
    mu1 = mean(x1)';
    pi0 = length(x0)/length(X);
    pi1 = length(x1) / length(X);
    Sigma = pi0*cov(x0) + pi1*cov(x1);
    w = pinv(Sigma)*(mu1-mu0);
    w0 = -0.5*(mu1'*pinv(Sigma)*mu1) + 0.5*(mu0'*pinv(Sigma)*mu0) + log(pi1/pi0);
    W = [w0; w];
end

