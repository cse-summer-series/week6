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

int check_password(
    unsigned char* potential_passwd,
    int length,
    unsigned char* hash,
    unsigned long* attempts,
    unsigned char* tocheck,
    unsigned char* user_input
) {
    // we have a potential password to check
    MD5(potential_passwd, length, hash); // result stored in hash
    potential_passwd[length] = '\0';
    *attempts += 1;
    if(strncmp(hash, tocheck, 16) == 0) {
        printf("Found it! MD5(%s) = %s\n", potential_passwd, user_input);
        printf("It took %ld attempts\n", *attempts);
        return 1;
    }
    else {
        return 0;
    }
}

int check_passwdaz_at(
    int index,
    unsigned char* potential_passwd,
    int length,
    unsigned char* hash,
    unsigned long* attempts,
    unsigned char* tocheck,
    unsigned char* user_input
) {
    int result;
    for(char c = 'a'; c <= 'z'; c += 1) {
        potential_passwd[index] = c;
        if(index == length - 1) {
            result = check_password(potential_passwd, length, hash, attempts, tocheck, user_input);
        }
        else {
            result = check_passwdaz_at(index + 1, potential_passwd, length, hash, attempts, tocheck, user_input);
        }
        if(result) { return result; }
    }
    return 0;

}

int main(int argc, char** argv) {
    // Big assumption – only finds 4-character passwords
    unsigned char potential_passwd[1000];
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
    int current_length = 1;
    int result = 0;
    while(current_length < 1000) {
        result = check_passwdaz_at(0, potential_passwd, current_length, hash, &attempts, tocheck, user_input);
        if(result) { break; }
        current_length += 1;
    }
    if(!result) { printf("Did not find a matching hash\n"); }

}

// Bigger challenge: make this work for passwords of length 4 to 20
// “Easy” version is 20 nested loops. But can you do it without 20 nested loops?