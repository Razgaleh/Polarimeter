
function Waveplate3

T=xlsread('intensity.xlsx',3);
I_out= T(2,:);
assignin('base','I_18',I_out)
save('I_18.mat')

theta_0=0:4:360;
theta=degtorad(theta_0);
  
% MY OWN DATA  
NI_out= I_out/max(I_out); % NI_out normalized 
%THEORY
I=@(d,theta) 0.5*(1+cos(2*theta).^2+cos(d)*sin(2*theta).^2);%using mathematica 

d_fit = nlfit(theta,NI_out,I,pi/2);

figure
plot(theta_0,NI_out) % MY DATA
hold on 
plot(theta_0,I(d_fit,theta)) % THEORY

legend('My Data','Thoery')
ylabel('Intensity')
xlabel('Rotating waveplate angle, \theta (°)')
title({'',['Normalized Intensity Plot for 3rd Waveplate with \delta=' ,num2str(d_fit/pi),'\pi'],'',[' Hence Waveplate#3 is ',num2str(d_fit/(2*pi)),'\lambda'],''})
 

end

