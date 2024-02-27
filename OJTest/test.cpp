#include <bits/stdc++.h>
#include <stdio.h>
#include <sys/epoll.h>
#include <string>
#include <sstream>
#include <iostream>

int main()
{
    // fork();
    auto c_complier_func = [](const std::string& complier_path) {
        int output_code;
        std::ostringstream std_output_stream;
        std::string gcc_complier_command = "gcc " + complier_path + ".c -o2 -pedantic -Wall -Werror-implicit-function-declaration " + complier_path + " 2>&1 ";
        // std::cout << gcc_complier_command << std::endl;
        output_code = std::system(gcc_complier_command.c_str());
        if (output_code != 0) {
            std::cout << std_output_stream.str() << std::endl;
        } else {
            std::cout << "not error!\n";
            std::cout << std_output_stream.str() << std::endl;
        }
    };
    c_complier_func("/home/linux2023/complier/1-1-1-2092393");
    return 0;
}