function Et = errorTable(Y, R, K)
	%ERRORTABLE Given the output of the classification system (Y), the real 
	%output(R) and the number of classes (K), calculates an evaluation metric 
	%table given by the count of the number of true positives, false positives, 
	%false negatives and true negatives. Based on this results, the precision, 
	%recall, and F1Score are calculated.  
	
	Et = zeros(K, K);

	for k=1:K	
		tmp = find(Y(:,k)==1);
		Et(k,:) = sum(R(tmp,:),1);
	end

	%dbstop; dbstep;
	for k=1:K
		%Of all the predicted values, how many are correctly classified
		%Precision = True Positives / #Predicted Positive
		precision(k) = Et(k,k) / sum(Et(k,:));
		%For each class, how many are correctly predicted
		%Recall = True Positives / #Actual Positives
		recall(k) = Et(k,k) / sum(Et(:,k));
		%A mergin metric for Precision and Recall
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
