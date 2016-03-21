A = [1 0.05 0 0 0; 0 1 0.05 0 0; 0 0 1 0.05 0; 0 0 0 0 1; 0 0 0 -0.66953 1.58995];
B = [0; 0; 0; 0.11700; 0.12710];
C = [0 1 0 0 0];
D = 0;
Q = [1 0 0 0 0; 0 10 0 0 0; 0 0 80 0 0; 0 0 0 0.00000001 0; 0 0 0 0 0.00000001];  
R = 0.000001;
[K, S, e] = dlqr(A, B, Q, R)
%sys = ss(A-B*K, B, C, D, 0.05);
%step(sys)
K1 = [0 1; 0 K(1)];
K2 = [0 1; 0 K(2)];
K3 = [0 1; 0 K(3)];
K4 = [0 1; 0 K(4)];
K5 = [0 1; 0 K(5)];
K6 = [0 1; 0 0];