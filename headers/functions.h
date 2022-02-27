#ifndef __FUNCTIONS_H__
#define __FUNCTION_H__

    void clean_stdin();
    int trim(char **string);
    int checkIntResponse(int value, int min, int max);
    int askForInt(char *question, int min, int max);

#endif