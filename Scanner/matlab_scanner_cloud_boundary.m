FileID = fopen('PointCloud13.txt','r');
FormatSpec = '%f';

SizeA = [3 Inf];
ScanData = fscanf(FileID, FormatSpec, SizeA);

% %% Point Cloud
Xset = ScanData(1,:);
Yset = ScanData(2,:);
Zset = ScanData(3,:);

PointLength = length(Xset);

NewXset = zeros(1, PointLength);
NewYset = zeros(1, PointLength);
NewZset = zeros(1, PointLength);

BoundXAbs = 0.05; % Unit m
BoundYAbs = 0.05; % Unit m

BoundZMin = 0.00174;
BoundZMax = 1.0; % Unit m

PointCounter = 0;

bIsFirst = true;
for i = 1 : PointLength
    bInnerBound = true;
    if ( (abs(Xset(i) - 0.1) > BoundXAbs) || (abs(Yset(i) - 0.1) > BoundYAbs) || (BoundZMin > Zset(i)) || (Zset(i) > BoundZMax) )
        bInnerBound = false;
    end
    
    if bInnerBound
        PointCounter = PointCounter + 1;
        NewXset(PointCounter) = Xset(i);
        NewYset(PointCounter) = Yset(i);
        NewZset(PointCounter) = Zset(i);
        
        if bIsFirst
            i
            bIsFirst = false;
        end
    end
    
end

% scatter3(Xset, Yset, Zset)
scatter3(NewXset(1:PointCounter), NewYset(1:PointCounter), NewZset(1:PointCounter))

%% File writing
OutputFileID = fopen('PostProcessing3.xyz', 'w');

fprintf(OutputFileID, '%d\n', PointCounter);

for i = 1 : PointCounter
    fprintf(OutputFileID, '%f %f %f \n', NewXset(i), NewYset(i), NewZset(i));
end
fclose(OutputFileID);