
class JSInterface {
public:
    JSInterface() = delete;

    JSInterface &operator=(const JSInterface &) = delete;

    ~JSInterface() = delete;

    void *operator new(size_t size) = delete;

    static void ThrowV8Exception(v8::Isolate *isolate, const char *message);

    static void ParameterCheckFailed(v8::Isolate *isolate);
};
