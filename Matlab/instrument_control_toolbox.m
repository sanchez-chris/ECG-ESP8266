clear all;


RemoteIPFernando = "192.168.2.178";
RemoteIPKarlsruhefreifunknet = "10.214.50.250";
RemoteIPRedmi = "192.168.51.212";

u = udpport;
RemoteIPLab = "192.168.188.197";
write(u, "Hi ESP, hier ist Matlab", "char", RemoteIPLab, 4210);
counter = 0;

pause(5);
counter = 1;
ekg = [];
data = zeros(1, 250);

% Buffer Raw and filtered
while (counter <= 60)

    if(u.NumBytesAvailable)

        data = read(u, 250, "uint16");
        ekg = [ekg, data];     
        counter = counter + 1
    end
end

% Divide the ekg array into two arrays of 750 elements each
ekg_raw = reshape(ekg(1:7500),7500,1);
ekg_filtered = reshape(ekg(7501:end),7500,1);

% Plot the "ekg_raw" and "ekg_filtered" arrays
figure

% Subplot for "ekg_raw"
subplot(2,1,1)
plot(ekg_raw)
xlabel('Samples')
ylabel('Amplitude')
title('Unfiltered EKG')

% Subplot for "ekg_filtered"
subplot(2,1,2)
plot(ekg_filtered)
xlabel('Samples')
ylabel('Amplitude')
title('Filtered EKG')



