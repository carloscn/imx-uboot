#include <common.h>
#include <command.h>
#include <cpu.h>
#include <dm.h>
#include <errno.h>
#include <linux/stddef.h>

#define HSE_VERSION "2.0"
#define NONE_HSE_VERSION "1.1"

static int get_cpu_verison(char *version)
{
    int32_t ret = 0;
    size_t len = 0;

    if (NULL == version) {
        ret = CMD_RET_FAILURE;
        error("hal_get_cpu_version failed! NULL pointer!\n");
        goto finish;
    }

    char cpuinfo_str[1024] = {0};
    ret = get_cpuinfo_str(cpuinfo_str);
    if (0 != ret) {
        ret = CMD_RET_FAILURE;
        error("get_cpuinfo_str failed!\n");
        goto finish;
    }

    len = strlen(cpuinfo_str);
    if (0 == len) {
        ret = CMD_RET_FAILURE;
        error("get_cpuinfo_str failed! len is 0!\n");
        goto finish;
    }

    for (size_t i = 0; i < len - 7; i ++) {
        if ('r' == cpuinfo_str[i] &&
            'e' == cpuinfo_str[i + 1] &&
            'v' == cpuinfo_str[i + 2] &&
            (len - i - 2) > 3) {
            size_t j = 0;
            for (j = 0; j < 3; j ++) {
                version[j] = cpuinfo_str[i + j + 3];
            }
            version[j] = '\0';
            goto finish;
        }
    }

    ret = CMD_RET_FAILURE;
    error("no version information!\n");

finish:
    return ret;
}

#define LOAD_ADDR (0x80000020UL)
#define BOOT_SEQ_OFFSET (0x28UL)
static int get_secure_boot_flags(int *result)
{
    int ret = 0;

    if (NULL == result) {
        ret = CMD_RET_FAILURE;
        error("get_secure_boot_flags failed! NULL pointer!\n");
        goto finish;
    }

    unsigned char *load_addr = (unsigned char*)(LOAD_ADDR + BOOT_SEQ_OFFSET);
    *result = (int) load_addr[0];

finish:
    return ret;
}

static int do_utils_load(cmd_tbl_t *cmdtp, int flag, int argc, char* const argv[])
{
    int ret = 0;
    int i = 0;
    char version[1024] = {0};

    printf("[INFO] The input is %d\n", argc);
    for (i = 0; i < argc; i ++) {
        printf("[INFO] the argv[%d] is %s\n", i, argv[i]);
    }

    ret = get_cpu_verison(version);
    if (ret != 0) {
        ret = CMD_RET_FAILURE;
        goto finish;
    }

    printf("[INFO] %s\n", version);

    int sec_flag = 0;
    ret = get_secure_boot_flags(&sec_flag);
    if (ret != 0) {
        ret = CMD_RET_FAILURE;
        goto finish;
    }

    printf("[INFO] secure flags is %d\n", sec_flag);

finish:
    return ret;
}

U_BOOT_CMD(
    utils_load,
    5,
    1,
    do_utils_load,
    "format : utils_load address",
    "example: utils_load 0x80000000"
);