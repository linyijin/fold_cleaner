#ifndef ERROR_H
#define ERROR_H
#include<iostream>
#include<exception>
#include<string>
using std::cout;
using std::endl;
class OutOfBound
{
public:
    OutOfBound(const char *msg):message_(message)
    {
        cout<<"out of bound"<<endl;
    }
    virtual ~OutOfBound()
    {}
    const char *what() const
    {
        return message_.c_str();
    }
private:
    string message_;
}
#endif // ERROR_H
