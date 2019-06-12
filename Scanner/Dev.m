FileID1 = fopen('NKalmanRabbitYaw.txt','r');
% FileID2 = fopen('NoiseCameraDev2.txt','r');
FormatSpec = '%f';

SizeA = [2 Inf];
ScanData = fscanf(FileID1, FormatSpec, SizeA);
% ScanData2 = fscanf(FileID2, FormatSpec, SizeA);
% %% Point Cloud
% Yset = ScanData2(1,1:354);
plot(ScanData(1, :), ScanData(2, :));
% hold on;
% plot(Yset)
% 
% xlim([1, 354]);
% 
% legend("Clean", "Noise");
% 
% Std1 = std(Xset);
% Std2 = std(Yset);
% 
% title("\sigma_c : " + Std1 + ", \sigma_n : " + Std2);