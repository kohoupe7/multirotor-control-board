data = iddata(accel_ident, ((cont_ident/100) + (volt_ident*2.3) -92.4), period);
data.InputName  = 'Some number';
data.InputUnit  = '-';
data.OutputName = 'Acceleration';
data.OutputUnit = 'm/s^2';
data.TimeUnit   = 's';
opt = ssestOptions('SearchMethod','auto');
opt.SearchOption.MaxIter = 100;
opt.Display = 'on';
sys = ssest(data, 2, 'Ts', period, 'Form', 'canonical', opt);
resid(sys, data);
compare(data,sys);