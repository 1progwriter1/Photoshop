#ifndef MY_EXCEPTIONS
#define MY_EXCEPTIONS


#define MY_EXCEPTION( arg, reason)  my_std::exception( arg, reason, __FILE__, __LINE__, (const char *)__func__)


#include <exception>
#include <string>


namespace my_std
{


enum RuntimeErrors
{
    Error = -1,
    NoError = 0,
    NoMemoryError = 2,
    FileOpenError = 3,
    FileCloseError = 4,
};


class exception : public std::exception
{
    exception *reason_;

    int error_int_;
    std::string error_str_;

    const char *file_;
    int line_;
    const char *func_;
public:
    explicit exception( const std::string &error, exception *init_reason = nullptr, const char *file = nullptr, const int line = -1,
                            const char *func = nullptr) noexcept( true);
    explicit exception( const int error, exception *init_reason = nullptr, const char *file = nullptr , const int line = -1,
                            const char *func = nullptr) noexcept( true);
    ~exception();

    void dumpInfo();
private:
    void dumpErrCodeExplanation();
    void dumpErrStr();
    void dumpErrSource();
};


}; // MyException





#endif // MY_EXCEPTIONS
