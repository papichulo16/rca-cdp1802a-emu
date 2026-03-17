#include <stdio.h>
#include <stdlib.h>
#include <cyaml/cyaml.h>

struct slave {
    char         *name;
    char         *value;
    struct slave *slaves;
    unsigned int  slaves_count;
};

struct master {
    char         *name;
    struct slave *slaves;
    unsigned int  slaves_count;
};

static const cyaml_schema_value_t slave_schema;

static const cyaml_schema_field_t slave_fields[] = {
    CYAML_FIELD_STRING_PTR("name",  CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, struct slave, name,  0, CYAML_UNLIMITED),
    CYAML_FIELD_STRING_PTR("value", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, struct slave, value, 0, CYAML_UNLIMITED),
    CYAML_FIELD_SEQUENCE(  "slaves",CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, struct slave, slaves, &slave_schema, 0, CYAML_UNLIMITED),
    CYAML_FIELD_END
};

static const cyaml_schema_value_t slave_schema = {
    CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, struct slave, slave_fields),
};

static const cyaml_schema_field_t master_fields[] = {
    CYAML_FIELD_STRING_PTR("name",   CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, struct master, name,   0, CYAML_UNLIMITED),
    CYAML_FIELD_SEQUENCE(  "slaves", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, struct master, slaves, &slave_schema, 0, CYAML_UNLIMITED),
    CYAML_FIELD_END
};

static const cyaml_schema_value_t top_schema = {
    CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER, struct master, master_fields),
};

static const cyaml_config_t cfg = {
    .log_fn    = cyaml_log,
    .mem_fn    = cyaml_mem,
    .log_level = CYAML_LOG_DEBUG,
};

int main(int argc, char *argv[]) {
    struct master *m = NULL;
    cyaml_err_t err = cyaml_load_file(argv[1], &cfg, &top_schema, (void **)&m, NULL);
    if (err != CYAML_OK) { fprintf(stderr, "ERROR: %s\n", cyaml_strerror(err)); return 1; }
    printf("name: %s, slaves: %u\n", m->name, m->slaves_count);
    for (unsigned i = 0; i < m->slaves_count; i++)
        printf("  slave: %s\n", m->slaves[i].name);
    cyaml_free(&cfg, &top_schema, m, 0);
    return 0;
}
