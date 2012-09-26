#include <iostream>

int main() {
    int n, m;
    std::cout << "Inserire due interi:" << std::endl;
    std::cin >> n >> m;

    if(n > m) { // Swap variables if necessary
        int temp = n;
        n = m;
        m = temp;
    }

    double sum = 0.;

    for(int i = n; i <= m; ++i) {
        sum += i * i;
    }

    std::cout << "La somma dei quadrati degli interi da " 
              << n << " a " << m 
              << " e' " << sum << std::endl;

    return 0;
}
