#include <iostream>

#include "Config.h"
#include "scanner.h"
#include "input.h"

utrigger_config::Config* g_config = nullptr;

void OnScannerEvent(Scanner* sender, ScanEvent* e_args)
{
    if (e_args->EventType == SCANEVENT_DETECT)
        MouseClick(0, 0);
    std::cout << "Scanner raised event" << "\n";
}

int main(int argc, char** argv)
{
    std::cout << "Hello World!\n";

    auto conf = utrigger_config::ReadCmdArgs(argc, argv, '=');

    g_config = new utrigger_config::Config(*conf);
    Scanner scanner(g_config);
    scanner.AddEventListener(OnScannerEvent);

    scanner.StartCapturingDesktopWindow(true);
}
