function Y=quadratic(X)
	[m,n] = size(X);
	j=1; i=1;
	while (i<=n)
		Y(:,j++) = X(:,i);
		Y(:,j++) = X(:,i).^2;
		k=i+1;
		while (k<=n)
			Y(:,j++) = X(:,i).*X(:,k);
			k++;
		end
		i++;
	end
end
