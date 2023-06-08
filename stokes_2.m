I=0:1:62;
N=size(I,2);

LHP =[1 1 0 0]; % left horizontal polarization 
LVP =[1 -1 0 0]; % left vertical polarization
Lp45P=[1 0 1 0]; % left +45 deg polarization
Lm45P =[1 0 -1 0];% left -45 deg polarization
RCP =[1 0 0 1]; % right circular polariztion
LCP=[1 0 0 -1]; % left  circular polariztion


for th=0:0.1:2*pi;
    for n=1:1:63;
A= (2/N)*sum(I);
m1=I(n)*sin(2*th);
m2=I(n)*cos(4*th);
m3=I(n)*sin(4*th);
B=(4/N)*sum(m1);
C=(4/N)*sum(m2);
D=(4/N)*sum(m3);

s0=A-C;
s1=2*C;
s2=2*D;
s3=B;
S=[s0,s1,s2,s3];


if S==LHP
        disp('left horizontal polarization')
elseif S==LVP
        disp('left vertical polarization')
elseif S==Lp45P
        disp('left +45 deg polarization')
elseif S==Lm45P
        disp('left -45 deg polarization')
elseif S==RCP
        disp('right circular polzarization ')
elseif S==LCP
        disp('left circular polarization')
else
    disp('it is none of them')
    disp(S)
end
    end
end
