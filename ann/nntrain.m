function [nn2] = nntrain(nn,X,R,iter,compnn,errnn)
    w = nn.W(:);
    v = nn.V(:);
    D = size(X,2);
    N = size(X,1);
    K = size(nn.V,1);
    H = size(nn.W,1);

    splitp = H*(D+1);

    tmp = [w;v];
    %[j,grad1] = nngrad(tmp,nn,X,R,compnn,errnn)
    %grad2 = computeNumericalGradient(@(t)(errNumGrad(t,nn,X,R,errnn)),tmp)

    options = optimset('GradObj', 'on', 'MaxIter', iter);
    tic
    wv = fmincg(@(x)(nngrad(x,nn,X,R,compnn,errnn)),tmp,options);
    toc
    w = wv(1:splitp);
    v = wv(splitp+1:end);
    nn2.W = reshape(w,H,D+1);
    nn2.V = reshape(v,K,H+1);
end
