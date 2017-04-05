system ('./fd_demo > res');
a = load ('res');
loglog (a(:,1), abs(a(:,2)-a(:,3)), a(:, 1), a(:, 1).^2)
xlabel ('delta');
ylabel ('error')
