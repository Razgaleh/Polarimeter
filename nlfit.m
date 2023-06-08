function [b,varargout] = nlfit(x,y,f,b0,fixed,lb,ub,err,alpha,opt)
%  NLFIT Nonlinear least-squares regression. (eEDM custom) [130429]
%     BETA = NLFIT(X,Y,MODELFUN,BETA0) estimates the coefficients of a
%     nonlinear regression function, using least squares estimation.
%     
%     BETA = NLFIT(X,Y,MODELFUN,BETA0,FIXED) estimates the coefficients of a
%     nonlinear regression function, holding BETA(i) fixed when FIXED(i) is 
%     nonzero.
%     
%     BETA = NLFIT(X,Y,MODELFUN,BETA0,FIXED,LB,UB) estimates the coefficients of a
%     nonlinear regression function, with lower bound LB and uppper bound UB.
%
%     BETA = NLFIT(X,Y,MODELFUN,BETA0,FIXED,LB,UB,ERR) estimates the coefficients of a
%     nonlinear regression function, with weights 1/ERR.
%  
%     [BETA,ERR] = NLFIT(X,Y,MODELFUN,BETA0) returns the fitted
%     coefficients BETA, and 1-sigma errors ERR.
%     
%     [BETA,ERR,CI] = NLFIT(X,Y,MODELFUN,BETA0) returns the fitted
%     coefficients BETA, 1-sigma errors ERR, and 0.95 confidence interval CI.
%     
%     [BETA,ERR,CI] = NLFIT(X,Y,MODELFUN,BETA0,FIXED,LB,UB,ERR,ALPHA) returns the 
%     fitted coefficients BETA, 1-sigma errors ERR, and ALPHA confidence 
%     interval CI.

%% Default arguments.
if nargin<10
    opt = [];
end
if nargin<9
    alpha = 0.95; % default to 0.95 confidence interval
end
if nargin<8
    err = [];
end
if nargin<7
    ub = [];
end
if nargin<6
    lb = [];
end
if nargin<5
    fixed = [];
end

if isempty(opt)
    % no options given, standard is to set Display to off and shutdown
    % warnings so that automatic fitting is facilitated
%     opt = optimset('Display','off');
opt = optimset('Display','off','TolFun',1e-9);
%     warning('off', 'all');
end
if isempty(err)
    err = 1+0*y;
end
if isempty(ub)
    ub = inf+0*b0; % default to +inf for upper bound
end
if isempty(lb)
    lb = -inf+0*b0; % default to -inf for lower bound
end
if isempty(fixed)
    fixed = 0*b0; % default to not fixing any parameters
end
if (size(fixed)~=size(b0)) | (size(lb)~=size(b0)) | (size(ub)~=size(b0))
    error('Parameters b0, fixed, lb and ub must have the same size!');
end
if any(lb(~fixed)==ub(~fixed))
    error('Lower Bound and Upper Bound cannot be the same!');
end
for i=1:length(lb)
    a = min(lb(i),ub(i));
    ub(i) = max(lb(i),ub(i));
    lb(i) = a;
end
if all(fixed)
    b = b0;
    out = cell(1,2);
    out{1,1} = 0*b0;
    out{1,2} = 0*b0;
    if nargout>1
        varargout = out(1,1:nargout-1);
    end
    return;
end
var = err.^2;
err = err/mean(err(:));
var(isnan(err))=inf;
err(isnan(err))=inf;
var(~err)=inf;
err(~err)=inf;

fixed(~(~(fixed)))=1; % make the fixed array ones and zeros;


%% Fitting
% generate a function, f_fixed, that is the same as f but hold certain
% parameters at constant values, and accepts a shorter range of not fixed
% parameters.
function y = f_fixed(beta,z)
    b = b0.*fixed;
    b(~fixed) = beta;
    y = f(b,z)./err;
end
% do fit using lsqcurvefit
[beta,~,residual,~,~,~,J] = lsqcurvefit(@f_fixed, b0(~fixed), x, y./err, lb(~fixed), ub(~fixed), opt);
% turn warnings back on
% warning('on', 'all');
% get the 1-sigma error bars from the residuals and jacobian.
fiterr = diff(nlparci(beta,residual,'jacobian',J,'alpha',1-erf(1/sqrt(2)))')/2;
% get the confidence interval, alpha.
ci = nlparci(beta,residual,'jacobian',J,'alpha',1-alpha);


%% Output conditioning
b = b0.*fixed;
b(~fixed) = beta;

err_out = 0*b0;
err_out(~fixed) = fiterr;

if size(b,1)==1
    ci_out = [b; b];
    ci_out(:,~fixed) = ci';
else
    ci_out = [b, b];
    ci_out(~fixed,:) = ci;
end

out = cell(1,2);
out{1,1} = err_out;
out{1,2} = ci_out;
out{1,3} = sum((f(b,x)-y).^2./var)/(length(y)-length(b(~fixed)));

if nargout>1
    varargout = out(1,1:nargout-1);
end

end