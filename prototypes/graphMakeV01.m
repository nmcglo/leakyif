Layers = 3;

Populations = [2,10,1];

TotalNodes = sum(Populations);

W = zeros(TotalNodes);

for p = 1:length(Populations)-1
    if(p == 1)
        startndx = 1
        endndx = sum(Populations(1:p))
    else
        startndx = sum(Populations(1:p-1))+1
        endndx = sum(Populations(1:p))
    end

    for i = startndx:endndx
            for j = endndx+1:sum(Populations(1:p+1))
                W(i,j) = 1;
            end
    end
end
