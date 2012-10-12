function [E,grad] = nngrad(wv,nn,X,R,compnn,errnn)
    D = size(X,2);
    N = size(X,1);
    K = size(nn.V,1);
    H = size(nn.W,1);

    splitp = H*(D+1);
    w = wv(1:splitp);
    v = wv(splitp+1:end);
    nn.W = reshape(w,H,D+1);
    nn.V = reshape(v,K,H+1);
    W = nn.W;
    V = nn.V;
   
    [Y,Z] = compnn(nn,X);

    Zt = [ones(N,1) Z];
    deltaV = zeros(K,H+1);
    for i = 1:K
	for h = 1:(H+1)
	    for t = 1:N
		tmp = (Y(t,i) - R(t,i)) * Zt(t,h);
		deltaV(i,h) = deltaV(i,h) + tmp;
	    end
	end
    end

    Xt = [ones(N,1) X];
    deltaW = zeros(H,D+1);
    for h = 1:H
	for j = 1:D+1
	    for t = 1:N
		tmp=0;
		for i = 1:K
		    tmp = tmp + (Y(t,i) - R(t,i))*V(i,h);
		end
		tmp2 = Z(t,h)*(1-Z(t,h))*Xt(t,j);
		deltaW += tmp*tmp2;
	    end
	end
    end

    E = errnn(nn,X,R);
    grad = [deltaW(:);deltaV(:)];
end
