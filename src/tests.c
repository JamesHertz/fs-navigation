// testing framework stuffs
#include <criterion/criterion.h>
#include <criterion/new/assert.h>

// stdlib headers
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <errno.h>
#include <assert.h>

// my own headers
#include "records.h"

#define EXAMPLES_NR (sizeof(examples) / sizeof(Record))
#define cr_assert_int_eq(x, y) cr_assert(eq(int, x, y))

// use to clear resources
static struct {
    char * filename;
    RecordsManager * manager;
} resources;

// used for some tests
static Record examples[] = {
    { .name = "a", .path = "/path/to/a" },
    { .name = "b", .path = "/path/to/b" },
    { .name = "c", .path = "/path/to/c" },
    { .name = "d", .path = "/path/to/c" },
    { .name = "e", .path = "/path/to/e" },
    { .name = "f", .path = "/path/to/f" },
    { .name = "g", .path = "/path/to/g" },
};

static void free_resources(){
    if(resources.manager != NULL){
        rm_destroy(resources.manager);
        resources.manager = NULL;
    }

    if(resources.filename != NULL) {
        unlink(resources.filename);
        free(resources.filename);
        resources.filename = NULL;
    }
}

static RecordsManager * create_rm(const char * filename){
    cr_assert_null(resources.manager, "Yes, you messed up! This time you are leaking memory :c");

    errno = 0;
    RecordsManager * rm = rm_load_records(filename);
    if(rm == NULL)
        cr_fail("Failed to load records from '%s': %s", filename, strerror(errno));

    resources.manager = rm;
    return rm;
}

static int destroy_rm(RecordsManager * manager){
    resources.manager = NULL;
    return rm_destroy(manager);
}

static const char * get_temp_filename(){
    char * template = strdup(".fs-base-XXXXXX");
    cr_assert_not_null(template);

    errno = 0;
    int fd = mkstemp(template);
    if(fd < 0) {
        free(template);
        cr_fail("Unable to create temporary file: %s\n", strerror(errno));
    }

    close(fd);

    resources.filename = template;
    return template;
}

static void write_file(const char * filename, const char * content){
    FILE * file = fopen(filename, "w");
    cr_assert_not_null(file);
    cr_assert(fwrite(content, sizeof(char), strlen(content), file) > 0);

    fflush(file);
    fclose(file);
}

Test(basic, load_records_empty_file, .fini = free_resources) {
    const char * filename = get_temp_filename();
    unlink(filename);

    RecordsManager * rm = create_rm(filename);
    cr_assert_eq(0, rm_records_number(rm));
    destroy_rm(rm);
}

Test(basic, load_records_non_empty_file, .fini = free_resources) {
    const char * filename = get_temp_filename();

    write_file(filename,
        "home\t/home\n"
        "nav\t/path/to/fs-nav\n"
        "root\t/"
    );

    RecordsManager * rm = create_rm(filename);
    cr_assert_eq(rm_records_number(rm), 3);

    cr_assert_str_eq(rm_find_path(rm, "home"), "/home");
    cr_assert_str_eq(rm_find_path(rm, "root"), "/");
    cr_assert_str_eq(rm_find_path(rm, "nav"), "/path/to/fs-nav");

    destroy_rm(rm);
}

Test(basic, put_and_find_path, .fini = free_resources) {
    RecordsManager * rm = create_rm( get_temp_filename() );
    cr_assert_eq(rm_records_number(rm), 0);

    for(size_t i = 0; i < EXAMPLES_NR; i++){
        Record * rec = &examples[i];
        cr_assert_null(rm_put_record(rm, rec->name, rec->path));
        cr_assert_eq(rm_records_number(rm), i + 1);
    }

    char * path = rm_put_record(rm, examples[0].name, examples[0].path);
    cr_assert_str_eq(path, examples[0].path);
    cr_assert_eq(rm_records_number(rm), EXAMPLES_NR);
    free(path); // TODO: when you change rm_put_record please remeber this big boy c:

    for(size_t i = 0; i < EXAMPLES_NR; i++){
        Record * rec = &examples[i];
        cr_assert_str_eq(rm_find_path(rm, rec->name), rec->path);
    }

    destroy_rm(rm);
}

Test(basic, remove, .fini = free_resources) {
    RecordsManager * rm = create_rm( get_temp_filename() );
    cr_assert_eq(rm_records_number(rm), 0);

    cr_assert_null(rm_remove_record(rm, "a"));
    cr_assert_null(rm_put_record(rm, "a", "aaa"));
    cr_assert_null(rm_put_record(rm, "b", "bbb"));
    cr_assert_null(rm_put_record(rm, "c", "ccc"));

    char * path = rm_remove_record(rm, "a");
    cr_assert_str_eq(path, "aaa");
    cr_assert_eq(rm_records_number(rm), 2);
    free(path);

    path = rm_remove_record(rm, "b");
    cr_assert_str_eq(path, "bbb");
    cr_assert_eq(rm_records_number(rm), 1);
    free(path);

    path = rm_remove_record(rm, "c");
    cr_assert_str_eq(path, "ccc");
    cr_assert_eq(rm_records_number(rm), 0);
    free(path);

    destroy_rm(rm);
}

Test(basic, serialization, .fini = free_resources){
    const char * filename = get_temp_filename();
    RecordsManager * rm   = create_rm(filename);

    // NOTE: Write the whole examples, read it and check if it remains the same
    for(size_t i = 0; i < EXAMPLES_NR; i++)
        cr_assert_null(rm_put_record(rm, examples[i].name, examples[i].path));

    destroy_rm(rm);

    rm = create_rm(filename);
    cr_assert_int_eq(rm_records_number(rm), EXAMPLES_NR);

    for(size_t i = 0; i < EXAMPLES_NR; i++)
        cr_assert_str_eq(rm_find_path(rm, examples[i].name), examples[i].path);

    // NOTE: Remove 3 records write, read and see it has the changes
    static_assert(EXAMPLES_NR > 3);
    for(size_t i = 0; i < 3; i++) {
        char * path = rm_remove_record(rm, examples[i].name);
        cr_assert_not_null(path);
        free(path);
    }

    destroy_rm(rm);
    rm = create_rm(filename);
    cr_assert_int_eq(rm_records_number(rm), EXAMPLES_NR - 3);

    for(size_t i = 0; i < EXAMPLES_NR - 3; i++) {
        size_t idx = i + 3;
        cr_assert_str_eq(rm_find_path(rm, examples[idx].name), examples[idx].path);
    }

    for(size_t i = 0; i < 3; i++)
        cr_assert_null(rm_find_path(rm, examples[i].name));

    // NOTE: change one record, write, read and see if it has the change
    char * path = rm_put_record(rm, examples[3].name, "/some/random/path");
    cr_assert_not_null(path);
    free(path);

    destroy_rm(rm);
    rm = create_rm(filename);
    cr_assert_int_eq(rm_records_number(rm), EXAMPLES_NR - 3);

    static_assert(EXAMPLES_NR > 4);

    cr_assert_str_eq(rm_find_path(rm, examples[3].name), "/some/random/path");
    for(size_t i = 0; i < EXAMPLES_NR - 4; i++) {
        size_t idx = i + 4;
        cr_assert_str_eq(rm_find_path(rm, examples[idx].name), examples[idx].path);
    }
}
