#include <openssl/md5.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

uint8_t char_to_num(unsigned char c) {
    // for example, 'f' as input should return 15
    // '0' as input should return 0
    // 'a' as input should return 10;
    if(c >= 'a' && c <= 'f') { return c - 96 + 9; }
    if(c >= '0' && c <= '9') { return c - 48; }
}

int main(int argc, char** argv) {
    // Big assumption – only finds 4-character passwords
    unsigned char potential_passwd[4];
    unsigned char tocheck[16];
    unsigned char* user_input = argv[1];
    for(int i = 0; i < 16; i += 1) {
        uint8_t char1v = char_to_num(user_input[i * 2]);
        uint8_t char2v = char_to_num(user_input[(i * 2) + 1]);
        tocheck[i] = char1v * 16 + char2v;
    }
    unsigned char hash[16];
    unsigned long attempts = 0;
    // Make this work for all keyboard characters (a-zA-Z0-9!@#$%^&*();',./:"<>?")
    // Verify that you can find 4-length password with other characters
    for(char c1 = 'a'; c1 <= 'z'; c1 += 1) {
        potential_passwd[0] = c1;
        for(char c2 = 'a'; c2 <= 'z'; c2 += 1) {
            potential_passwd[1] = c2;
            for(char c3 = 'a'; c3 <= 'z'; c3 += 1) {
                potential_passwd[2] = c3;
                for(char c4 = 'a'; c4 <= 'z'; c4 += 1) {
                    potential_passwd[3] = c4;
                    // we have a potential password to check
                    MD5(potential_passwd, 4, hash); // result stored in hash
                    attempts += 1;
                    if(strncmp(hash, tocheck, 16) == 0) {
                        printf("Found it! MD5(%.4s) = %s\n", potential_passwd, user_input);
                        printf("It took %ld attempts\n", attempts);
                        return 0;
                    }
                }
            }
        }
    }
    printf("Did not find a matching hash");

}

// Bigger challenge: make this work for passwords of length 4 to 20
// “Easy” version is 20 nested loops. But can you do it without 20 nested loops?