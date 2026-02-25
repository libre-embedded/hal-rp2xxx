#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdint.h>

#ifndef __packed
#define __packed __attribute__((packed))
#endif

    typedef struct _binary_info_core binary_info_t;

#define BINARY_INFO_TYPE_RAW_DATA 1
#define BINARY_INFO_TYPE_SIZED_DATA 2
#define BINARY_INFO_TYPE_BINARY_INFO_LIST_ZERO_TERMINATED 3
#define BINARY_INFO_TYPE_BSON 4
#define BINARY_INFO_TYPE_ID_AND_INT 5
#define BINARY_INFO_TYPE_ID_AND_STRING 6
// traditional block device
#define BINARY_INFO_TYPE_BLOCK_DEVICE 7
#define BINARY_INFO_TYPE_PINS_WITH_FUNC 8
#define BINARY_INFO_TYPE_PINS_WITH_NAME 9
#define BINARY_INFO_TYPE_NAMED_GROUP 10
#define BINARY_INFO_TYPE_PTR_INT32_WITH_NAME 11
#define BINARY_INFO_TYPE_PTR_STRING_WITH_NAME 12
#define BINARY_INFO_TYPE_PINS64_WITH_FUNC 13
#define BINARY_INFO_TYPE_PINS64_WITH_NAME 14

// note plan is to reserve c1 = 0->31 for "collision tags"; i.e.
// for which you should always use random IDs with the binary_info,
// giving you 4 + 8 + 32 = 44 bits to avoid collisions
#define BINARY_INFO_MAKE_TAG(c1, c2)                                          \
    ((((unsigned int)c2 & 0xffu) << 8u) | ((unsigned int)c1 & 0xffu))

// Raspberry Pi defined. do not use
#define BINARY_INFO_TAG_RASPBERRY_PI BINARY_INFO_MAKE_TAG('R', 'P')

#define BINARY_INFO_ID_RP_PROGRAM_NAME 0x02031c86
#define BINARY_INFO_ID_RP_PROGRAM_VERSION_STRING 0x11a9bc3a
#define BINARY_INFO_ID_RP_PROGRAM_BUILD_DATE_STRING 0x9da22254
#define BINARY_INFO_ID_RP_BINARY_END 0x68f465de
#define BINARY_INFO_ID_RP_PROGRAM_URL 0x1856239a
#define BINARY_INFO_ID_RP_PROGRAM_DESCRIPTION 0xb6a07c19
#define BINARY_INFO_ID_RP_PROGRAM_FEATURE 0xa1f4b453
#define BINARY_INFO_ID_RP_PROGRAM_BUILD_ATTRIBUTE 0x4275f0d3
#define BINARY_INFO_ID_RP_SDK_VERSION 0x5360b3ab
#define BINARY_INFO_ID_RP_PICO_BOARD 0xb63cffbb
#define BINARY_INFO_ID_RP_BOOT2_NAME 0x7f8882e1

#define bi_ptr_of(x) x *

    typedef struct __packed _binary_info_core
    {
        uint16_t type;
        uint16_t tag;
    } binary_info_core_t;

    typedef struct __packed _binary_info_raw_data
    {
        struct _binary_info_core core;
        uint8_t bytes[1];
    } binary_info_raw_data_t;

    typedef struct __packed _binary_info_sized_data
    {
        struct _binary_info_core core;
        uint32_t length;
        uint8_t bytes[1];
    } binary_info_sized_data_t;

    typedef struct __packed _binary_info_list_zero_terminated
    {
        struct _binary_info_core core;
        bi_ptr_of(binary_info_t) list;
    } binary_info_list_zero_terminated_t;

    typedef struct __packed _binary_info_id_and_int
    {
        struct _binary_info_core core;
        uint32_t id;
        int32_t value;
    } binary_info_id_and_int_t;

    typedef struct __packed _binary_info_id_and_string
    {
        struct _binary_info_core core;
        uint32_t id;
        bi_ptr_of(const char) value;
    } binary_info_id_and_string_t;

    typedef struct __packed _binary_info_ptr_int32_with_name
    {
        struct _binary_info_core core;
        int32_t id;
        bi_ptr_of(const int) value;
        bi_ptr_of(const char) label;
    } binary_info_ptr_int32_with_name_t;

    typedef struct __packed _binary_info_ptr_string_with_name
    {
        struct _binary_info_core core;
        int32_t id;
        bi_ptr_of(const char) value;
        bi_ptr_of(const char) label;
        uint32_t len;
    } binary_info_ptr_string_with_name_t;

    typedef struct __packed _binary_info_block_device
    {
        struct _binary_info_core core;
        bi_ptr_of(const char)
            name; // optional static name (independent of what is formatted)
        uint32_t address;
        uint32_t size;
        bi_ptr_of(binary_info_t) extra; // additional info
        uint16_t flags;
    } binary_info_block_device_t;

