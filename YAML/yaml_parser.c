#include "yaml_parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const cyaml_schema_value_t slave_schema;

static const cyaml_schema_field_t slave_fields_schema[] = {
    CYAML_FIELD_STRING_PTR("name", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, struct slave, name, 0, CYAML_UNLIMITED),
    CYAML_FIELD_STRING_PTR("value", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, struct slave, value, 0, CYAML_UNLIMITED),
    CYAML_FIELD_SEQUENCE("slaves", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, struct slave, slaves, &slave_schema, 0, CYAML_UNLIMITED),
    CYAML_FIELD_END
};

static const cyaml_schema_value_t slave_schema = {
    CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, struct slave, slave_fields_schema),
};

static const cyaml_schema_field_t master_fields_schema[] = {
    CYAML_FIELD_STRING_PTR("name", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, struct master, name, 0, CYAML_UNLIMITED),
    CYAML_FIELD_SEQUENCE("slaves", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL, struct master, slaves, &slave_schema, 0, CYAML_UNLIMITED),
    CYAML_FIELD_END
};

static const cyaml_schema_value_t top_schema = {
    CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER, struct master, master_fields_schema),
};

static const cyaml_config_t cyaml_cfg = {
    .log_fn    = cyaml_log,
    .mem_fn    = cyaml_mem,
    .log_level = CYAML_LOG_WARNING,
    .flags     = CYAML_CFG_IGNORE_UNKNOWN_KEYS,
};

cyaml_err_t load_master_yaml(const char *filepath, master_t **out)
{
    if (filepath == NULL || out == NULL) {
        return CYAML_ERR_BAD_PARAM_NULL_DATA;
    }
    return cyaml_load_file(filepath, &cyaml_cfg, &top_schema, (cyaml_data_t **)out, NULL);
}


static void build_slave_list_recursive(slave_t *slaves, unsigned int count)
{
    for (unsigned int i = 0; i < count; i++) {
        slaves[i].next = (i + 1 < count) ? &slaves[i + 1] : NULL;
        if (slaves[i].slaves != NULL)
            build_slave_list_recursive(slaves[i].slaves, slaves[i].slaves_count);
    }
}

void build_slave_list(master_t *master)
{
    if (master == NULL || master->slaves == NULL || master->slaves_count == 0)
        return;
    build_slave_list_recursive(master->slaves, master->slaves_count);
}


void free_master_yaml(master_t *master)
{
    if (master == NULL) return;
    cyaml_err_t err = cyaml_free(&cyaml_cfg, &top_schema, master, 0);
    if (err != CYAML_OK)
        fprintf(stderr, "yaml_parser: cyaml_free error: %s\n", cyaml_strerror(err));
}

static void print_slaves(const slave_t *s, unsigned int depth)
{
    char indent[64] = {0};
    for (unsigned int i = 0; i < depth * 2; i++) indent[i] = ' ';

    unsigned int idx = 0;
    while (s != NULL) {
        printf("%s[%u] name=%-16s  value=%s\n",
               indent, idx,
               s->name  ? s->name  : "(null)",
               s->value ? s->value : "(null)");
        if (s->slaves != NULL)
            print_slaves(s->slaves, depth + 1);
        s = s->next;
        idx++;
    }
}

void print_master(const master_t *master)
{
    if (master == NULL) {
	printf("[master] (null)\n");
	return;
    }

    printf("[master] name: %s\n", master->name ? master->name : "(none)");
    printf("[master] slaves (%u):\n", master->slaves_count);
    print_slaves(master->slaves, 1);
}
