function [S0,S1,S2,S3]= stokes_fit(I_out) %input data of intensity

theta_0=0:4:360; % different theta values of rotating waveplatet 

%angleInRadians = degtorad(angleInDegrees)
theta=degtorad(theta_0);

    
fitfun=@(a,x) 1/2*(a(1)+a(2)*sin(2*x)+a(3)*cos(4*x)+a(4)*sin(4*x)); %the function used for nlfit program    

% abcd_fit = nlfit(theta,I_out,fitfun,[0 0 0 0]); % finding coefficients A,B,C,D
% S0= abcd_fit(1)-abcd_fit(3);
% S1=2*abcd_fit(3);
% S2=2*abcd_fit(4);
% S3=abcd_fit(2);
% disp('abcd_fit_nlfit is')
% disp(abcd_fit)

N=numel(theta);
a = 2/N*sum(I_out);
b=4/N*sum(I_out.*sin(2*theta));
c=4/N*sum(I_out.*cos(4*theta));
d=4/N*sum(I_out.*sin(4*theta));

abcd_fit_1=[a b c d];
% disp('abcd_fit_paper is')
% disp(abcd_fit_1)

S0=a-c;
S1=2*c;
S2=2*d;
S3=b;

S=[S0,S1,S2,S3]; % our stokes vector

% HS= [1,1,0,0];

maxVal = max(I_out);

norm_I =I_out/maxVal;



I_fit = fitfun(abcd_fit_1,theta);
norm_I_fit=I_fit/max(I_fit);
%   I0 = @(x) 1/2*(1+cos(2*x).^2);
%   I=I0/(theta);
figure
%  plot(theta_0,norm_I)
% hold on; 
%  plot(theta_0,norm_I_fit,'--')


% plot(theta_0,I,'--')
% ylim([0 1]);
% plot(theta_0,I_out)
% plot(theta_0,I_fit,'--')

 

ylabel('Intensity')
xlabel('Rotating waveplate angle, \theta (°)')
title('Normalized Intensity Plot')
legend('Normalized Plot','Expected Fit-Plot')

% determining polarization : 

% LHP =[1 1 0 0]; % left horizontal polarization 
% LVP =[1 -1 0 0]; % left vertical polarization
% Lp45P=[1 0 1 0]; % left +45 deg polarization
% Lm45P =[1 0 -1 0]; % left -45 deg polarization
% RCP =[1 0 0 1]; % right circular polariztion
% LCP=[1 0 0 -1]; % left  circular polariztion

% if S==LHP
%         disp('left horizontal polarization')
% elseif S==LVP
%         disp('left vertical polarization')
% elseif S==Lp45P
%         disp('left +45 deg polarization')
% elseif S==Lm45P
%         disp('left -45 deg polarization')
% elseif S==RCP
%         disp('right circular polzarization ')
% elseif S==LCP
%         disp('left circular polarization')
% else
%     disp('it is none of them')
      disp('S is')
      disp(S/S0)
%     disp('S0 is ')
%     disp(S(1)/norm(S))
%     
%     disp('S1 is ')
%     disp(S(2)/norm(S))
%     
%     disp('S2 is ')
%     disp(S(3)/norm(S))
%     
%     disp('S3 is ')
%     disp(S(4)/norm(S))
% end


%drawing poincare sphere: 

% just some equations that we'll use to derive spherical coordinates
%I=S0;
%p=sqrt(S1^2+S2^2+S3^2)/S0;
%psi_2 = atan(S2/S1); % 2*psi
%chi_2= atan (S3/sqrt(S1^2+S2^2)); % 2*chi

%converting to spherical coordinates

%phi= psi_2; %azimuth angle
%theta_1= chi_2; % elevation angle 
%r= p*I; % radius

%[x,y,z]=sph2cart(phi,theta_1,r);
%[x,y,z]=sphere;
%figure
%point=[S1,S2,S3]/(S1^2+S2^2+S3^2);
%m = mesh(x,y,z);
%set(m,'facecolor','none')
%hold on 
%plot3(S1,S2,S3,'marker','or') or use the following command:
%scatter3(point(1),point(2),point(3),'o','filled')
%labels={'Stokes Parameter'};
%text(S1,S2,S3,labels)
%quiver3(0,0,0,S1/(S1^2+S2^2+S3^2),S2/(S1^2+S2^2+S3^2),S3/(S1^2+S2^2+S3^2))
%title('Poincare Sphere')
%xlabel('S1','fontweight','bold')
%ylabel('S2','fontweight','bold')
%zlabel('S3','fontweight','bold')
end