#ifndef __CURL_H__
#define __CURL_H__
    #include <liste.h>

    int send_curl(char *url, char * postfield);
    void registerTarget(char *name, char *url);
    int search_lines_in_file(char *fileName, char *find, CHAR_ITEM **start);
    int get_attr_from_line(char *key, char **attr, char *line);
    int analyse(char *fileName);
    void saveData (char *url, char *inputCommand);
    char *findIp(char *begin, char *final, CHAR_ITEM *line);
    char * findCountry(char *begin, char *final, CHAR_ITEM *line);
    void addWebsite (char *name, char *url, char *ip, char *country, char *timeDisplay);

#endif
