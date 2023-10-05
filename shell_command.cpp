#include "shell_command.h"

#include "utils/my_string.h"

int cmd_insert(sptr_sss skip_list, int argc, const std::vector<std::string> &args) {
    if (argc != 2) {
        std::cout << "parameter count error！" << std::endl;
        return -1;
    }

    using K = decltype(skip_list->key_flag);
    using V = decltype(skip_list->value_flag);

    K key = switch_type<K>(args[0]);
    V value = switch_type<V>(args[1]);

    int res = skip_list->insert_element(key, value);
    if (!res)
        return -1;
    else
        return 0;
}

int cmd_delete(sptr_sss skip_list, int argc, const std::vector<std::string> &args) {
    if (argc != 1) {
        std::cout << "parameter count error！" << std::endl;
        return -1;
    }

    using K = decltype(skip_list->key_flag);
    K key = switch_type<K>(args[0]);

    skip_list->delete_element(key);

    return 0;
}

int cmd_search(sptr_sss skip_list, int argc, const std::vector<std::string> &args) {
    if (argc != 1) {
        std::cout << "parameter count error！" << std::endl;
        return -1;
    }

    using K = decltype(skip_list->key_flag);
    K key = switch_type<K>(args[0]);

    auto value = skip_list->search_element(key);
    if (value)
        std::cout << "value: " << value.value() << std::endl;
    else
        std::cout << "Not found the key！" << std::endl;

    return 0;
}

int cmd_show(sptr_sss skip_list, int argc, const std::vector<std::string> &args) {
    skip_list->show_list();

    return 0;
}

int cmd_size(sptr_sss skip_list, int argc, const std::vector<std::string> &args) {
    std::cout << skip_list->size() << std::endl;

    return 0;
}

int cmd_loadfile(sptr_sss skip_list, int argc, const std::vector<std::string> &args) {
    if (argc != 1) {
        std::cout << "parameter count error！" << std::endl;
        return -1;
    }

    skip_list->load_file(args[0]);

    return 0;
}

int cmd_dumpfile(sptr_sss skip_list, int argc, const std::vector<std::string> &args) {
    if (argc != 1) {
        std::cout << "parameter count error！" << std::endl;
        return -1;
    }

    skip_list->dump_file(args[0]);

    return 0;
}