#ifndef PARSER_H
#define PARSER_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifndef TRUE
    #define TRUE 1
#endif

#ifndef FALSE
    #define FALSE 1
#endif

/* Definitions*/

struct _XMLNODEList
{
    int heap_size;
    int size;
    struct _XMLNODE** data;
};
typedef struct _XMLNODEList XMLNODEList;

struct _XMLNODE
{
    char* tag;
    char* value;
    struct _XMLNODE* parent;
    struct _XMLNODEList children;
    char* comment; 
};
typedef struct _XMLNODE XMLNODE;



struct _XMLDOC
{
    XMLNODE* root;
};
typedef struct _XMLDOC XMLDOC;



/* Function prototypes*/
int XMLDOC_load(XMLDOC* doc, const char* path);
void XMLDOC_free(XMLDOC* doc);
XMLNODE* XMLNODE_new(XMLNODE* parent);
void XMLNODE_free(XMLNODE* node); 
void XMLNODEList_init(XMLNODEList* list);
void XMLNODEList_add(XMLNODEList* list, XMLNODE* node);
XMLNODE* XMLNODE_child(XMLNODE* parent, int index);
char* GET(char* query, char* file);
void Treeprint(XMLNODE* node);
int XMLDOC_write(XMLDOC* doc, const char* path, int indent);
char* SET(char* query, char* newvalue, char* file);



#endif // xml_parser