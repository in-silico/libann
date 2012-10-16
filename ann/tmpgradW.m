function deltaW = tmpgradW(Y,R,Z,V,Xt)
    [N,K] = size(Y);
    H = size(V,2)-1;
    D = size(Xt,2)-1;
    deltaW = zeros(H,D+1);
    for h = 1:H
	for j = 1:D+1
	    for t = 1:N
		tmp=0;
		for i = 1:K
		    tmp = tmp + (R(t,i) - Y(t,i))*V(i,h+1);
		end
		tmp2 = Z(t,h)*(1-Z(t,h))*Xt(t,j);
		deltaW(h,j) += tmp*tmp2;
	    end
	end
    end
    deltaW = deltaW*-1;
end
