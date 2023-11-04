

%%
%{
--- ONEMLI NOT ---
KODUN DOGRU CALISMASI ICIN BU DOSYANIN (C19052025.m) BULUNDUGU DIZINDE
"Data" ISMINDE BIR KLASORUN OLMASI VE BU KLASORUN ICINDE 
"my_oil_tank.png" OLMASI GEREKMEKTEDIR

BU KOD DATA KLASORUNUN VE TANKER GORSELININ PATH'INI ISTEMEK YERINE
BULUNDUGU DIZINDE DATA KLASORU ARAMASI YAPAR VE BIRDEN 
COK KEZ CALISTIRMADA GEREKSIZ PATH GIRMESININ ONUNE GECER
%}

%% Giris kismi

clc; clear all; close all;

fprintf("\n---         OGUZHAN TOPALOGLU        ---\n");
fprintf("---   C19052025 (CAP OGRENCISIYIM)   ---\n\n");

soru1(100);
soru2();

fprintf("--- PROGRAM SONU ---\n\n");



%% Soru 1

function soru1(tryCount)
    for counter = 1 : tryCount
        A = rand(100); % 100x100
        B = rand(20); % 20x20
        C1 = conv2(A, B, 'valid');
        C2 = myConv2(A, B);
        errorMatrix = abs(C1 - C2);
        % Ahmet hocam mailde 10^-10'dan az hatanin olusu sorun olmaz dedi
        if any(errorMatrix(:) > 1E-10) % 1E-12'de de calisiyor 1E-13 olmuyor
            error("FONKSIYON YANLIS CALISIYOR");
        end
    end
    fprintf("SORU1 FONKSIYONU DOGRU CALISIYOR (%d KEZ DENENDI)\n\n\n", counter);
end


function C = myConv2(A, B)
    [rowA, colA] = size(A);
    [rowB, colB] = size(B);
    
    rowC = rowA - rowB + 1;
    colC = colA - colB + 1;
    C = zeros(rowC, colC);

    B = rot90(B, 2);
    
    for i = 1:rowC
        for j = 1:colC
            C(i,j) = sum(sum(A(i:i+rowB-1, j:j+colB-1) .* B));
        end
    end
end



%% Soru 2

function soru2()
    % Data klasorunden .jpg ve .jpeg dosyalarini elde etme
    imagePaths = {};
    files = dir('Data/*.jp*g');  % .jp(e)g oldugundan regex
    if length(files) == 0 % EGER KLASOR YOKSA KULLANICIYI YONLENDIR
        error(sprintf( ...
            "LUTFEN DOSYANIN EN USTUNDEKI ACIKLAMAYI OKUYUNUZ\n" + ...
            "LUTFEN DOSYANIN EN USTUNDEKI ACIKLAMAYI OKUYUNUZ\n" + ...
            "LUTFEN DOSYANIN EN USTUNDEKI ACIKLAMAYI OKUYUNUZ\n"));
    end
    for i = 1:length(files)
        imagePaths{end+1} = files(i).name;
    end

    % Paint'ten alinan yag tankeri gorselini yukleme
    oilTankPixels = rgb2gray(imread("Data/my_oil_tank.png"));
    oilTankPixels = imresize(oilTankPixels, 0.005);
    
    fprintf('%10s%10s%10s%10s%10s\n', "Resim", "Sayi", "conv2", ... 
        "myConv2", "xcorr2");

    for i = 1 : length(imagePaths)
        currentImagePath = string(strcat('Data/', imagePaths(i)));
        notResized = rgb2gray(imread(currentImagePath));
        img = imresize(notResized, 0.005); % izin verilmemis olabilir?

        % Dosyanin isminden gercek sayiyi elde et
        actualTankCount = split(currentImagePath, '_');
        actualTankCount = actualTankCount(end, 1);
        actualTankCount = split(actualTankCount, '.');
        actualTankCount = actualTankCount(1, 1);
        
        % Konvolusyon islemleri ve tabloya veri kaydetme
        c1 = conv2(img, oilTankPixels, 'valid');
        c2 = myConv2(img, oilTankPixels);
        % corr2 yerine xcorr2 cunku boyutlar farkli
        c3 = xcorr2(img, oilTankPixels); 
    
        graphTitle = sprintf("%s - %s", string(imagePaths(i)), 'conv2');
        found1 = calculateAndDrawFor(graphTitle, notResized, c1);
        graphTitle = sprintf("%s - %s", string(imagePaths(i)), 'myConv2');
        found2 = calculateAndDrawFor(graphTitle, notResized, c2);
        graphTitle = sprintf("%s - %s", string(imagePaths(i)), 'corr2d');
        found3 = calculateAndDrawFor(graphTitle, notResized, c3);
        
        fileName = split(string(imagePaths(i)), '.');
        fprintf('%10s%10s%10d%10d%10d\n', fileName(1, 1), ...
            actualTankCount, found1, found2, found3);
    end
    fprintf("\n\n");

end



function [foundOilTanks] = calculateAndDrawFor(graphTitle, bigImage, c)
    % x ve y koordinatlari icin cizimde kullanmak icin oran hesabi
    [cR, cC] = size(c);
    [iR, iC] = size(bigImage);
    scaleR = iR/cR;
    scaleC = iC/cC;

    % Peak detection algoritmasi (findpeaks ve CCA'den daha iyi gibi)
    threshold = 0.8;
    [y,x] = find(c > threshold * max(c(:)));
    foundOilTanks = length(x);
    
    % Grafik cizimi
    figure;
    imshow(bigImage);
    hold on;
    title(graphTitle);
    for j = 1 : foundOilTanks
        % paint'ten aldigim gorsel 57x57 oldugundan 57
        posVec = [(x(j)-1) * scaleC, (y(j)-1) * scaleR, 57, 57];
        rectangle('Position', posVec, 'EdgeColor', 'magenta');
    end
   
end



