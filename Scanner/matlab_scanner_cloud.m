FileID = fopen('Test.txt','r');
FormatSpec = '%f';

SizeA = [3 Inf];
ScanData = fscanf(FileID, FormatSpec, SizeA);

% %% Point Cloud
Xset = ScanData(1,:);
Yset = ScanData(2,:);
Zset = ScanData(3,:);

%tri = delaunay(Xset, Yset);
%trimesh(tri, Xset, Yset, Zset);

scatter3(Xset, Yset, Zset)

% %% Rotation angle
% Xset = ScanData(1,:);
% Yset = ScanData(2,:);
% 
% plot(Xset, Yset);