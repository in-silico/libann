
T = readColors();
T = double(T);
[Tn, mu, sigma] = featureNormalize(T);
k = 16;
tmp = randperm(length(Tn));
c = Tn(tmp(1:k),:);
%c = 4*rand(k,3) .- 2;
%Tn(1:10,:)

[c, idx] = runkMeans(Tn,c,10);
%idx = findClosestCentroids(Tn,c);

I = imread('test2.jpg');
figure(1);
imshow(I);


%cun = c.*sigma .+ mu;
cun = bsxfun(@times, c, sigma);
cun = bsxfun(@plus, cun, mu);

T2 = cun(idx,:);
I2 = ones(48,64);
for i = 1:3
    tmp2 = reshape(T2(:,i),48,64);
    I2(:,:,i) = tmp2;
end

figure(2);
imshow(uint8(I2));
pause;
close;
