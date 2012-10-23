function [err, Et] = calcError( Y, R, K, eps )
	[ym, idx]  = max(Y,[],2);
	Yp = (Y >= (repmat(ym-eps ,1, K)));
	err = sum(sum(R ~= Yp))/2;
	
	errorTable(Yp, R, K);
end
