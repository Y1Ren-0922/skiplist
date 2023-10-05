#pragma once

#include <iostream>
#include "type.h"

typedef int (shell_cmd)(sptr_sss skip_list, int argc, const std::vector<std::string> &args);

int cmd_insert(sptr_sss skip_list, int argc, const std::vector<std::string> &args);

int cmd_delete(sptr_sss skip_list, int argc, const std::vector<std::string> &args);

int cmd_search(sptr_sss skip_list, int argc, const std::vector<std::string> &args);

int cmd_show(sptr_sss skip_list, int argc, const std::vector<std::string> &args);

int cmd_size(sptr_sss skip_list, int argc, const std::vector<std::string> &args);

int cmd_loadfile(sptr_sss skip_list, int argc, const std::vector<std::string> &args);

int cmd_dumpfile(sptr_sss skip_list, int argc, const std::vector<std::string> &args);
