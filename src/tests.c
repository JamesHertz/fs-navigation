// testing framework stuffs
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

// stdlib headers
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>

// my own headers
#include "records.h"

char * get_temp_filename(){
    char * template = strdup(".fs-base-XXXXXX");
    cr_assert_not_null(template);

    errno = 0;
    int fd = mkstemp(template);
    if(fd < 0) {
        free(template);
        cr_fail("Unable to create temporary file: %s\n", strerror(errno));
    }

    close(fd);
    return template;
}

Test(load_records, empty_file) {
    char * filename = get_temp_filename();

    errno = 0;

    RecordsManager * rm = rm_load_records(filename);
    if(rm == NULL){
        cr_log_error("Failed to load records from '%s': %s", filename, strerror(errno));
        free(filename);
        cr_fail("Due to previos error");
    }

    free(filename);
    cr_assert_eq(0, rm_records_number(rm));

    rm_destroy(rm);
}

