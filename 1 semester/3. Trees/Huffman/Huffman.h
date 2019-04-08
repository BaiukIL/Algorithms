//
// Created by ilya on 22.11.18.
//

#ifndef UNTITLED_HUFFMAN_H
#define UNTITLED_HUFFMAN_H

#endif //UNTITLED_HUFFMAN_H

#pragma once

#include <iostream>
#include <string>
#include <fstream>

typedef unsigned char byte;

struct IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte& value) = 0;
};

struct Reader : IInputStream {
    Reader(std::string name);
//    void Сlose() { file.close(); }
    bool Read(byte& value) override;
private:
    std::ifstream file;
};

Reader::Reader(std::string name) {
    file.open(name, std::ios::binary | std::ios::in);
    if (!file.is_open()) std::cout<<"Error";
}

bool Reader::Read(byte& value) {
    file.read((char*)& value, 1);
    return !file.eof();
}

struct IOutputStream {
    virtual void Write(byte value) = 0;
};

struct Writer : IOutputStream {
    Writer(std::string name);
//    void Сlose() { file.close(); }
    void Write(byte value) override;
private:
    std::ofstream file;
};

Writer::Writer(std::string name) {
    file.open(name, std::ios::binary | std::ios::out);
    if (!file.is_open()) std::cout<<"Error";
}

void Writer::Write(byte value) {
    file.write((char*)& value, 1);
}
