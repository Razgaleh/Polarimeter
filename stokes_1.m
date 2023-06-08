% variable definitions
s0=1;
s1=1;
s2=0;
s3=0;
d=pi/2;
%th=[pi/2 0 pi/4 pi/3] ; 
t=pi/2;

% stokes matrices
S= [s0 s1 s2 s3]; 
LHP =[1 1 0 0]; % left horizontal polarization 
LVP =[1 -1 0 0]; % left vertical polarization
Lp45P=[1 0 1 0]; % left +45 deg polarization
Lm45P =[1 0 -1 0];% left -45 deg polarization
RCP =[1 0 0 1]; % right circular polariztion
LCP=[1 0 0 -1]; % left  circular polariztion


% mueller matrices 

waveplate14= [1 0 0 0 ; 0 1 0 0 ; 0 0 0 -1 ; 0 0 1 0]; % quarter waveplate fast vertical axis

W=cell(1,63);
for th=0:0.1:2*pi
    for i=1:63
               W{1,i}= [1 0 0 0;
               0 cos(2*th)^2+cos(d)*sin(2*th)^2  cos(2*th)*sin(2*th)-cos(2*th)*cos(d)*sin(2*th) sin(2*th)*sin(d); 
               0  cos(2*th)*sin(2*th)-cos(2*th)*cos(d)*sin(2*th) cos(d)*cos(2*th)^2+sin(2*th)^2  -cos(2*th)*sin(d);
               0  -sin(2*th)*sin(d) cos(2*th)*sin(d) cos(d)];
           %general waveplate d=phase difference between the fast and slow axis & th=the angle of the fast axis(theta) 
    end
    
end
polarizerh= 0.5*[1 1 0 0; 1 1 0 0; 0 0 0 0; 0 0 0 0]; % linear polarizer horizontal transmission

polarizergen = 0.5*[1 cos(2*t) sin(2*t) 0;
                cos(2*t) cos(2*t)^2 sin(2*t)*cos(2*t) 0;
                sin(2*t) sin(2*t)*cos(2*t) sin(2*t)^2 0;
                0  0  0 0]; % general linear polarizer transmission t=angle of the polarizer
            
  result = cell(1,63);
   for n=1:63        
result{1,n}=S*polarizerh*W{1,n};

%figuring out how is it polarized
if result{1,n}==LHP
        disp('left horizontal polarization')
elseif result{1,n}==LVP
        disp('left vertical polarization')
elseif result{1,n}==Lp45P
        disp('left +45 deg polarization')
elseif result{1,n}==Lm45P
        disp('left -45 deg polarization')
elseif result{1,n}==RCP
        disp('right circular polzarization ')
elseif result{1,n}==LCP
        disp('left circular polarization')
else
    disp('it is none of them')
end

    end