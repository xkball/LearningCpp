#include "c32string.h"
#include "BitOut.h"
#include "Math.h"

static const char emptyMusk = 0b00000000;

using ptr = std::unique_ptr<char32_t[]>;

c32string::c32string(int capacity){
    size = capacity;
    used = 0;
    string = ptr(new char32_t[size]{0});
}

c32string::c32string(char32_t* _string,int _size) {
    size = _size;
    used = size;
    string = ptr(new char32_t[size]);
    memcpy(string.get(), _string, size * 4);
}

c32string::c32string(const std::string& _string){
    size = _string.size()+1;
    string = ptr(new char32_t[size] {0});
    
    using namespace xkball;
    int readLoop = 0;
    int loop = 0;
    int wch = 0;
    for (char ch : _string) {
        //判断utf-8
        if (readLoop == 0) {
            if (!unsignedRightMove(musk4byte ^ ch, 3)) readLoop = 3;
            else if (!unsignedRightMove(musk3byte ^ ch, 4)) readLoop = 2;
            else if (!unsignedRightMove(musk2byte ^ ch, 5)) readLoop = 1;
        }
        if (readLoop > 0) {
            unsigned int c = ((unsigned int)ch) & 0b00000000000000000000000011111111;
            readLoop--;
            wch <<= 8;
            wch |= c;
        }
        else {
            if (wch) {
                unsigned int c = ((unsigned int)ch) & 0b00000000000000000000000011111111;
                wch <<= 8;
                wch |= c;
                string[loop] = (char32_t)wch;
                wch = 0;
            }
            else {
                string[loop] = ((char32_t)ch);
            }
            loop++;
        }
        
    }
    used = loop;
   
}

c32string::c32string(const c32string& copy) {
    this->size = copy.size;
    this->used = copy.used;
    string = ptr(new char32_t[size] {0});
    memcpy(string.get(), copy.string.get(), used * 4);
}

c32string::~c32string(){
    //delete[] string;
}

std::vector<c32string>* c32string::split(char32_t mark){
    std::vector<c32string>* bufArray = new std::vector<c32string>();
    c32string* buf = new c32string();
    for (int i = 0; i < used; i++) {
        if (mark == string[i]) {
            if (buf->used > 0) {
                 bufArray->push_back(*buf);
                 delete buf;
                 buf = new c32string();
            }
        }
        else *buf += string[i];
    }
    bufArray->push_back(*buf);
    delete buf;
    return bufArray;
}

void c32string::changeStringSize(int _size){
    char32_t* buff =new char32_t[size];
    memcpy(buff, string.get(), used * 4);
    //delete[] string;
    string.reset(new char32_t[_size]);
    size = _size;
    memcpy(string.get(), buff, used * 4);
    delete[] buff;
}

c32string* c32string::append(char32_t* _string,int toAdd){
  
    int toUse = toAdd + used;
    if (toUse > size) {
        changeStringSize(toUse+10);
    }
    memcpy(string.get() + used, _string, toAdd * 4);
    used = toUse;
    return this;
}

int c32string::length(){
    return this->used;
}

char32_t c32string::operator[](const int index) const{
    return string.get()[index];
}

c32string c32string::operator=(const c32string& toCopy){
    return c32string(toCopy);
}

std::size_t c32string::hash_code() const
{
    int h = 0;
    for (int i = 0; i < this->used; i++) {
        h = 31 * h + (int)(*this)[i];
    }
    return h;
}

c32string& operator+=(c32string& src, char32_t ch) {
    src.append(&ch,1);
    return src;
}

c32string& operator+=(c32string& src, c32string string){
    src.append(string.string.get(), string.used);
    return src;
}

std::ostream& operator<<(std::ostream& _Ostr,const c32string& string){
    for (int i = 0; i < string.used; i++) {
        char32_t ch = string[i];
        //xkball::bout << (int)ch;
        for (int j = 3; j >= 0; j--) {
            char c = (char)(ch >> 8 * j);
            if (c) {
                _Ostr << c;
                //xkball::bout << c;
            }
        }
        
    }
    
    return _Ostr;
}

c32string* substr(int start, int end, c32string& raw, c32string* rev) {
    for (int i = start; i < end; i++){
        *rev += raw[i];
    }
    return rev;
}

bool operator==(const c32string& left,const c32string& right){
    //数组大小不是很重要
    //if (left.size != right.size) return false;
    if (left.used != right.used) return false;
    for (int i = 0; i < left.used; i++) {
        if (left[i] != right[i]) return false;
    }
    return true;
}

//c32string& c32string::operator=(c32string&& other) noexcept{
//    if (this != &other) {
//        this->size = other.size;
//      this->used = other.used;
//        this->string = other.string;
//        other.string = nullptr;
//        other.used = 0;
//        other.size = 0;
//    }
//    return *this;
// 
//}




