#pragma once

#include <cstring>
#include <dlfcn.h>
#include "api.hpp"

namespace {

    struct ZygiskContext;
    struct ZygiskModule;

    struct AppSpecializeArgs_v1;
    using  AppSpecializeArgs_v2 = AppSpecializeArgs_v1;
    struct AppSpecializeArgs_v3;
    using  AppSpecializeArgs_v4 = AppSpecializeArgs_v3;
    struct AppSpecializeArgs_v5;

    struct module_abi_v1;
    using  module_abi_v2 = module_abi_v1;
    using  module_abi_v3 = module_abi_v1;
    using  module_abi_v4 = module_abi_v1;
    using  module_abi_v5 = module_abi_v1;

    struct api_abi_v1;
    struct api_abi_v2;
    using  api_abi_v3 = api_abi_v2;
    struct api_abi_v4;
    using  api_abi_v5 = api_abi_v4;

    union ApiTable;

    struct AppSpecializeArgs_v3 {
        jint &uid;
        jint &gid;
        jintArray &gids;
        jint &runtime_flags;
        jobjectArray &rlimits;
        jint &mount_external;
        jstring &se_info;
        jstring &nice_name;
        jstring &instruction_set;
        jstring &app_data_dir;

        jintArray *fds_to_ignore = nullptr;
        jboolean *is_child_zygote = nullptr;
        jboolean *is_top_app = nullptr;
        jobjectArray *pkg_data_info_list = nullptr;
        jobjectArray *whitelisted_data_info_list = nullptr;
        jboolean *mount_data_dirs = nullptr;
        jboolean *mount_storage_dirs = nullptr;

        AppSpecializeArgs_v3(
                jint &uid, jint &gid, jintArray &gids, jint &runtime_flags,
                jobjectArray &rlimits, jint &mount_external, jstring &se_info, jstring &nice_name,
                jstring &instruction_set, jstring &app_data_dir) :
                uid(uid), gid(gid), gids(gids), runtime_flags(runtime_flags), rlimits(rlimits),
                mount_external(mount_external), se_info(se_info), nice_name(nice_name),
                instruction_set(instruction_set), app_data_dir(app_data_dir) {}
    };

    struct AppSpecializeArgs_v5 : public AppSpecializeArgs_v3 {
      jboolean *mount_sysprop_overrides = nullptr;

      AppSpecializeArgs_v5(
              jint &uid, jint &gid, jintArray &gids, jint &runtime_flags,
              jobjectArray &rlimits, jint &mount_external, jstring &se_info, jstring &nice_name,
              jstring &instruction_set, jstring &app_data_dir) : AppSpecializeArgs_v3(
                      uid, gid, gids, runtime_flags, rlimits, mount_external,
                      se_info, nice_name, instruction_set, app_data_dir) {}
    };

    struct AppSpecializeArgs_v1 {
        jint &uid;
        jint &gid;
        jintArray &gids;
        jint &runtime_flags;
        jint &mount_external;
        jstring &se_info;
        jstring &nice_name;
        jstring &instruction_set;
        jstring &app_data_dir;

        jboolean *const is_child_zygote;
        jboolean *const is_top_app;
        jobjectArray *const pkg_data_info_list;
        jobjectArray *const whitelisted_data_info_list;
        jboolean *const mount_data_dirs;
        jboolean *const mount_storage_dirs;

        AppSpecializeArgs_v1(const AppSpecializeArgs_v5 *a) :
                uid(a->uid), gid(a->gid), gids(a->gids), runtime_flags(a->runtime_flags),
                mount_external(a->mount_external), se_info(a->se_info), nice_name(a->nice_name),
                instruction_set(a->instruction_set), app_data_dir(a->app_data_dir),
                is_child_zygote(a->is_child_zygote), is_top_app(a->is_top_app),
                pkg_data_info_list(a->pkg_data_info_list),
                whitelisted_data_info_list(a->whitelisted_data_info_list),
                mount_data_dirs(a->mount_data_dirs), mount_storage_dirs(a->mount_storage_dirs) {}
    };

    struct ServerSpecializeArgs_v1 {
        jint &uid;
        jint &gid;
        jintArray &gids;
        jint &runtime_flags;
        jlong &permitted_capabilities;
        jlong &effective_capabilities;

        ServerSpecializeArgs_v1(
                jint &uid, jint &gid, jintArray &gids, jint &runtime_flags,
                jlong &permitted_capabilities, jlong &effective_capabilities) :
                uid(uid), gid(gid), gids(gids), runtime_flags(runtime_flags),
                permitted_capabilities(permitted_capabilities),
                effective_capabilities(effective_capabilities) {}
    };

