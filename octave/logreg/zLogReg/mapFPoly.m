function PHI = mapFPoly(X, p)
%MAPFPOLY Transforms the vector X into a vector PHI given by the
%polynomial features of order p.	
	[M,D] = size(X);
	PHI = zeros(M, p*D);

	icol = 0;
    for i = 1:p
        PHI(:,icol+1:i*D) = X.^i;
		icol = i*D;
    end
end
