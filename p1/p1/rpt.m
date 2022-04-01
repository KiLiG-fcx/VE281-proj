
plot(n,bubble)
hold on;
plot(n,insertion)
hold on;
plot(n,selection)
hold on;
plot(n,merge)
hold on
plot(n,qe)
hold on;
plot(n,qs)
hold on;
plot(n,sortstd)
hold on;
ylim([0 1.5])
legend('bubble sort','insertion sort','selection sort','merge sort','quick extra','quick inplace','std::sort')