#ifndef __CURL_H__
#define __CURL_H__
    #include <liste.h>

    int send_curl(char *url, char * postfield);
    void register_target();
    char * findIp(char *begin, char *final, CHAR_ITEM *line);
    char * findCountry(char *begin, char *final, CHAR_ITEM *line);
    int search_lines_in_file(char *fileName, char *find, CHAR_ITEM **start);
    int get_attr_from_line(char *key, char **attr, char *line);
    int analyse(char *fileName);
    void saveData (char *url, char *inputCommand);

#endif