    struct module_abi_v1 {
        long api_version;
        void *impl;
        void (*preAppSpecialize)(void *, void *);
        void (*postAppSpecialize)(void *, const void *);
        void (*preServerSpecialize)(void *, void *);
        void (*postServerSpecialize)(void *, const void *);
    };

    enum : uint32_t {
        PROCESS_GRANTED_ROOT = zygisk::StateFlag::PROCESS_GRANTED_ROOT,
        PROCESS_ON_DENYLIST = zygisk::StateFlag::PROCESS_ON_DENYLIST,

        PROCESS_IS_MANAGER = (1u << 27),
        PROCESS_ROOT_IS_APATCH = (1u << 28),
        PROCESS_ROOT_IS_KSU = (1u << 29),
        PROCESS_ROOT_IS_MAGISK = (1u << 30),
        PROCESS_IS_FIRST_STARTED = (1u << 31),

        PRIVATE_MASK = PROCESS_IS_FIRST_STARTED
    };

    struct api_abi_base {
        ZygiskModule *impl;
        bool (*registerModule)(ApiTable *, long *);
    };

    struct api_abi_v1 : public api_abi_base {
        /* 0 */ void (*hookJniNativeMethods)(JNIEnv *, const char *, JNINativeMethod *, int);
        /* 1 */ void (*pltHookRegister)(const char *, const char *, void *, void **);
        /* 2 */ void (*pltHookExclude)(const char *, const char *);
        /* 3 */ bool (*pltHookCommit)();
        /* 4 */ int (*connectCompanion)(ZygiskModule *);
        /* 5 */ void (*setOption)(ZygiskModule *, zygisk::Option);
    };

    struct api_abi_v2 : public api_abi_v1 {
        /* 6 */ int (*getModuleDir)(ZygiskModule *);
        /* 7 */ uint32_t (*getFlags)(ZygiskModule *);
    };

    struct api_abi_v4 : public api_abi_base {
        /* 0 */ void (*hookJniNativeMethods)(JNIEnv *, const char *, JNINativeMethod *, int);
        /* 1 */ void (*pltHookRegister)(dev_t, ino_t, const char *, void *, void **);
        /* 2 */ bool (*exemptFd)(int);
        /* 3 */ bool (*pltHookCommit)();
        /* 4 */ int (*connectCompanion)(ZygiskModule *);
        /* 5 */ void (*setOption)(ZygiskModule *, zygisk::Option);
        /* 6 */ int (*getModuleDir)(ZygiskModule *);
        /* 7 */ uint32_t (*getFlags)(ZygiskModule *);
    };

    union ApiTable {
        api_abi_base base;
        api_abi_v1 v1;
        api_abi_v2 v2;
        api_abi_v4 v4;
    };

#define call_app(method)               \
switch (*mod.api_version) {            \
case 1:                                \
case 2: {                              \
    AppSpecializeArgs_v1 a(args);      \
    mod.v1->method(mod.v1->impl, &a);  \
    break;                             \
}                                      \
case 3:                                \
case 4:                                \
case 5:                                \
    mod.v1->method(mod.v1->impl, args);\
    break;                             \
}

    struct ZygiskModule {

        void onLoad(void *env) {
            entry.fn(&api, env);
        }
        void preAppSpecialize(AppSpecializeArgs_v5 *args) const {
            call_app(preAppSpecialize)
        }
        void postAppSpecialize(const AppSpecializeArgs_v5 *args) const {
            call_app(postAppSpecialize)
        }
        void preServerSpecialize(ServerSpecializeArgs_v1 *args) const {
            mod.v1->preServerSpecialize(mod.v1->impl, args);
        }
        void postServerSpecialize(const ServerSpecializeArgs_v1 *args) const {
            mod.v1->postServerSpecialize(mod.v1->impl, args);
        }

        bool valid() const;
        int connectCompanion() const;
        int getModuleDir() const;
        void setOption(zygisk::Option opt);
        static uint32_t getFlags();
        bool tryUnload() const { return unload && dlclose(handle) == 0; };
        void clearApi() { memset(&api, 0, sizeof(api)); }
        int getId() const { return id; }
        void *getHandle() const { return handle; }

        ZygiskModule(int id, void *handle, void *entry);

        static bool RegisterModuleImpl(ApiTable *api, long *module);

    private:
        const int id;
        bool unload = false;

        void * const handle;
        union {
            void * const ptr;
            void (* const fn)(void *, void *);
        } entry;

        ApiTable api;

        union {
            long *api_version;
            module_abi_v1 *v1;
        } mod;
    };

} // namespace
