#define NOB_IMPLEMENTATION
#include "src/nob.h"
#include <stddef.h>
#include <string.h>
#include <stdbool.h>

// TODO: make async with procs
bool build_target(char *outfile, char *infile)
{
    bool result = true;
    Nob_Cmd cmd = { 0 };

    nob_log(NOB_INFO, "Building: %s", infile);
    nob_log(NOB_INFO, "out: %s", outfile);
    nob_cmd_append(&cmd, "cc");
    nob_cmd_append(&cmd, "-Wall", "-Wextra", "-pedantic");
    nob_cmd_append(&cmd, "-o", outfile);
    nob_cmd_append(&cmd, infile);
    nob_cmd_append(&cmd, "-lm");

    if (!nob_cmd_run_sync(cmd)) result = false;

    nob_cmd_free(cmd);
    return result;
}

bool build_tests(char *outfile, char *infile)
{
    bool result = true;
    Nob_Cmd cmd = { 0 };

    nob_log(NOB_INFO, "Building: %s", infile);
    nob_log(NOB_INFO, "out: %s", outfile);
    nob_cmd_append(&cmd, "cc");
    nob_cmd_append(&cmd, "-Wall", "-Wextra", "-pedantic");
    nob_cmd_append(&cmd, "-o", outfile);
    nob_cmd_append(&cmd, infile);
    nob_cmd_append(&cmd, "-lm");
    nob_cmd_append(&cmd, "-DTEST");

    if (!nob_cmd_run_sync(cmd)) result = false;

    nob_cmd_free(cmd);
    return result;
}

int main(int argc, char **argv)
{
    NOB_GO_REBUILD_URSELF(argc, argv);

    nob_log(NOB_INFO, "-- STAGE 1 --");
    if (!nob_mkdir_if_not_exists("target")) return 1;

    char *program = nob_shift_args(&argc, &argv);

    bool flag_test = false;
    if (argc > 0) {
        char *operand = nob_shift_args(&argc, &argv);
        if (strcmp(operand, "test") == 0) {
            nob_log(NOB_INFO, "Enabling tests");
            flag_test = true;
            if (!nob_mkdir_if_not_exists("target/tests")) return 1;
        } else {
            nob_log(NOB_ERROR, "Invalid operand: `%s`", operand);
        }
    }

    Nob_File_Paths files = { 0 };
    nob_read_entire_dir("src", &files);

    size_t i;
    for (i = 0; i < files.count; ++i) {
        if (strncmp(files.items[i] + strlen(files.items[i]) - 2,".c", 2) == 0) {
            Nob_String_Builder infile = { 0 };
            nob_sb_append_cstr(&infile, "./src/");
            nob_sb_append_cstr(&infile, files.items[i]);
            nob_sb_append_null(&infile);

            Nob_String_Builder outfile = { 0 };
            nob_sb_append_cstr(&outfile, "./target/");
            if (flag_test) nob_sb_append_cstr(&outfile, "tests/");
            nob_sb_append_cstr(&outfile, files.items[i]);
            outfile.count -= 2;
            nob_sb_append_null(&outfile);

            if (nob_needs_rebuild1(outfile.items, infile.items)) {
                nob_log(NOB_INFO, "Building: %s", infile.items);
                if (flag_test) {
                    if (!build_tests(outfile.items, infile.items)) return 1;
                } else {
                    if (!build_target(outfile.items, infile.items)) return 1;
                }
            }

            nob_sb_free(outfile);
            nob_sb_free(infile);
        }
    }

    // if (!build_target()) return 1;
    return 0;
}
