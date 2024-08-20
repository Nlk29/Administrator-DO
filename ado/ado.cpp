#include <iostream>
#include <string>
#include <conio.h>
#include <Windows.h>

void ExecuteAsAdmin(std::string input)
{
    // Prozess erstellen
    STARTUPINFOA si;
    PROCESS_INFORMATION pi;

    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    if (!CreateProcessA(NULL,
        const_cast<char*>(input.c_str()),
        NULL,
        NULL,
        FALSE,
        0,
        NULL,
        NULL,
        &si,
        &pi))
    {
        // Prozess erstellen fehlgeschlagen, Fehler behandeln.
        LPSTR messageBuffer = nullptr;
        size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

        std::cout << "Error: Could not create process: " << std::string(messageBuffer, size) << std::endl;

        LocalFree(messageBuffer);
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);
        exit(0);
    }

    // Prozess solange am Leben halten bis er geschlossen wird
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Aufräumen
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

int main(int argc, char* argv[])
{
    std::string input;
    
    if (argc < 2)
    {
        // Input abfragen
        std::cout << "Please enter command: ";
        std::getline(std::cin, input);
    }

    if (argv[1] == "-h" || argv[1] == "-H" || argv[1] == "/h" || argv[1] == "/H")
    {
        // Hilfe anzeigen
        exit(0);
    }

    for (int argi = 1; argi < argc; argi++)
    {
        input = input + argv[argi] + " ";
    }
   
    ExecuteAsAdmin(input);
    std::cout << "Press any key to exit...";
    char dummy = _getch();

    return 0;
}