


clear all;


fprintf("-----------------------------------------------\n");
fprintf("OGUZHAN TOPALOGLU, C19052025 (CAP OGRENCISIYIM)\n");
fprintf("SAYISAL ISARET ISLEME - ODEV 1\n");
fprintf("-----------------------------------------------\n");
fprintf("\n\n---------------- SORU 1 BASLANGICI ----------------\n");
soru1();
fprintf("\n---------------- SORU 1 SONU ----------------\n\n");
fprintf("\n\n---------------- SORU 2 BASLANGICI ----------------\n");
soru2();
fprintf("\n---------------- SORU 2 SONU ----------------\n\n");





function soru1()
    k = input('Kac terimli Fourier serisi hesaplansin?: ');

    a = zeros(1, k);
    for i=1:k
        real = input(sprintf("a(%d)'nin reel kismi: ", i));
        comp = input(sprintf("a(%d)'nin kompleks kismi: ", i));
        a(i) = real + 1j * comp;
    end

    %{
    t = 0:1/100000:5;
    fourier = 0;
    for i=1:k
        fourier = fourier + ...
            a(i) * exp(1j * i * 2 * pi * t);  % e ^ j n w t
    end
    %}

    freqs = zeros(1, k);
    scales = zeros(1, k);
    
    for i = 1:k
        freqs(i) = i;
        scales(i) = abs(a(i));
    end

    % diyelim ki [3 1 2] [4.47 12.04 4.12] verildi
    % 4.47 sin(3t) + 12.04 sin(1t) + 4.12 sin(2t)  yapilmali
    % ancak 4.47 sin(3t)/3pi + 12.04 sin(1t)/1pi + 4.12 sin(2t)/2pi olacak
    % bu yuzden scales(i) = scales(i) * freqs(i) * pi yapilmali
    for i = 1:k
        scales(i) = scales(i) * freqs(i) * pi;
    end
    drawAnimationFor(freqs, scales);
    
end




function soru2()
    N = input('Sinyal sayisini (N) giriniz: ');
    A = zeros(1, N);
    f = zeros(1, N);
    phi = zeros(1, N);
    
    % Degerlerin okunmasi ve soruda cos denildiginden pi/2 eklenerek sin 
    % fonksiyonlarinin elde edilmesi
    for i = 1:N
        fprintf('\nSinyal #%d icin deger giriniz:\n', i);
        A(i) = input('  - Genlik: ');
        f(i) = input('  - Frekans: ');
        phi(i) = input('  - Faz: ');
        % Her terim icin cos(2*pi*f + phi) olsun diye phi acisina pi/2 ekle ve
        % sin(2*pi*f + phi + pi/2) olsun cunku sin(x + pi/2) = cos(x)
        phi(i) = phi(i) + pi/2;
    end
    
    % Degerlerin gerekli donusumlerle drawAnimationFor fonksiyonuna konulmasi
    for i = 1:N
        c = A(i);
        k = 2*pi*f(i) + phi(i);
        % c sin(k) cizilmeli ancak drawAnimationFor([k], [c]) yazarsam
        % c * sin(k) / (kpi) cizilecek. Bu yuzden c yerine c * k * pi 
        % veririm ve c*k*pi * sin(k) / (kpi) = c * sin(k) cizilmis olur :)
        % yani c yerine c*k*pi verilmeli, c'yi carpip guncelle
        c = c * k * pi;
        % simdi de drawAnimationFor([k_i, ...], [c_i, ...]) yaz
        freqs(i) = k;
        scales(i) = c;
        drawAnimationFor(freqs, scales);
    end
end





