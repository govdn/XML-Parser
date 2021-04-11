#include "parser.h"
char header[255];

//ends with
int ends_with(const char* haystack, const char* needle)
{
    int h_len = strlen(haystack);
    int n_len = strlen(needle);

    if (h_len < n_len)
        return FALSE;

    for (int i = 0; i < n_len; i++) {
        if (haystack[h_len - n_len + i] != needle[i])
            return FALSE;
    }

    return TRUE;
}

//implementing helper function
XMLNODE* XMLNODE_child(XMLNODE* parent, int index){
    return parent->children.data[index];
}


//implementing  new node function  
XMLNODE* XMLNODE_new(XMLNODE* parent){
    XMLNODE* node = (XMLNODE*)malloc(sizeof(XMLNODE));
    node->parent = parent;
    node->tag = NULL;
    node->value = NULL;
    node->comment = NULL;
    XMLNODEList_init(&node->children);
    if(parent){
        XMLNODEList_add(&parent->children, node);
        parent->value = NULL;
    }
    return node;
}

// implementing free node function
void XMLNODE_free(XMLNODE* node){
    if(node->tag)
        free(node->tag);
    if(node->value)
        free(node->value);
    free(node);
}

//implement of new node list
void XMLNODEList_init(XMLNODEList* list){
    list->heap_size = 1;
    list->size = 0;
    list->data = (XMLNODE**)malloc(sizeof(XMLNODE*)*list->heap_size);
}

//adding node to list
void XMLNODEList_add(XMLNODEList* list, XMLNODE* node){
    while(list->size >= list->heap_size){
        list->heap_size *= 2;
        list->data = (XMLNODE**)realloc(list->data, sizeof(XMLNODE*)*list->heap_size);
    }
    list->data[list->size++] = node;
}


//implementing load function 

int XMLDOC_load(XMLDOC* doc, const char* path){
    /*reading from file*/
    FILE* file = fopen(path, "r");
    if(!file){
        fprintf(stderr, "Could not load file from '%s'\n", path);
        return FALSE;
    }
    fseek(file, 0, SEEK_END);
    int size = ftell(file);
    fseek(file, 0, SEEK_SET); 
    char* buff = (char*)malloc(sizeof(char)*size+1);
    fread(buff, 1, size, file);
    fclose(file);
    buff[size] = '\0';
    doc->root = XMLNODE_new(NULL);
    
    /*lexical operation*/
    char lex[256];
    int lexi = 0;
    int i =0;

    XMLNODE* curr_node = doc->root;
    while(buff[i] != '\0'){                                //loop till file end
       if(buff[i] == '<') {                                //check for start of start tag or end tag

           lex[lexi] = '\0';                               //adding NULL at end to perform Stirng operation 

            // XML header
            if (buff[i + 1] == '?') {
                while (buff[i] != '>')
                    lex[lexi++] = buff[i++];
                lex[lexi] = '\0';

                // Comments
                if (strcmp(lex, "<?")) {
                    lex[lexi] = '\0';
                    while (!ends_with(lex, "?>")) {
                        lex[lexi++] = buff[i++];
                        lex[lexi] = '\0';
                    }
                    lex[lexi++] = '>';
                    lex[lexi] = '\0';
                    strcpy(header, lex);
                    lexi  = 0;
                    continue;
                }
            }

            // Special nodes
            else if (buff[i + 1] == '!') {
                while (buff[i] != '>')
                    lex[lexi++] = buff[i++];
                lex[lexi] = '\0';

                // Comments
                if (strcmp(lex, "<!--")) {
                    lex[lexi] = '\0';
                    while (!ends_with(lex, "-->")) {
                        lex[lexi++] = buff[i++];
                        lex[lexi] = '\0';
                    }
                    lex[lexi++] = '>';
                    lex[lexi] = '\0';
                    curr_node->comment = strdup(lex);
                    lexi = 0;
                    continue;
                }
            }


           //value of tag
           if(lexi > 0){
               if(!curr_node){                                //enters this loop if text is outside tag
                fprintf(stderr, "Text outside of document\n");
                return FALSE;
               }
                curr_node->value = strdup(lex);
               lexi = 0; 
           }


           //end of the node
           if(buff[i+1] == '/'){
               i+=2;
                while(buff[i] != '>')
                    lex[lexi++] = buff[i++];
                lex[lexi] = '\0';
                if(!curr_node){
                   fprintf(stderr, "Already at the root\n");
                   return FALSE; 
                }
                if(strcmp(curr_node->tag, lex)){
                    fprintf(stderr, "Mismatch Tags (%s != %s)\n", curr_node->tag, lex);
                    return FALSE;
                }
                curr_node = curr_node->parent;
                i++;
                continue;                 
           }


           // set current node
            curr_node = XMLNODE_new(curr_node);
           
           //Set tag name
           i++;
           while(buff[i] != '>')
               lex[lexi++] = buff[i++];
            lex[lexi] = '\0';
            curr_node->tag = strdup(lex);

            //reset lexi
            lexi = 0;
            i++;
            continue;
       }else{
           lex[lexi++] = buff[i++];
       }
    }
    return TRUE;
}

