//lc3 simulator - file base.h
//a file providing functions for converting between hex - dex - bin
#pragma once
#include<cmath>
#include<bitset>

namespace lc3 {

    //TODO convert into one function
    //it's kinda dumb as is
    struct conv {
        //for converting register numbers mainly 
        static int bin3_to_dec(std::bitset<3> bin) {
           int dec = 0; 
           for(int i = 0; i < 3; i++) {
                if(bin[i] == 1) {
                    dec += pow(2, i);
                }
           }
           return dec;
        }

        static int bin6_to_dec(std::bitset<6> bin) {
           int dec = 0; 
           for(int i = 0; i < 6; i++) {
                if(bin[i] == 1) {
                    dec += pow(2, i);
                }
           }
           return dec;
        }

        static int bin8_to_dec(std::bitset<8> bin) {
           int dec = 0; 
           for(int i = 0; i < 8; i++) {
                if(bin[i] == 1) {
                    dec += pow(2, i);
                }
           }
           return dec;
        }

        //for converting register numbers mainly 
        static int bin16_to_dec(std::bitset<16> bin) {
           int dec = 0; 
           for(int i = 0; i < 16; i++) {
                if(bin[i] == 1) {
                    dec += pow(2, i);
                }
           }
           return dec;
        }
    };
}