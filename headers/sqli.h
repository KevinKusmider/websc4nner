#ifndef __SQLI_H__
#define __SQLI_H_

    void injection_sql();
    int get_forms(char *fileName, FORM_ITEM **forms);
    char * get_form_start_tag(FILE * file);
    char * get_input_tag(FILE * file);

#endif