% Bu fonksiyon, freq = [1 3 5] ve scale=[2 7 9] ise 
% 2*sin(1x)/(1pi) + 7*sin(3x)/(3pi) + 9*sin(5x)/(5pi) serisini simule eder
function drawAnimationFor(freqs, scales)
    % Genel degiskenler
    r = scales ./ (freqs * pi);  % sinuslerin genlikleri | cemberlerin yaricaplari
    N = length(freqs);        % sinus sayisi | cember sayisi
    offset = ceil(sum(r));   % cemberler icin sola tasima miktari
       
    % Animasyon parametreleri
    animationSteps = 300;  % kac kez rendering yapilacagi (frame) sayisi
    rotationAngles = zeros(1,N);  % her cemberin acilarinin vektoru
    angleSteps = (2*pi) .* freqs / animationSteps; % her frame +aci


    % Cember noktalarinin (co-centric sekilde) hesaplanmasi, bu kismi 
    % internette buldum. Oncelikle NPTS=50 alarak t,x,y hesapliyoruz.
    % Ardindan x ve y'ye merkezlerini ekleyerek her cemberde yaricapin
    % cizilmesini saglamis oluyoruz. En sonda da homojen formda cember
    % noktalarini hesapliyoruz ve bu noktalarin diziminin transpozunu
    % aliyoruz.
    t = linspace(0, 2*pi, 50).';
    x = r .* cos(t);
    y = r .* sin(t);
    x = x([1 1:end],:);  x(1,:) = 0;
    y = y([1 1:end],:);  y(1,:) = 0;
    circlePoints = cat(2, reshape(x,[],1,N), reshape(y,[],1,N));
    circlePoints(:,3,:) = 0;  % z=0
    circlePoints(:,4,:) = 1;  % w=1
    circlePoints = permute(circlePoints, [2 1 3]);  % transpoz
    

    % 900x500 bir grafik acip monitorde ortala
    movegui(figure('Position', [0, 0, 900, 500]), 'center')
    % Cemberleri olusturup leftTransform ile ekranda sola otele ve
    % ardindan siyah renkle ciz, ek olarak grafikleri de ayarla (grid vb)
    trrr = makehgtform('translate', [-offset 0 0]);
    leftTransform = hgtransform('Matrix', trrr); % sola otelemek icin
    circles = line(x, y, 'Color', 'black'); % cemberler
    set(circles, 'Parent', leftTransform) % cemberleri oteleme
    % Sag ve solu ayiran dikey dogru (y=0)
    line([0 0], [-offset offset], 'Color', 'red') 
    % Grid'i acma, grafigi cevreleme filan
    grid on; box on; axis equal; 
    axis([-2 * offset ceil(2*pi) -offset offset])
    % X ekseninin 0'dan baslamasini saglama
    xAxis = gca;
    xAxis.XTick = xAxis.XTick(xAxis.XTick >= 0);
    % Noktanin cemberlerle aldigi yolun egrisi
    drawnPath = animatedline('Color', 'magenta', 'Parent', leftTransform);
    % Sol ve sag grafikler arasina cizilecek kirmizi cizgi
    inbetweenLine = line(nan, nan, 'Color', 'red');


    % Sum of harmonics ile Fourier serisi
    fourierX = linspace(0, 2*pi, animationSteps); % [0, 2pi]yi step kadar bol
    fourierY = nan(1, animationSteps); % ilk basta hesapli y degeri yok
    % fourierSeries degiskeni: sagdaki grafikte cizilen egri
    fourierSeries = line(fourierX, fourierY, 'Color', 'black');


    % Animasyonun simule edilmesinin loop'u, burada internetten yardim aldim
    for i=1:animationSteps+1    
        % Translasyon (oteleme) ve rotasyon (dondurme) matrislerinin hesabi
        tt = [rotationAngles(1) diff(rotationAngles)];
        translationMatrix = cell(1, N);
        rotationMatrixZ = cell(1, N);

        for k=1:N  % her cember icin
            translationMatrix{k} = makehgtform('translate', [r(k) 0 0]);
            rotationMatrixZ{k} = makehgtform('zrotate', tt(k));
        end
    
        for k=1:N  % her cember icin
            T = rotationMatrixZ{1};
            for j=1:k-1
                T = T * translationMatrix{j} * rotationMatrixZ{j+1};
            end
    
            % Transformlarin k'inci cembere uygulanmasi
            pt = T * circlePoints(:, :, k);
            pt = pt(1:2, :);  % homojen formdan x ve y degerlerini al
            circles(k).XData = pt(1, :); % cemberi guncelle
            circles(k).YData = pt(2, :); % cemberi guncelle
        end
    
        % Noktamizin aldigi yolu (drawnPath'i) guncelleme
        pt = pt(:,2);  % son cemberin merkez disinda ilk noktasi
        addpoints(drawnPath, pt(1), pt(2))
    
        % Sag grafikteki fourier serisini guncelle
        fourierY = [pt(2) fourierY(1:end-1)];
        fourierSeries.YData = fourierY;
    
        % Grafikler arasi dogruyu guncelle
        inbetweenLine.XData = [pt(1)-offset; 0];
        inbetweenLine.YData = [pt(2); pt(2)];
    
        pause(0.02)   % plot'un update edilmesini saglar
        rotationAngles = rotationAngles + angleSteps; % bir sonraki aciya gec
    end

    pause(0.5);
end









