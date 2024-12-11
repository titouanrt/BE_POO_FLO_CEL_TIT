#include <exception>
#include <string>

class WiFiExceptions: public std::exception {
    private:
    std::string message_err;
public:
    WiFiExceptions(){
        message_err = "WiFi Error";
    }

    const char* what() const noexcept override {
        return message_err.c_str();
    }
};
