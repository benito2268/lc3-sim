// lc-3 simulator - file instr.h
// a file to define lc3 instructions
#pragma once
#include<bitset>
#define WORDSIZE 16 //a 'word' is 16 bits

namespace lc3 {
    typedef std::bitset<WORDSIZE> Register;
    typedef std::bitset<WORDSIZE> Instruction;

    struct INSTR {
        const char* name;
        Instruction b_instr{};  //the binary representaion of the instruction

        INSTR(const char* iname):name(iname) {}
        virtual void exec(Instruction i) = 0;
    };

    //define syntax and semantics for all of the lc3 instructions
    struct NOT : public INSTR { 
        //name as appears in syntax
        NOT():INSTR("NOT") {
            INSTR::b_instr[15] = 1; 
            INSTR::b_instr[14] = 0; 
            INSTR::b_instr[13] = 0; 
            INSTR::b_instr[12] = 1; 
        }
        virtual void exec(Instruction i) override;
    };

    struct ADD : public INSTR { 
        //name as appears in syntax
        ADD():INSTR("ADD") {
            INSTR::b_instr[15] = 0; 
            INSTR::b_instr[14] = 0; 
            INSTR::b_instr[13] = 0; 
            INSTR::b_instr[12] = 1; 
        }
        virtual void exec(Instruction i) override;
    };

    struct AND : public INSTR { 
        //name as appears in syntax
        AND():INSTR("AND") {
            INSTR::b_instr[15] = 0; 
            INSTR::b_instr[14] = 1; 
            INSTR::b_instr[13] = 0; 
            INSTR::b_instr[12] = 1; 
        }
        virtual void exec(Instruction i) override;
    };

    struct BR : public INSTR { 
        //name as appears in syntax
        BR():INSTR("BR") {
            INSTR::b_instr[15] = 0; 
            INSTR::b_instr[14] = 0; 
            INSTR::b_instr[13] = 0; 
            INSTR::b_instr[12] = 0; 
        }
        virtual void exec(Instruction i) override;
    };

    struct JMP : public INSTR { 
        //name as appears in syntax
        JMP():INSTR("JMP") {
            INSTR::b_instr[15] = 1; 
            INSTR::b_instr[14] = 1; 
            INSTR::b_instr[13] = 0; 
            INSTR::b_instr[12] = 0; 
        }
        virtual void exec(Instruction i) override;
    };

    struct JSR : public INSTR { 
        //name as appears in syntax
        JSR():INSTR("JSR") {
            INSTR::b_instr[15] = 0; 
            INSTR::b_instr[14] = 1; 
            INSTR::b_instr[13] = 0; 
            INSTR::b_instr[12] = 0; 
        }
        virtual void exec(Instruction i) override;
    };

    struct JSRR : public INSTR { 
        //name as appears in syntax
        JSRR():INSTR("JSRR") {
            INSTR::b_instr[15] = 0; 
            INSTR::b_instr[14] = 1; 
            INSTR::b_instr[13] = 0; 
            INSTR::b_instr[12] = 0; 
        }
        virtual void exec(Instruction i) override;
    };

    struct LD : public INSTR { 
        //name as appears in syntax
        LD():INSTR("LD") {
            INSTR::b_instr[15] = 0; 
            INSTR::b_instr[14] = 0; 
            INSTR::b_instr[13] = 1; 
            INSTR::b_instr[12] = 0; 
        }
        virtual void exec(Instruction i) override;
    };

    struct LDI : public INSTR { 
        //name as appears in syntax
        LDI():INSTR("LDI") {
            INSTR::b_instr[15] = 1; 
            INSTR::b_instr[14] = 0; 
            INSTR::b_instr[13] = 1; 
            INSTR::b_instr[12] = 0; 
        }
        virtual void exec(Instruction i) override;
    };

    struct LDR : public INSTR { 
        //name as appears in syntax
        LDR():INSTR("LDR") {
            INSTR::b_instr[15] = 0; 
            INSTR::b_instr[14] = 1; 
            INSTR::b_instr[13] = 1; 
            INSTR::b_instr[12] = 0; 
        }
        virtual void exec(Instruction i) override;
    };

    struct LEA : public INSTR { 
        //name as appears in syntax
        LEA():INSTR("LEA") {
            INSTR::b_instr[15] = 1; 
            INSTR::b_instr[14] = 1; 
            INSTR::b_instr[13] = 1; 
            INSTR::b_instr[12] = 0; 
        }
        virtual void exec(Instruction i) override;
    };

    struct RET : public INSTR { 
        //name as appears in syntax
        RET():INSTR("RET") {
            INSTR::b_instr[15] = 1; 
            INSTR::b_instr[14] = 1; 
            INSTR::b_instr[13] = 0; 
            INSTR::b_instr[12] = 0; 
        }
        virtual void exec(Instruction i) override;
    };

    struct RTI : public INSTR { 
        //name as appears in syntax
        RTI():INSTR("RTI") {
            INSTR::b_instr[15] = 1; 
            INSTR::b_instr[14] = 0; 
            INSTR::b_instr[13] = 0; 
            INSTR::b_instr[12] = 0; 
        }
        virtual void exec(Instruction i) override;
    };

    struct ST : public INSTR { 
        //name as appears in syntax
        ST():INSTR("ST") {
            INSTR::b_instr[15] = 0; 
            INSTR::b_instr[14] = 0; 
            INSTR::b_instr[13] = 1; 
            INSTR::b_instr[12] = 1; 
        }
        virtual void exec(Instruction i) override;
    };

    struct STI : public INSTR { 
        //name as appears in syntax
        STI():INSTR("STI") {
            INSTR::b_instr[15] = 1; 
            INSTR::b_instr[14] = 0; 
            INSTR::b_instr[13] = 1; 
            INSTR::b_instr[12] = 1; 
        }
        virtual void exec(Instruction i) override;
    };

    struct STR : public INSTR { 
        //name as appears in syntax
        STR():INSTR("STR") {
            INSTR::b_instr[15] = 0; 
            INSTR::b_instr[14] = 1; 
            INSTR::b_instr[13] = 1; 
            INSTR::b_instr[12] = 1; 
        }
        virtual void exec(Instruction i) override;
    };

    struct TRAP : public INSTR { 
        //name as appears in syntax
        TRAP():INSTR("TRAP") {
            INSTR::b_instr[15] = 1; 
            INSTR::b_instr[14] = 1; 
            INSTR::b_instr[13] = 1; 
            INSTR::b_instr[12] = 1; 
        }
        virtual void exec(Instruction i) override;
    };      
}