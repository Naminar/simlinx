int fibonacci_recursive(int n) {
    if (n <= 1) return 1;
    return fibonacci_recursive(n-1) + fibonacci_recursive(n-1);
}

int main() {
    return fibonacci_recursive(3);
}