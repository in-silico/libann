function p = predict(theta, X)
    prob = sigmoid(X*theta);
    p = (prob >= 0.5);
end
