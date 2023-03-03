#include<iostream>
#include<string>
#include<bitset>

std::string toBinary(int n)
{
    std::bitset<3> bits;
    int i = 0
    while(n!=0) {

        (n%2==0 ?0:1); n/=2;
    }
    return r;
}
int main()
{
    std::string i= toBinary(10);
    std::cout<<i;
}