
%% Syntax
% cutMessdaten (Times)
% Times = cell-Array mit Startzeitpunkt und Endzeitpunkt des gewünschten
% Ausschnitts
% 
% Beispiel: 
% Times = {0, 20; 60, 80;}
%%
function cutMessdaten (Times)
    % Laden der gewünschten mat-Datei, die geschnitten werden soll/
    % Dateiname/ Structname gegebenenfalls anpassen!!!
    load('rotorspeed_set')
    datanames = fieldnames(Geshwindigkeitsverlauf);
     
    % Beginnen des Schneidens für die Länge von Times
    for ii = 1:1:size(Times, 1)
        for iii = 1:1:length(datanames)
            Structname = sprintf('rotorspeed_%d', (ii));  
            datanew.(Structname).(datanames{iii}).data = rotorspeed_set.(datanames{iii}).data(Times{ii, 1}: Times{ii, 2}); % Dateiname gegebenenfalls anpassen!!!
        end
        Filename = sprintf('rotorspeed_V1_%d.mat', (ii));  % anpassen
        save(Filename, '-struct', 'datanew');
    end
end