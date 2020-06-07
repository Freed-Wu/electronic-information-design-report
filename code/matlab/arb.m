tic;
clc;
clear;
close;

%%in
N=1024;
fs=300;
%Hz
Ts=1/fs;
n=0:N-1;
fp=n/N*fs;
%上位机软件采集到的数据默认放在misc
[FileName,PathName] = uigetfile('../../misc/USB.dat','Select the USB.dat file');

%%process
f = fullfile(PathName,filesep,FileName);
fid = fopen(f,'r');
data = fscanf(fid,'%x');
fclose(fid);
data = data(1:2:end)*256 + data(2:2:end);%进制转换
data=data-mean(data);%波形下移
dataal=data;
m=max(data);
n=min(data);
data=data/(m-n);%量化
datsgn1 = data;
plot(datsgn1);     %时域波形
xlabel('采样点');
ylabel('电压/mV');
title('量化后的心电图');
saveas(gcf,'../../fig/arb1.png')

F1=fft(datsgn1);
[pks1,locs1] = findpeaks(datsgn1,'MINPEAKDISTANCE',50,'MINPEAKHEIGHT',0.2);
dis1=diff(locs1);
avrdis1=mean(dis1);
avrpks1=mean(pks1);
beat1=round(fs/avrdis1*60);
fprintf('加窗前此人的心率为%fbpm\n', beat1);
fprintf('加窗前心率的最大值为%fmV\n', avrpks1);
%加滤波器
fp=0.05;fc=100;As=80;Ap=1;Fs=300;
wc=2*pi*fc/Fs;
wp=2*pi*fp/Fs;
wd=wc-wp;
beta=0.1102*(As-8.7);
N=ceil((As-7.95)/2.286/wd);
wn=kaiser(N+1,beta);
ws=(wp+wc)/2/pi;
b=fir1(N,ws,wn);
datsgn2=fftfilt(b,data);
F2=fft(datsgn2);
figure;
plot(datsgn2);
xlabel('采样点');
ylabel('电压/mV');
title('加窗后的心电图');
saveas(gcf,'../../fig/arb2.png')

[pks,locs] = findpeaks(datsgn2,'MINPEAKDISTANCE',50,'MINPEAKHEIGHT',0.2);
dis=diff(locs);
avrdis=mean(dis);
beat=round(fs/avrdis*60);
fprintf('此人的心率为%fbpm\n', beat);
fprintf('心率的周期为%fHz\n', fs/avrdis);
%计算信噪比
pdatsgn1=datsgn1.*datsgn1;
pdatsgn2=datsgn2.*datsgn2;
snr=sum(pdatsgn2)/(sum(pdatsgn1)-sum(pdatsgn2));
fprintf('心电图的信噪比为%f\n', snr);
%Q波
dat=[datsgn2(85:95)];
Q1=mean(find(diff(sign(diff(dat)))>0)+85);
dat=[datsgn2(280:290)];
Q2=mean(find(diff(sign(diff(dat)))>0)+280);
dat=[datsgn2(475:485)];
Q3=mean(find(diff(sign(diff(dat)))>0)+475);
dat=[datsgn2(670:680)];
Q4=mean(find(diff(sign(diff(dat)))>0)+670);
dat=[datsgn2(860:870)];
Q5=mean(find(diff(sign(diff(dat)))>0)+860);
avrQ=(Q1+Q2+Q3+Q4+Q5)/5;
%S波
dat=[datsgn2(105:115)];
S1=mean(find(diff(sign(diff(dat)))>0)+105);
dat=[datsgn2(300:310)];
S2=mean(find(diff(sign(diff(dat)))>0)+300);
dat=[datsgn2(495:505)];
S3=mean(find(diff(sign(diff(dat)))>0)+495);
dat=[datsgn2(690:700)];
S4=mean(find(diff(sign(diff(dat)))>0)+690);
dat=[datsgn2(885:895)];
S5=mean(find(diff(sign(diff(dat)))>0)+885);
avrS=(S1+S2+S3+S4+S5)/5;
%R波
dat=[datsgn2(95:105)];
R1=mean(find(diff(sign(diff(dat)))<0)+95);
dat=[datsgn2(290:300)];
R2=mean(find(diff(sign(diff(dat)))<0)+290);
dat=[datsgn2(485:495)];
R3=mean(find(diff(sign(diff(dat)))<0)+485);
dat=[datsgn2(680:690)];
R4=mean(find(diff(sign(diff(dat)))<0)+680);
dat=[datsgn2(875:885)];
R5=mean(find(diff(sign(diff(dat)))<0)+875);
avrR=(R1+R2+R3+R4+R5)/5;
%P波
dat=[datsgn2(60:70)];
P1=mean(find(diff(sign(diff(dat)))<0)+60);
dat=[datsgn2(260:270)];
P2=mean(find(diff(sign(diff(dat)))<0)+260);
dat=[datsgn2(455:465)];
P3=mean(find(diff(sign(diff(dat)))<0)+455);
dat=[datsgn2(650:660)];
P4=mean(find(diff(sign(diff(dat)))<0)+650);
dat=[datsgn2(845:855)];
P5=mean(find(diff(sign(diff(dat)))<0)+845);
avrP=(P1+P2+P3+P4+P5)/5;
%T波
dat=[datsgn2(145:155)];
T1=mean(find(diff(sign(diff(dat)))<0)+145);
dat=[datsgn2(340:350)];
T2=mean(find(diff(sign(diff(dat)))<0)+340);
dat=[datsgn2(535:545)];
T3=mean(find(diff(sign(diff(dat)))<0)+535);
dat=[datsgn2(730:740)];
T4=mean(find(diff(sign(diff(dat)))<0)+730);
dat=[datsgn2(925:935)];
T5=mean(find(diff(sign(diff(dat)))<0)+925);
avrT=(T1+T2+T3+T4+T5)/5;

%%out
fprintf('此人的PR间期为:%fs\n', (avrR-avrP)/fs);
fprintf('此人的QRS宽度为:%fs\n', (avrS-avrQ)/fs);
fprintf('此人的QT宽度为:%fs\n', (avrT-avrQ)/fs);
fprintf('此人的ST宽度为:%fs\n', (avrT-avrS)/fs);
fprintf('此人的P点幅值为:%fmV\n', datsgn2(P1));
fprintf('此人的Q点幅值为:%fmV\n', datsgn2(Q1));
fprintf('此人的R点幅值为:%fmV\n', datsgn2(R1));
fprintf('此人的S点幅值为:%fmV\n', datsgn2(S1));
fprintf('此人的T点幅值为:%fmV\n', datsgn2(T1));

fprintf('Running time is %f second.\n', toc)

