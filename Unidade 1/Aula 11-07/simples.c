#include <stdio.h>

int fibonacci (int n){
    if (n <=1){
        return n;
    }
    else{
        return fibonacci(n - 1) + fibonacci(n - 2);
    }
}

int main(){
    fibonacci(15);
    return 0;
}