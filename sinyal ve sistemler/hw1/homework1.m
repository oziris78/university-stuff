
%% Oğuzhan Topaloğlu, Ç19052025, grup2
% BM icin sinyaller ve sistemler odev 1
% son teslim tarihi 30.11.2021

clear all
clc


%% soru 2 kismi

% VEKTOREL KARSILASTIRMA (SUNUMDAKI ORNEK)
xExample = [1 2 3];
xExampleZero = 2;
yExample = [1 2 3];
yExampleZero = 1;

[exampleConv, exampleConvZero] = myConvolution(xExample, ...
    xExampleZero, yExample, yExampleZero);
matlabExampleConv = conv(xExample, yExample);

exampleConv
matlabExampleConv
exampleConvZero % 2 olmali


% VEKTOREL KARSILASTIRMA (BENIM ORNEGIM)
xSignal = [-2 7 6 1 5];
ySignal = [9 4 0 3];

[myConv, myConvZero] = myConvolution(xSignal, 1, ySignal, 1);
matlabConv = conv(xSignal, ySignal);

myConv
matlabConv
myConvZero

% GRAFIKSEL KARSILASTIRMA (BENIM ORNEGIM)

tiledlayout(2,1)
nexttile
stem(myConv)
title('Benim Buldugum Sonuc')
nexttile
stem(matlabConv)
title('Hazir Fonksiyon Sonucu')



%% soru 3 kismi


% recObj = audiorecorder; calismadi diye boyle yazdim ayni seyler
recObj = audiorecorder(8000, 8, 1);


disp('5 saniye konusma basladi.')
recordblocking(recObj, 5);
disp('5 saniye konusma bitti.');
x1 = getaudiodata(recObj); % 40k terim

disp('10 saniye konusma basladi.')
recordblocking(recObj, 10);
disp('10 saniye konusma bitti.');
x2 = getaudiodata(recObj); % 80k terim


%% soru 4 kismi

x1_400 = 0.4 * [zeros(400,1) ; x1 ; zeros(400, 1)];
x1_800 = 0.4 * [zeros(800,1) ; x1];
x2_400 = 0.4 * [zeros(400,1) ; x2 ; zeros(400, 1)];
x2_800 = 0.4 * [zeros(800,1) ; x2];

y1 = [x1 ; zeros(800,1)] + x1_400 + x1_800;
y2 = [x2 ; zeros(800,1)] + x2_400 + x2_800;

myY1 = myConvolution(x1', 1, y1', 1)';
myY2 = myConvolution(x2', 1, y2', 1)';

matlabY1 = conv(x1, y1);
matlabY2 = conv(x2, y2);



%% soru 5 kismi

printPlayPrint('x1', x1, 5);
printPlayPrint('x2', x2, 5);
printPlayPrint('myY1', myY1, 10);
printPlayPrint('myY2', myY2, 10);
printPlayPrint('matlabY1', matlabY1, 10);
printPlayPrint('matlabY2', matlabY2, 10);



%% soru 1 kismi


function [convArr, convZero] = myConvolution(xSig, xZero, ySig, yZero)
    xLen = length(xSig);
    yLen = length(ySig); 
    convArr = zeros(1, xZero + yZero - 1);

    upper = [ zeros(1, yLen-1) xSig zeros(1, yLen-1) ];
    lower = [ fliplr(ySig) ];

    convZero = xZero + yZero - 1;
    for i = 1 : (xLen + yLen - 1)
        convArr(i) = myDotProduct(upper, lower);
        lower = [ 0 lower];
    end


end


% bu fonksiyon boyutlari ayni degilse min boyutu alacak 
% yani en sonda olmayan degerleri (sifirlari) gormezden gelecek
function dotProd = myDotProduct(arr1, arr2)
    dotProd = 0;
    for i = 1:min(length(arr1), length(arr2))
        dotProd = dotProd + ( arr1(i) * arr2(i) );
    end
end


% 5. soru icin
function [] = printPlayPrint(varName, soundArr, time)
    disp( [varName ' calindi' ]);
    sound(soundArr);
    pause(time)
end




