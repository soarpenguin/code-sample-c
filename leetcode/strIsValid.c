#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define true 1
#define false 0

int
isValid(char *str) {
    char *stack = NULL;
    int i = 0;
    int top = -1;
    int result = true;

    if (NULL == str) {
        return true;
    }

    stack = calloc(strlen(str) / 2, sizeof(char));

    if (NULL == stack) {
        return false;
    }

    for (i = 0; i < strlen(str); i++) {
        if ('{' == str[i] || '[' == str[i] || '(' == str[i]) {
            stack[++top] = str[i];

        } else if ('}' == str[i] || ']' == str[i] || ')' == str[i]) {
            if (top >= 0) {
                if ('}' == str[i] && '{' == stack[top]) {
                    top--;
                } else if (']' == str[i] && '[' == stack[top]) {
                    top--;
                } else if (')' == str[i] && '(' == stack[top]) {
                    top--;
                } else {
                    result = false;
                    break;
                }
            } else {
                result = false;
                break;
            }
        }
    }

    if (top >= 0) {
        result = false;
    }

    free(stack);
    return result;
}

int
main(int argc, char *argv[]) {
    char str[100];
    int result = 0;

    printf("Please input a string:\n");
    scanf("%s", str);

    result = isValid(str);

    printf("Input string is:\n%s\n", str);
    printf("The result of judge is: %s\n", (result ? "true" : false));

    return 0;
}

//class Solution {
//    public:
//        bool isValid(string s) {
//            // Start typing your C/C++ solution below
//            // DO NOT write int main() function
//
//            char *stack = NULL;
//            int i = 0;
//            int top = -1;
//            int result = true;
//
//            if (s.empty()) {
//                return true;
//            }
//
//            stack = (char*)calloc(s.length() / 2, sizeof(char));
//
//            if (NULL == stack) {
//                return false;
//            }
//
//            for (i = 0; i < s.length(); i++) {
//                if ('{' == s[i] || '[' == s[i] || '(' == s[i]) {
//                    stack[++top] = s[i];
//
//                } else if ('}' == s[i] || ']' == s[i] || ')' == s[i]) {
//                    if (top >= 0) {
//                        if ('}' == s[i] && '{' == stack[top]) {
//                            top--;
//                        } else if (']' == s[i] && '[' == stack[top]) {
//                            top--;
//                        } else if (')' == s[i] && '(' == stack[top]) {
//                            top--;
//                        } else {
//                            result = false;
//                            break;
//                        }
//                    } else {
//                        result = false;
//                        break;
//                    }
//                }
//            }
//
//            free(stack);
//            if(top >= 0) {
//                result = false;
//            }
//            return result;
//        }
//};
