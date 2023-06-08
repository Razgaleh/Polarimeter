
function stokes_vector_fit

% choose initial stokes vector
S_in = [1 0.5 0.5 1/sqrt(2)]';

polarizer_matrix= 0.5*[1 1 0 0; 1 1 0 0; 0 0 0 0; 0 0 0 0];
theta = linspace(0,2*pi,100);
delta=pi/2;
A=S_in(1)+(S_in(2)/2);
B=S_in(4);
C=S_in(2)/2;
D=S_in(3)/2;

S_out = zeros(4,numel(theta));
I= zeros(1,numel(theta));

for i = 1:numel(theta)
    S_out(:,i) = intensity(theta(:,i),S_in);
    
end

I_out = S_out(1,:);

fitfun =@(b,x) 1/2*(b(1) + b(2)*sin(2*(x-b(5))) + b(3)*cos(4*(x-b(5))) + b(4)*sin(4*(x-b(5))));
abcd_fit = nlfit(theta,I_out,fitfun,[0 0 0 0 0]);
I_fit = fitfun(abcd_fit,theta);

figure(1); clf;
g1=plot(theta,I_out);
hold on;
g2= plot(theta,I_fit,'--');
xlabel ('theta in rad')
ylabel('Intensity')
legend('Using S_{out}','Using Intensity fcn')
set(g1,'LineWidth',1)
set(g2,'LineWidth',2)
hold off;
% disp('S out is :' )
 disp(I_out) 
disp('ABCD coeff are')
disp(abcd_fit)

S00= abcd_fit(1)-abcd_fit(3);
S11=2*abcd_fit(3);
S22=2*abcd_fit(4);
S33=abcd_fit(2);
N_S= [S00,S11,S22,S33];
disp('This is S in ')
disp(N_S)

%function K = qwp_matrix(theta)

%                K=[1 0 0 0;
%                0 cos(2*theta)^2  cos(2*theta)*sin(2*theta) sin(2*theta);
%                0  cos(2*theta)*sin(2*theta) sin(2*theta)^2  -cos(2*theta);
%                0  -sin(2*theta) cos(2*theta)  0];
%
%     end

    function K = qwp_matrix(theta)
        K= [1 0 0 0;
            0 cos(2*theta)^2+cos(delta)*sin(2*theta)^2  cos(2*theta)*sin(2*theta)-cos(2*theta)*cos(delta)*sin(2*theta) sin(2*theta)*sin(delta);
            0  cos(2*theta)*sin(2*theta)-cos(2*theta)*cos(delta)*sin(2*theta) cos(delta)*cos(2*theta)^2+sin(2*theta)^2  -cos(2*theta)*sin(delta);
            0  -sin(2*theta)*sin(delta) cos(2*theta)*sin(delta) cos(delta)];
        
    end

    function S_out = intensity(theta,S_in)
        
        S_out = polarizer_matrix*qwp_matrix(theta)*S_in;
        
    end

    function fcn = intense(theta)
        fcn= 0.5*(A+B*sin(2*theta)+C*cos(4*theta)+D*sin(4*theta));
    end
end
