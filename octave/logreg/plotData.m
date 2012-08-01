function plotData(X,y)
    figure; hold on;
    
    pos = X(find(y==1),:);
    neg = X(find(y==0),:);
    plot(pos(:,1),pos(:,2),'r+','MarkerSize',8);
    plot(neg(:,1),neg(:,2),'bo','MarkerSize',8);
    
    hold off;
end
