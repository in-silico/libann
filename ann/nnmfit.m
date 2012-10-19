function [nns,Jvals,Jtrain] = nnmfit(Xt,Yt,Xv,Yv,ntimes,iter,hidden,errnn,compnn)
    % Computes the neural networks and errors on validation set for different neural networks
    % with different ammount of hidden units and iteration values. For each value in iter and
    % hidden it runs the neural network training algorithm ntimes times and the one with less
    % error on the training set is appended to nns matrix, and the validation error is appended
    % on Jvals matrix

    [N,D] = size(Xt);
    K = size(Yt,2);

    nhv = length(hidden);
    niv = length(iter);
    Jvals = Jtrain = zeros(niv,nhv);
    for i = 1:niv
	for h = 1:nhv
	    jtm = 1e100; %infinity
	    nnm = 0;
	    for j = 1:ntimes
		nn = createnn(D,hidden(h),K);
		nn = nntrain(nn,Xt,Yt,iter(i),compnn,errnn);
		jt = errnn(nn,Xt,Yt);
		if (jt < jtm)
		    jtm = jt;
		    nnm = nn;
		end
	    end
	    nns(i,h) = nnm; 
	    Jtrain(i,h) = jtm;
	    Jvals(i,h) = errnn(nnm,Xv,Yv);
	end
    end		

end
