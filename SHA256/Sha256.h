#ifndef SHA256_SHA256_H
#define SHA256_SHA256_H

#include <string>
#include <cstdint>
#include <vector>

class Sha256
{
public:
    std::string hash(const std::string& input);

private:
    std::string preprocess(const std::string& input);
    void processBlock(const std::string& block, std::vector<uint32_t>& h);
    static uint32_t Ch(uint32_t x, uint32_t y, uint32_t z);
    static uint32_t Maj(uint32_t x, uint32_t y, uint32_t z);
    static uint32_t Sigma0(uint32_t x);
    static uint32_t Sigma1(uint32_t x);
    static uint32_t sigma0(uint32_t x);
    static uint32_t sigma1(uint32_t x);
    static uint32_t ROTR(uint32_t x, uint8_t n);
};


#endif //SHA256_SHA256_H
