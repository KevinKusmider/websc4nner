#ifndef __CURL_H__
#define __CURL_H__
    #include <liste.h>

    int send_curl(char *url, char * postfield);
    void sendCurl2(char *url);
    int search_lines_in_file(char *fileName, char *find, CHAR_ITEM **start);
    int get_attr_from_line(char *key, char **attr, char *line);
    int analyse(char *fileName);
    void SaveData (char *url, char *inputCommand);

#endif
