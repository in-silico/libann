function T = readColors()
    %Read all the currency pictures of the ts folder and returns a N by 3 matrix containing the RGB
    %values of the pixels in the training set

    T = [];
    dirs = ls("ts");
    for i = 1:size(dirs,1)
    	d = dirs(i,:);
	p = sprintf("ts/%s",d);
	F = ls(p);
	for j = 1:size(F,1)
	    f = F(j,:);
	    p = sprintf("ts/%s/%s",d,f);
	    I = imread(p);
	    tmp = [ vec(I(:,:,1)), vec(I(:,:,2)), vec(I(:,:,3)) ];
	    T = [T;tmp];
	end
    end

end
