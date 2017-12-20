%DataAnalysis

%%

%Analysis of rotor speed
figure
plot(time,rotorspeed_set,'k');
g = findobj(gcf,'type','line');
set(g,'linewidth',2);
grid on;

hold on;

plot(time,rotorspeed_act,'b');
g = findobj(gcf,'type','line');
set(g,'linewidth',2);
grid on;

%To calculate the needed Parameter

h_t = @(taux,t) (1-exp(-((t-taux(2))/taux(1)))); %PT1

h2_t =

tau0 = [0.01 0.02]; % Anfangswerte für die Suche
[tau,R1,~,~,MSE1] = nlinfit(time,rotorspeed_act,h_t,tau0); % Parameter,Residual,ErrormodelInfo

fplot (@(t) (1-exp(-((t-tau(2))/tau(1))))); %plot the regression with the parameters
g = findobj(gcf,'type','line');
set(g,'linewidth',2);
grid on;
legend('soll','ist','fit'); xlabel('Zeit [s]'); ylabel('Rotorgeschwindigkeit [rpm]');

%%

%Analysis of Thrust

figure
scatter(normRotorspeed,thrust);
g = findobj(gcf,'type','line');
set(g,'linewidth',2);
grid on;
xlabel('Normierte Rotorgeschwindigkeit [rpm]'); ylabel('Schub Kraft [N]');

%We assume that the aerodynamic lift force is proportional to the square of the rotorspeed

F_thrust= @(a,omega) ((a(3)*omega.^2)+(a(2)*omega) + a(1));

alpha0 = [0.02 0.5 1]; % Anfangswerte für die Suche
[alpha,R2,~,~,MSE2] = nlinfit(omega,thrust,F_thrust,alpha0); % Parameter,Residual,ErrormodelInfo

fplot (@(omega) ((alpha(3)*omega.^2)+(alpha(2)*omega) + alpha(1))); %plot the regression with the parameters
g = findobj(gcf,'type','line');
set(g,'linewidth',2);
grid on;
xlabel('Zeit [s]'); ylabel('Schub Kraft [N]');


%%

%Analysis of Current

figure
plot(time,current);
g = findobj(gcf,'type','line');
set(g,'linewidth',2);
grid on;
