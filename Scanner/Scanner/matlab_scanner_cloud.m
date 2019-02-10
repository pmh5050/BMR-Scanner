FileID = fopen('PointCloud.txt','r');
FormatSpec = '%f';

SizeA = [3 Inf];
ScanData = fscanf(FileID, FormatSpec, SizeA);

Xset = ScanData(1,:);
Yset = ScanData(2,:);
Zset = ScanData(3,:);

%tri = delaunay(Xset, Yset);
%trimesh(tri, Xset, Yset, Zset);

scatter3(Xset, Yset, Zset)