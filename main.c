#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * AsciiRange
 *
 * This enum holds the uppercase and lowercase ascii bounds
 * so that they do not need to remain in the code as constants
 * and do not need to be declared as pre-processor replacements
 *
 * uppercase ascii letters go from A(65) to Z(90)
 * lowercase ascii letters go from a(97) to z(122)
 * the distance between A and a is 32
 */
typedef enum {
    uppercase_upper = 90,
    uppercase_lower = 65,
    lowercase_upper = 122,
    lowercase_lower = 97,
    range_distance = 32
} AsciiRange;

/**
 * Function signatures
 */
char calculateRotShift(char in, char offset);

char *correctKey(char *key);

char *getInput(char *input, char *prompt);

char *vigenere(char *plainText, char *key);

bool isUppercase(char in);

bool isLowercase(char in);

bool inBounds(char in);

int getFloor(char in);

int main() {
    char *key, *input;

    key = getInput(key, "Password: ");
    input = getInput(input, "Plain text: ");

    key = correctKey(key);

    printf("\n%s\n", vigenere(input, key));
    return 0;
}

/**
 * isUppercase
 *
 * determines whether or not the passed character is within the uppercase
 * ascii bounds.
 *
 * @param in - the character to check
 * @return bool
 */
bool isUppercase(char in) {
    return (in >= uppercase_lower && in <= uppercase_upper);
}

/**
 * isLowercase
 *
 * determines whether or not the passed character is within the lowercase
 * ascii bounds.
 *
 * @param in - the character to check
 * @return bool
 */
bool isLowercase(char in) {
    return (in >= lowercase_lower && in <= lowercase_upper);
}

/**
 * inBounds
 *
 * determines if the character passed to it is within either the
 * uppercase or lowercase ascii ranges as defined above
 *
 * @param in - the character to check
 * @return bool
 */
bool inBounds(char in) {
    return (isLowercase(in) || isUppercase(in)) ? true : false;
}

/**
 * getFloor
 *
 * takes a character and determines its case. It then returns the lowest
 * bound for that case.
 *
 * @param in - character to determine case floor for
 * @return int
 */
int getFloor(char in) {
    return isLowercase(in) ? lowercase_lower : uppercase_lower;
}

/**
 * calculateRotShift
 *
 * takes two characters and uses them to determine by how much the first
 * should be shifted according to the ROT cipher. For a quick run down on
 * what that is please see the following Wikipedia article:
 * <https://en.wikipedia.org/wiki/ROT13>
 *
 * @quirk this function will use the case of the input character and not the
 * offset character when returning the calculated result. If this is not
 * desirable behavior then you can change it by substituting inFloor for
 * offsetFloor.
 *
 * @param in - character to shift
 * @param offset - character to shift by
 * @return char
 */
char calculateRotShift(char in, char offset) {
    int inFloor = getFloor(in),
            offsetFloor = getFloor(offset);

    return (((in + offset) - (inFloor + offsetFloor)) % 26) + inFloor;
}

/**
 * correctKey
 *
 * takes a key and will normalize it so that it only contains letters.
 * @param key
 * @return
 */
char *correctKey(char *key) {
    if (!key) {
        goto error;
    }

    int length = strlen(key);

    for (int i = 0; i < length; i++) {
        // nullify invalid characters
        if (!inBounds((key[i]))) {
            strcpy(&key[i], &key[i + 1]);
            i--;
            length--;
        }

        // convert uppercase to lowercase
        if (isUppercase(key[i])) {
            key[i] += range_distance;
        }
    }

    key = realloc(key, strlen(key) + 1);
    key[strlen(key)] = '\0';

    return key;

    error:
    printf("[correctKey] key was NULL!\n");
    return NULL;
}

/**
 * vigenere
 *
 * performs the vigenere cipher on the input against the key
 *
 * @param plainText - text to cipher
 * @param key - key to cipher against
 */
char *vigenere(char *plainText, char *key) {
    if (!plainText || !key) {
        goto error;
    }

    int keyi = 0,
            keylen = strlen(key),
            length = strlen(plainText);

    for (int i = 0; i < length; i++) {
        if (keyi == keylen) {
            keyi = 0;
        }

        if (inBounds(plainText[i])) {
            plainText[i] = calculateRotShift(plainText[i], key[keyi]);
        }

        keyi++;
    }

    return plainText;

    error:
    if (!plainText) {
        printf("[vigenere] plainText was NULL!\n");
    }

    if (!key) {
        printf("[vigenere] key was NULL!\n");
    }

    return NULL;
}

/**
 * getInput
 *
 * takes a prompt and a pointer and then dynamically grows it as more
 * memory is needed for dynamic input.
 *
 * @param input - dest
 * @param prompt - prompt for the user
 * @return char*
 */
char *getInput(char *input, char *prompt) {
    if (!input || !prompt) {
        goto error;
    }

    int c,
            i = 0,
            length = 3;

    // initial allocation
    input = malloc(sizeof(char) * length);

    printf("%s", prompt);

    while (true) {
        if (i == length) {
            length += 3;
            input = realloc(input, length);
        }

        if ((c = getchar()) == '\n') {
            break;
        }

        input[i] = c;

        i++;
    }

    // trim up
    input = realloc(input, i + 1);
    input[i] = '\0';

    return input;

    error:
    if (!input) {
        printf("[getInput] input was NULL!\n");
    }

    if (!prompt) {
        printf("[getInput] prompt was NULL!\n");
    }

    return NULL;
}