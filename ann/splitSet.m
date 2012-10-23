function [ Xtrain, XCVal, Xtest ] = splitSet( X, t, qTrain, qCVal )
%SPLITSET Splits a data set into 3 sets [training, cross_val, test]
%   The split is depending on the percentages specified by qTest and qCVal

    if qTrain + qCVal > 1 
        error('The percentages must add up to 1 max.');
    end

	ut = unique(t)';
	if length(ut) <= 0
		error('There must be at least one value in the t vector');
	end    
	
	Xtrain = [];
	XCVal = []; 
	Xtest = [];

	for i = ut
		tmp = X(t==i,:);
		n = size(tmp,1);
		randX = randperm(n)';
		nt = uint32(n*qTrain);
		nv = nt + uint32(n*qCVal);
	
		Xtrain = [Xtrain; tmp(randX(     1:nt,:),:)];
		XCVal  = [XCVal ; tmp(randX(nt + 1:nv,:),:)];
		Xtest  = [Xtest ; tmp(randX(nv + 1:end ,:),:)];
	end
end