#define BI_PINS_ENCODING_RANGE 1
#define BI_PINS_ENCODING_MULTI 2

    typedef struct __packed _binary_info_pins_with_func
    {
        struct _binary_info_core core;
        // p4_5 : p3_5 : p2_5 : p1_5 : p0_5 : func_4 : 010_3 //individual pins
        // p0,p1,p2,p3,p4 ... if fewer than 5 then duplicate p
        //                    phi_5 : plo_5 : func_4 : 001_3 // pin range
        //                    plo-phi inclusive
        uint32_t pin_encoding;
    } binary_info_pins_with_func_t;

    typedef struct __packed _binary_info_pins64_with_func
    {
        struct _binary_info_core core;
        // p6_8 : p5_8 : p4_8 : p3_8 : p2_8 : p1_8 : p0_8 : func_5 : 010_3
        // //individual pins p0,p1,p2 ... if fewer than 7 then duplicate p
        //                    phi_8 : plo_8 : func_5 : 001_3 // pin range
        //                    plo-phi inclusive
        uint64_t pin_encoding;
    } binary_info_pins64_with_func_t;

    typedef struct __packed _binary_info_pins_with_name
    {
        struct _binary_info_core core;
        uint32_t pin_mask;
        bi_ptr_of(const char) label;
    } binary_info_pins_with_name_t;

    typedef struct __packed _binary_info_pins64_with_name
    {
        struct _binary_info_core core;
        uint64_t pin_mask;
        bi_ptr_of(const char) label;
    } binary_info_pins64_with_name_t;

#define BI_NAMED_GROUP_SHOW_IF_EMPTY 0x0001   // default is to hide
#define BI_NAMED_GROUP_SEPARATE_COMMAS 0x0002 // default is newlines
#define BI_NAMED_GROUP_SORT_ALPHA 0x0004      // default is no sort
#define BI_NAMED_GROUP_ADVANCED                                               \
    0x0008 // if set, then only shown in say info -a

    typedef struct __packed _binary_info_named_group
    {
        struct _binary_info_core core;
        uint32_t parent_id;
        uint16_t flags;
        uint16_t group_tag;
        uint32_t group_id;
        bi_ptr_of(const char) label;
    } binary_info_named_group_t;

    enum
    {
        BINARY_INFO_BLOCK_DEV_FLAG_READ =
            1 << 0, // if not readable, then it is basically hidden, but tools
                    // may choose to avoid overwriting it
        BINARY_INFO_BLOCK_DEV_FLAG_WRITE = 1 << 1,
        BINARY_INFO_BLOCK_DEV_FLAG_REFORMAT = 1 << 2, // may be reformatted..

        BINARY_INFO_BLOCK_DEV_FLAG_PT_UNKNOWN = 0 << 4, // unknown free to look
        BINARY_INFO_BLOCK_DEV_FLAG_PT_MBR = 1 << 4,     // expect MBR
        BINARY_INFO_BLOCK_DEV_FLAG_PT_GPT = 2 << 4,     // expect GPT
        BINARY_INFO_BLOCK_DEV_FLAG_PT_NONE = 3 << 4,    // no partition table
    };

#ifdef __cplusplus
}
#endif

#define __bi_decl(name, bi, section_prefix, attr)                             \
    static const attr __attribute__((section(section_prefix __STRING(         \
        name)))) struct _binary_info_core *const name = bi
#define __bi_lineno_var_name __CONCAT(__bi_, __LINE__)
#define __bi_ptr_lineno_var_name __CONCAT(__bi_ptr, __LINE__)
#define __bi_enclosure_check_lineno_var_name                                  \
    __CONCAT(_error_bi_is_missing_enclosing_decl_, __LINE__)
#define __bi_mark_enclosure                                                   \
    static const __unused int __bi_enclosure_check_lineno_var_name = 0;
