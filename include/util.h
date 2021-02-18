//
// Created by zhaow on 2/18/2021.
//

#ifndef HELLOTRIANGLE_UTIL_H
#define HELLOTRIANGLE_UTIL_H

#define ParameterCheckFailed(isolate) { \
        std::string argsStr = "";\
        for (int c = 0; c < args.Length(); ++c) {\
            String::Utf8Value arg_utf8(isolate, args[c]);\
            std::string arg(*arg_utf8);\
            argsStr += arg;\
            if (c != args.Length() - 1) {\
                argsStr += ", ";\
            }\
        }\
        char buff[256]; sprintf( buff, "function: %s; args: %s; parameter check failed;",  __FUNCTION__, argsStr.c_str()); \
        fprintf(stderr, "function: %s; args: %s; parameter check failed;\n",  __FUNCTION__, argsStr.c_str());\
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, buff).ToLocalChecked()));\
}

#define THROW_ON_GL_ERROR(isolate) { \
    int errno;\
    if (sonic_gl_error_check && (errno = glGetError()) != GL_NO_ERROR) {\
        std::string argsStr = "";\
        for (int c = 0; c < args.Length(); ++c) {\
            String::Utf8Value arg_utf8(isolate, args[c]);\
            std::string arg(*arg_utf8);\
            argsStr += arg;\
            if (c != args.Length() - 1) {\
                argsStr += ", ";\
            }\
        }\
        char buff[256]; sprintf( buff, "function: %s; args: %s; gl error: %d;",  __FUNCTION__, argsStr.c_str(), errno ); \
        fprintf(stderr, "function: %s; args: %s; gl error: %d;\n",  __FUNCTION__, argsStr.c_str(), errno ); \
        isolate->ThrowException(Exception::Error(String::NewFromUtf8(isolate, buff).ToLocalChecked()));\
    }\
}

#endif //HELLOTRIANGLE_UTIL_H
