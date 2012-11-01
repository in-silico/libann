function s=sigmoid(v)
%SIGMOID Compute sigmoid functoon
%   J = SIGMOID(z) computes the sigmoid of z.

	s = 1.0 ./ (1.0 + exp(-v));
	
end
