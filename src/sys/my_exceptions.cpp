#include <sys/my_exceptions.hpp>
#include <systemdata.h>


namespace my_std
{


exception::exception( const std::string &error, exception *init_reason /*= nullptr*/, const char *file /*= nullptr*/ ,
                            const int line /*= -1*/, const char *func /*= nullptr*/) noexcept(true)
    :   reason_( init_reason), error_str_( error), file_( file), line_( line), func_( func)
{}


exception::exception( const int error, exception *init_reason /*= nullptr*/, const char *file /*= nullptr*/ ,
                            const int line /*= -1*/, const char *func /*= nullptr*/) noexcept(true)
    :   reason_( init_reason), error_int_( error), file_( file), line_( line), func_( func)
{}


exception::~exception()
{
    if ( reason_ != nullptr )
    {
        delete reason_;
    }
}



void exception::dumpInfo()
{
    if ( !error_str_.empty() && error_int_ == NoError )
    {
        error_int_ = Error;
    }
    dumpErrSource();
    dumpErrCodeExplanation();
    dumpErrStr();

    if ( reason_ != nullptr )
    {
        reason_->dumpInfo();
    }
}


void exception::dumpErrSource()
{
    printf( "in file: " BRIGHT_CYAN "%s" END_OF_COLOR
            ", in function: " BRIGHT_CYAN "%s" END_OF_COLOR
            ", on line: " BRIGHT_CYAN "%d" END_OF_COLOR ":\n",
            file_, func_, line_);
}


void exception::dumpErrStr()
{
    printf( RED "\terror message: " END_OF_COLOR);
    if ( !error_str_.empty() )
    {
        printf( "%s\n", error_str_.c_str());
    } else
    {
        printf( "(empty)\n");
    }
}


void exception::dumpErrCodeExplanation()
{
    printf( RED "\terror code: " END_OF_COLOR "%d - ", error_int_);
    switch ( error_int_ )
    {
        case Error:
            printf( "error\n");
            break;
        case NoError:
            printf( "success\n");
            break;
        case NoMemoryError:
            printf( "failed to allocate memory\n");
            break;
        case FileOpenError:
            printf( "failed to open file\n");
            break;
        case FileCloseError:
            printf( "failed to close file\n");
            break;
        default:
            printf( "unkown\n");
    }
}


}; // my_std