#if __cplusplus || __GNUC__ >= 8
#define __bi_enclosure_check(x) (x + __bi_enclosure_check_lineno_var_name)
#else
// skip the version check on older GCC non C++, as it doesn't compile.. this is
// only here to catch the user accidentally forgetting to enclose the binary
// item with bi_decl
#define __bi_enclosure_check(x) (x)
#endif
/**
 * \brief Declare some binary information that will be included if the contain
 * source file/line is compiled into the binary
 * \ingroup pico_binary_info
 */
#define bi_decl(_decl)                                                        \
    __bi_mark_enclosure _decl;                                                \
    __bi_decl(__bi_ptr_lineno_var_name, &__bi_lineno_var_name.core,           \
              ".binary_info.keep.", __used);
/**
 * \brief Declare some binary information that will be included if the function
 * containing the decl is linked into the binary. The SDK uses --gc-sections,
 * so functions that are never called will be removed by the linker, and any
 * associated binary information declared this way will also be stripped
 * \ingroup pico_binary_info
 */
#define bi_decl_if_func_used(_decl)                                           \
    ({                                                                        \
        __bi_mark_enclosure _decl;                                            \
        __bi_decl(__bi_ptr_lineno_var_name, &__bi_lineno_var_name.core,       \
                  ".binary_info.", );                                         \
        *(const volatile uint8_t *)&__bi_ptr_lineno_var_name;                 \
    });

#define bi_decl_with_attr(_decl, _attr)                                       \
    __bi_mark_enclosure _attr _decl;                                          \
    __bi_decl(__bi_ptr_lineno_var_name, &__bi_lineno_var_name.core,           \
              ".binary_info.keep.", __used);
#define bi_decl_if_func_used_with_attr(_decl, _attr)                          \
    ({                                                                        \
        __bi_mark_enclosure _attr _decl;                                      \
        __bi_decl(__bi_ptr_lineno_var_name, &__bi_lineno_var_name.core,       \
                  ".binary_info.", );                                         \
        *(const volatile uint8_t *)&__bi_ptr_lineno_var_name;                 \
    });

#define bi_int(_tag, _id, _value)                                             \
    static const struct _binary_info_id_and_int __bi_lineno_var_name = {      \
        .core =                                                               \
            {                                                                 \
                .type = __bi_enclosure_check(BINARY_INFO_TYPE_ID_AND_INT),    \
                .tag = _tag,                                                  \
            },                                                                \
        .id = _id,                                                            \
        .value = _value};

#define bi_string(_tag, _id, _value)                                          \
    static const struct _binary_info_id_and_string __bi_lineno_var_name = {   \
        .core =                                                               \
            {                                                                 \
                .type = __bi_enclosure_check(BINARY_INFO_TYPE_ID_AND_STRING), \
                .tag = _tag,                                                  \
            },                                                                \
        .id = _id,                                                            \
        .value = _value,                                                      \
    }

#define __bi_ptr_int32_with_name(_tag, _id, _label, _value)                   \
    static const struct _binary_info_ptr_int32_with_name                      \
        __bi_lineno_var_name = {                                              \
            .core =                                                           \
                {                                                             \
                    .type = __bi_enclosure_check(                             \
                        BINARY_INFO_TYPE_PTR_INT32_WITH_NAME),                \
                    .tag = _tag,                                              \
                },                                                            \
            .id = _id,                                                        \
            .value = &_value,                                                 \
            .label = _label,                                                  \
    }

#define bi_ptr_int32(_tag, _id, _var, _default)                               \
    __attribute__((section(".data"))) static int _var = _default;             \
    __bi_ptr_int32_with_name(_tag, _id, __STRING(_var), _var)

#define __bi_ptr_string_with_name(_tag, _id, _label, _value, _len)            \
    static const struct _binary_info_ptr_string_with_name                     \
        __bi_lineno_var_name = {                                              \
            .core =                                                           \
                {                                                             \
                    .type = __bi_enclosure_check(                             \
                        BINARY_INFO_TYPE_PTR_STRING_WITH_NAME),               \
                    .tag = _tag,                                              \
                },                                                            \
            .id = _id,                                                        \
            .value = _value,                                                  \
            .label = _label,                                                  \
            .len = _len,                                                      \
    }

#define bi_ptr_string(_tag, _id, _var, _default, _max_len)                    \
    static char _var[_max_len] = _default;                                    \
    __bi_ptr_string_with_name(_tag, _id, __STRING(_var), _var, _max_len)

