#include <iostream>
#include <fstream>
#include "httplib.h"

int main()
{
    using namespace httplib;


    Server svr;

    char data[1000] = {0};
    svr.Get("/stream", [&](const Request &req, Response &res) {
        res.set_content_provider(
                "application/pdf", // Content type
//                "text/plain",
                [&](size_t offset,  DataSink &sink) {
                    std::ifstream input_fd(u8"/home/kai/资料库/trex_book.pdf",std::ios::binary);
//                        std::vector<char> data;
                    input_fd.seekg(0,std::ios::end);
                    auto file_size = input_fd.tellg();
                    input_fd.seekg(offset,std::ios::beg);
                    if (offset < file_size) {
                        // prepare data...
                        std::cout << "go on:"<< offset<< " file_size:"<< file_size << std::endl;
                        if(file_size-input_fd.tellg()>=1000)
                        {
                            input_fd.read(data,1000);
                            sink.write(data,1000);
                        }
                        else
                        {
                            auto last_data_size = file_size - input_fd.tellg();
                            input_fd.read(data, last_data_size);
                            std::cout << file_size-input_fd.tellg() << std::endl;
                            sink.write(data,last_data_size);
                        }
                    } else {
                        sink.done(); // No more data
                    }
                    input_fd.close();

                    return true; // return 'false' if you want to cancel the process.
                });
    });

    svr.listen("localhost", 1234);
    std::cout << "Hello, World!" << std::endl;
    return 0;
}
