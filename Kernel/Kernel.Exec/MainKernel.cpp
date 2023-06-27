#include <iostream>
#include <stdexcept>

#include "KernelBuilder.hpp"
#include "ArgParser.hpp"

using Kernel::Builder::KernelBuilder;
using Kernel::Builder::SettingsManager;

int main(int argc, const char** argv)
{
    ArgParser parser(argc, argv);
    auto      settingsManager = std::make_unique<SettingsManager>(parser);
    auto      kernel = KernelBuilder(std::move(settingsManager)).makeKernel();

    kernel->start();

    while (true)
    {
        kernel->update();
    }
    return 0;
}
