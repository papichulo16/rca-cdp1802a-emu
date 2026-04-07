
#ifndef YAML_PARSER_H
#define YAML_PARSER_H

#include <stdint.h>
#include <cyaml/cyaml.h>

/* -------------------------------------------------------------------------
 * Slaves are linked list objs; lives in master obj
 * The 'next' pointer is managed manually after libcyaml loads the array
 * ---------------------------------------------------------------------- */
typedef struct slave slave_t;

struct slave {
    char        *name;          /* Key name from YAML.. "charlie", "age" */
    char        *value;         /* String value (optional , also might be NULL)       */
    slave_t     *slaves;          /* Linked-list pointer (build_slave_list sets) */
    unsigned int slaves_count;
    slave_t	*next;
};

/* -------------------------------------------------------------------------
 * Master object
 * Top-level; loaded by YAML
 * slaves_count from libcyaml (sequence length).
 * ---------------------------------------------------------------------- */

typedef struct master {
    char        *name;          /* Master identifier		              */
    slave_t     *slaves;        /* Pointer to first slave in the linked list  */
    unsigned int slaves_count;  /* Number of slaves (filled by libcyaml)      */
} master_t;

cyaml_err_t load_master_yaml(const char *filepath, master_t **out);

void build_slave_list(master_t *master);
void free_master_yaml(master_t *master);
void print_master(const master_t *master);
void parse_yaml(char* file);

#endif /* YAML_PARSER_H */
