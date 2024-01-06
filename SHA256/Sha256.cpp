#include "Sha256.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <vector>

std::string Sha256::hash(const std::string &input) {
    std::vector<uint32_t> hashVariables = {
            0x6a09e667, 0xbb67ae85, 0x3c6ef372, 0xa54ff53a,
            0x510e527f, 0x9b05688c, 0x1f83d9ab, 0x5be0cd19,
    };

    std::string paddedInput = preprocess(input);

    for (size_t i = 0; i < paddedInput.size(); i += 64) {
        processBlock(paddedInput.substr(i, 64), hashVariables);
    }

    std::stringstream stringStream;
    stringStream << std::hex << std::setfill('0');
    for (const auto &value: hashVariables) {
        stringStream << std::setw(8) << value;
    }

    return stringStream.str();
}

std::string Sha256::preprocess(const std::string &input) {
    std::string processedInput = input;
    processedInput += static_cast<char>(0x80);

    size_t originalLength = input.size() * 8;
    size_t zerosToAdd = (originalLength + 64) % 512 == 0 ? 0 : 512 - ((originalLength + 64) % 512);
    processedInput += std::string(zerosToAdd / 8, 0);

    for (int i = 7; i >= 0; --i) {
        processedInput += static_cast<char>((originalLength >> (8 * i)) & 0xFF);
    }

    return processedInput;
}

void Sha256::processBlock(const std::string &block, std::vector<uint32_t> &hashVariables) {
    const std::vector<uint32_t> mainConstants = {
            0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
            0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
            0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
            0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
            0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
            0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0xf49b69c1,
            0x5dd1a475, 0x76f988da, 0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147,
            0x06ca6351, 0x14292967, 0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb,
    };

    std::vector<uint32_t> messageSchedule(64, 0);

    for (int i = 0; i < 16; ++i) {
        messageSchedule[i] = (static_cast<uint32_t>(block[i * 4]) << 24) |
               ((block[i * 4 + 1]) << 16) |
               (static_cast<uint32_t>(block[i * 4 + 2]) << 8) |
               static_cast<uint32_t>(block[i * 4 + 3]);
    }

    for (int i = 16; i < 64; ++i) {
        messageSchedule[i] = sigma1(messageSchedule[i - 2]) + messageSchedule[i - 7] + sigma0(messageSchedule[i - 15]) + messageSchedule[i - 16];
    }

    uint32_t 
        a = hashVariables[0],
        b = hashVariables[1],
        c = hashVariables[2],
        d = hashVariables[3],
        e = hashVariables[4],
        f = hashVariables[5],
        g = hashVariables[6],
        h = hashVariables[7];

    for (int i = 0; i < 64; ++i) {
        uint32_t t1 = h + Sigma1(e) + Ch(e, f, g) + mainConstants[i] + messageSchedule[i];
        uint32_t t2 = Sigma0(a) + Maj(a, b, c);
        h = g;
        g = f;
        f = e;
        e = d + t1;
        d = c;
        c = b;
        b = a;
        a = t1 + t2;
    }

    hashVariables[0] += a;
    hashVariables[1] += b;
    hashVariables[2] += c;
    hashVariables[3] += d;
    hashVariables[4] += e;
    hashVariables[5] += f;
    hashVariables[6] += g;
    hashVariables[7] += h;
};

uint32_t Sha256::Ch(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (~x & z);
}

uint32_t Sha256::Maj(uint32_t x, uint32_t y, uint32_t z) {
    return (x & y) ^ (x & z) ^ (y & z);
}

uint32_t Sha256::Sigma0(uint32_t x) {
    return ROTR(x, 2) ^ ROTR(x, 13) ^ ROTR(x, 22);
}

uint32_t Sha256::Sigma1(uint32_t x) {
    return ROTR(x, 6) ^ ROTR(x, 11) ^ ROTR(x, 25);
}

uint32_t Sha256::sigma0(uint32_t x) {
    return ROTR(x, 7) ^ ROTR(x, 18) ^ (x >> 3);
}

uint32_t Sha256::sigma1(uint32_t x) {
    return ROTR(x, 17) ^ ROTR(x, 19) ^ (x >> 10);
}

uint32_t Sha256::ROTR(uint32_t x, uint8_t n) {
    return (x >> n) | (x << (32 - n));
}
