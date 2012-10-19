function savenn(nn,file)
    W = nn.W;
    V = nn.V;
    save("-binary",file,"V","W");
end