#define bi_block_device(_tag, _name, _address, _size, _extra, _flags)         \
    static const struct _binary_info_block_device __bi_lineno_var_name = {    \
        .core =                                                               \
            {                                                                 \
                .type = __bi_enclosure_check(BINARY_INFO_TYPE_BLOCK_DEVICE),  \
                .tag = _tag,                                                  \
            },                                                                \
        .name = _name,                                                        \
        .address = _address,                                                  \
        .size = _size,                                                        \
        .extra = _extra,                                                      \
        .flags = _flags,                                                      \
    }

#define __bi_encoded_pins_with_func(_encoding)                                \
    static const struct _binary_info_pins_with_func __bi_lineno_var_name = {  \
        .core =                                                               \
            {                                                                 \
                .type =                                                       \
                    __bi_enclosure_check(BINARY_INFO_TYPE_PINS_WITH_FUNC),    \
                .tag = BINARY_INFO_TAG_RASPBERRY_PI,                          \
            },                                                                \
        .pin_encoding = _encoding}

#define __bi_pins_with_name(_mask, _label)                                    \
    static const struct _binary_info_pins_with_name __bi_lineno_var_name = {  \
        .core =                                                               \
            {                                                                 \
                .type =                                                       \
                    __bi_enclosure_check(BINARY_INFO_TYPE_PINS_WITH_NAME),    \
                .tag = BINARY_INFO_TAG_RASPBERRY_PI,                          \
            },                                                                \
        .pin_mask = _mask,                                                    \
        .label = _label}

#define __bi_named_group(_parent_tag, _parent_id, _group_tag, _group_id,      \
                         _label, _flags)                                      \
    static const struct _binary_info_named_group __bi_lineno_var_name = {     \
        .core =                                                               \
            {                                                                 \
                .type = __bi_enclosure_check(BINARY_INFO_TYPE_NAMED_GROUP),   \
                .tag = _parent_tag,                                           \
            },                                                                \
        .parent_id = _parent_id,                                              \
        .flags = _flags,                                                      \
        .group_tag = _group_tag,                                              \
        .group_id = _group_id,                                                \
        .label = _label}

#define bi_binary_end(end)                                                    \
    bi_int(BINARY_INFO_TAG_RASPBERRY_PI, BINARY_INFO_ID_RP_BINARY_END, end)
#define bi_program_name(name)                                                 \
    bi_string(BINARY_INFO_TAG_RASPBERRY_PI, BINARY_INFO_ID_RP_PROGRAM_NAME,   \
              name)
#define bi_program_description(description)                                   \
    bi_string(BINARY_INFO_TAG_RASPBERRY_PI,                                   \
              BINARY_INFO_ID_RP_PROGRAM_DESCRIPTION, description)
#define bi_program_version_string(version_string)                             \
    bi_string(BINARY_INFO_TAG_RASPBERRY_PI,                                   \
              BINARY_INFO_ID_RP_PROGRAM_VERSION_STRING, version_string)
#define bi_program_build_date_string(date_string)                             \
    bi_string(BINARY_INFO_TAG_RASPBERRY_PI,                                   \
              BINARY_INFO_ID_RP_PROGRAM_BUILD_DATE_STRING, date_string)
#define bi_program_url(url)                                                   \
    bi_string(BINARY_INFO_TAG_RASPBERRY_PI, BINARY_INFO_ID_RP_PROGRAM_URL, url)
// multiple of these may be added
#define bi_program_feature(feature)                                           \
    bi_string(BINARY_INFO_TAG_RASPBERRY_PI,                                   \
              BINARY_INFO_ID_RP_PROGRAM_FEATURE, feature)
#define bi_program_build_attribute(attr)                                      \
    bi_string(BINARY_INFO_TAG_RASPBERRY_PI,                                   \
              BINARY_INFO_ID_RP_PROGRAM_BUILD_ATTRIBUTE, attr)
#define bi_program_feature_group(tag, id, name)                               \
    __bi_named_group(BINARY_INFO_TAG_RASPBERRY_PI,                            \
                     BINARY_INFO_ID_RP_PROGRAM_FEATURE, tag, id, name, 0)
#define bi_program_feature_group_with_flags(tag, id, name, flags)             \
    __bi_named_group(BINARY_INFO_TAG_RASPBERRY_PI,                            \
                     BINARY_INFO_ID_RP_PROGRAM_FEATURE, tag, id, name, flags)
