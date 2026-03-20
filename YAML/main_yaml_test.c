/**
 * Build:
 *   gcc -std=c11 -Wall -o yaml_parse yaml_parser.c main_yaml_test.c -lcyaml -lyaml
 *
 * Run:
 *   ./yaml_test machine.yaml
 *
 * Example yaml:
 * ----------------------
 * name: MASTER
 * slaves:
 *   - name: alice
 *     value: "some value"
 *   - name: bob
 *     value: "another value"
 *   - name: charlie
 *     value: "age"
 */

#include "yaml_parser.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <yaml_file>\n", argv[0]);
        return EXIT_FAILURE;
    }

    master_t *machine = NULL;

    cyaml_err_t err = load_master_yaml(argv[1], &machine);
    if (err != CYAML_OK) {
        fprintf(stderr, "Failed to load YAML: %s\n", cyaml_strerror(err));
        return EXIT_FAILURE;
    }

    /* Wire slave array into a linked list */
    build_slave_list(machine);

    /* Print */
    print_master(machine);

    /* Clean up */
    free_master_yaml(machine);

    return EXIT_SUCCESS;
}
