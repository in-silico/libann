function Et = errorTable(Y, R, K)
	Et = zeros(K, K);

	for k=1:K	
		tmp = find(Y(:,k)==1);
		Et(k,:) = sum(R(tmp,:),1);
	end

	%dbstop; dbstep;
	for k=1:K
		precision(k) = Et(k,k) / sum(Et(k,:));
		recall(k) = Et(k,k) / sum(Et(:,k));
		f1score(k) = 2*precision(k)*recall(k) / (precision(k)+recall(k));
	end

	
	disp(Et);
	printf('Presicion: ');
	disp(precision);
	printf('Recall:    ');
	disp(recall);
	printf('F1Score:   ');
	disp(f1score);
end
