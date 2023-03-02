//convert the instruction into binary
        // void decode(int regdst, int regsrc1, int regsrc2) {
        //     //set the rest of the instruction bits
        //     //convert the register number to binary
        //     int i = 9, j = 6, f = 0, k, r, l;
        //     while(regdst != 0) {
        //         k = regdst%2==0 ?0:1;
        //         r = regsrc1%2==0 ?0:1;
        //         l = regsrc2%2==0 ?0:1;
        //         INSTR::b_instr[i++] = k;
        //         INSTR::b_instr[j++] = r;
        //         INSTR::b_instr[f++] = l;
        //         regdst /= 2;
        //         regsrc2 /= 2;
        //         regsrc1 /= 2;
        //     }
            
        //     //bits 3, 4, and 5 are 0s
        //     INSTR::b_instr[3] = 0; 
        //     INSTR::b_instr[4] = 0; 
        //     INSTR::b_instr[5] = 0; 
        // }