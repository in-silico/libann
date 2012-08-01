function [theta] = logistClassif(X,y)
	
	%Initialization
	[m,d] = size(X);
	
	%Split the set
	%rp = randperm(m);
	s1 = floor(0.7*m);
	s2 = floor(0.85*m);
	Xt = [ones(s1,1) X(1:s1,:)]; yt=y(1:s1,:);
	Xv = [ones(s2-s1,1) X(s1+1:s2,:)]; yv=y(s1+1:s2,:);
	Xtest = [ones(m-s2,1) X(s2+1:end,:)]; ytest=y(s2+1:end,:);
	d = d+1; %Bias term
	
	%Train the logistic regression with several values of lambda
	lambdas = [0,0.1,0.3,0.5,1,3,5,10,30];
	best = 0; lambda=0; theta=0;
	%Options for gradient calculation
	options = optimset('GradObj', 'on', 'MaxIter', 400); 
	
	for i = 1:length(lambdas)
		lambda = lambdas(i);
		[a_theta, J] = fminunc(@(t)(logistCostReg(t, Xt, yt, lambda)), rand(d,1), options);
		%[a_theta, J] = fminunc(@(t)(sebCost1(t, Xt, yt, lambda)), rand(d,1), options);
		act = sum( (Xv*a_theta > 0)==yv );		
		if (act > best)
			best = act;
			theta = a_theta;
		end
		pval = act/length(yv); ptrain = sum( (Xt*a_theta > 0)==yt )/length(yt);
		fprintf('Accuracy-training=%f%% \t Accuracy-val=%f%% lambda=%f\n',100*ptrain,100*pval,lambda);
	end
	
	%Compute the classification error on training, validation and test set
	%First compute the predicted values for training, val and test
	pt = (sigmoid(Xt*theta)>0.5); per_t = 100*sum(pt==yt)/length(yt);
	pv = (sigmoid(Xv*theta)>0.5); per_v = 100*sum(pv==yv)/length(yv);
	ptest = (sigmoid(Xtest*theta)>0.5); per_test = 100*sum(ptest==ytest)/length(ytest);
	fprintf('Accuracy: Training=%f%% \t Validation=%f%% \t Test=%f%%\n', per_t, per_v, per_test);
end
