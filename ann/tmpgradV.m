function [deltaV] = tmpgradV(Y,R,Zt,V)
    K = size(Y,2);
    H = size(V,2)-1;
    N = size(Y,1);

    deltaV = zeros(K,H+1);
    for i = 1:K
	for h = 1:(H+1)
	    for t = 1:N
		tmp = (Y(t,i) - R(t,i)) * Zt(t,h);
		deltaV(i,h) = deltaV(i,h) + tmp;
	    end
	end
    end
end
