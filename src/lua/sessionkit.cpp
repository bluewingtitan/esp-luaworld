#include "lua_bind.h"

#include <mbedtls/base64.h>
#include <AES.h>
#include <SPIFFS.h>
#include <RNG.h>

#define SKIT_PREPEND "SKIT::"

/// @brief Loads or generates a chacha streamkey by name
/// @param keyname
/// @param chacha
void load_or_create_key(const std::string &keyname, AES256 &aes)
{
    if (!SPIFFS.exists("/sessionkit"))
    {
        SPIFFS.mkdir("/sessionkit");
    }

    uint8_t key[256];

    auto filename = "/sessionkit/" + keyname + ".skey";
    if (!SPIFFS.exists(filename.c_str()))
    {
        // generate key
        RNG.rand(key, 256);
        auto f = SPIFFS.open(filename.c_str(), FILE_WRITE);
        f.write(key, 256);
        f.close();
    }
    else
    {
        auto f = SPIFFS.open(filename.c_str(), FILE_READ);
        f.readBytes((char *)key, 256);
        f.close();
    }

    aes.setKey(&key[0], 256);
}

class sessionkit
{
public:
    sessionkit(std::string keyname) : aes_{}
    {
        load_or_create_key(keyname, aes_);
    }

    std::string get_token(std::string data)
    {
        data.insert(0, SKIT_PREPEND);

        size_t len = data.length();
        if (len > 256 + 6)
        {
            return std::string{};
        }

        size_t block_size = aes_.blockSize();
        size_t block_count = std::ceil(len / (block_size * 1.0));
        size_t bufferlen = block_count * block_size;
        uint8_t dout[bufferlen];
        uint8_t din[bufferlen];

        memset(din, 0, bufferlen);

        data.copy((char *)din, bufferlen);

        for (size_t i = 0; i < block_count; i++)
        {
            size_t buffer_offset = i * block_size;
            aes_.encryptBlock(dout + buffer_offset, din + buffer_offset);
        }

        return std::string{(char *)dout, bufferlen};
    }

    std::string get_data(std::string data)
    {
        size_t len = data.length();

        size_t block_size = aes_.blockSize();
        size_t block_count = std::ceil(len / (block_size * 1.0));
        size_t bufferlen = block_count * block_size;
        uint8_t dout[bufferlen];
        uint8_t din[bufferlen];

        memset(din, 0, bufferlen);
        memset(dout, 0, bufferlen);

        data.copy((char *)din, bufferlen);

        for (size_t i = 0; i < block_count; i++)
        {
            size_t buffer_offset = i * block_size;
            aes_.decryptBlock(dout + buffer_offset, din + buffer_offset);
        }

        // check if data is a valid token
        size_t prepend_len = strlen(SKIT_PREPEND);
        if (strncmp(SKIT_PREPEND, (const char *)dout, prepend_len) != 0)
        {
            return "";
        }

        return std::string{(char *)dout + prepend_len, bufferlen - prepend_len};
    }

private:
    AES256 aes_;
};

void lw::register_sessionkit(sol::state &lua)
{
    // todo remove test code!

    sessionkit skit{"demo"};
    std::string res{"This is a test."};
    Serial.println(res.c_str());
    auto out = skit.get_token(res);
    Serial.println(out.c_str());
    auto fin = skit.get_data(out);
    Serial.println(fin.c_str());
}
