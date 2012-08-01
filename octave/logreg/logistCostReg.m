function [J, grad] = logistCostReg(theta, X, y, lambda)

m = length(y); % number of training examples

h = sigmoid(X*theta);
e1 = -y.*log(h) - (1-y).*log(1-h);
regv = theta;
regv(1)=0;
J = (1/m)*sum(e1) + (lambda/(2*m))*regv'*regv;
grad = (1/m) * (X'*(h-y) + lambda*regv);

end