void XMLDOC_free(XMLDOC* doc){
    XMLNODE_free(doc->root);
}

char* GET(char* query, char* file){
    XMLDOC doc;
    if(XMLDOC_load(&doc, file)){
        XMLNODE* temp;
        temp = doc.root;
        char buff[256];
        int i = 0;
        int index = 0;
        while (query[index] !='\0'){
            int i= 0;
            while(query[index] !='.' && query[index] !='\0')
                buff[i++] = query[index++]; 
            buff[i] = '\0';
            if(query[index] !='\0')
                index++;
            for(int j = 0; j < temp->children.size;j++){
                if(!strcmp(temp->children.data[j]->tag, buff)){
                    temp = temp->children.data[j];
                    continue;
                }
            }

        }
        if(temp->children.size == 0){
            XMLDOC_free(&doc);
            return temp->value;
        }
    }
    XMLDOC_free(&doc);
    return "Error: Either this has no value or it is invalid tag";    
}

void Treeprint(XMLNODE* node){
    if(node->children.size == 0){
        printf("%s %s\n",node->tag, node->value);
        return;
    }
    else{
        printf("%s %s\n",node->tag, node->value);
        for(int i = 0; i< node->children.size; i++){
            Treeprint(node->children.data[i]);
        }
        return;
    } 
}

char* SET(char* query, char* newvalue, char* file){
    XMLDOC doc;
    if(XMLDOC_load(&doc, file)){
        XMLNODE* temp;
        temp = doc.root;
        char buff[256];
        int i = 0;
        int index = 0;
        while (query[index] !='\0'){
            int i= 0;
            while(query[index] !='.' && query[index] !='\0')
                buff[i++] = query[index++]; 
            buff[i] = '\0';
            if(query[index] !='\0')
                index++;
            for(int j = 0; j < temp->children.size;j++){
                if(!strcmp(temp->children.data[j]->tag, buff)){
                    temp = temp->children.data[j];
                    continue;
                }
            }

        }
        if(temp->children.size == 0)
        {
            temp->value = newvalue;
        }
        else{
            XMLDOC_free(&doc);
            return "Error: Either node has children or it is invalid tag";
        }
    }
    XMLDOC_write(&doc, file,3);
    XMLDOC_free(&doc);
    return "Write is complete";    
}

static void node_out(FILE* file, XMLNODE* node, int indent, int times){
    for(int i = 0; i< node->children.size; i++){
        XMLNODE* child = node->children.data[i];
        if(times > 0)
            fprintf(file, "%*s", indent * times, " ");
        fprintf(file,"<%s", child->tag);
        if (child->children.size == 0 && !child->value)
            fprintf(file, " />\n");
        else {
            fprintf(file, ">");
            if (child->children.size == 0)
                fprintf(file, "%s</%s>\n", child->value, child->tag);
            else {
                fprintf(file, "\n");
                node_out(file, child, indent, times + 1);
                if (times > 0)
                    fprintf(file, "%*s", indent * times, " ");
                fprintf(file, "</%s>\n", child->tag);
            }
        }
    }
}

int XMLDOC_write(XMLDOC* doc, const char* path, int indent){
    FILE* file = fopen(path, "w");
    if(!file){
        fprintf(stderr,"Could not open file %s \n",path);
        return FALSE;
    }
    fprintf(file, "%s\n", header);
    node_out(file, doc->root, indent, 0);
    fclose(file);
}