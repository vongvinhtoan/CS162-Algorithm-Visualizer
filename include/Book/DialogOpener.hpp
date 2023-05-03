#pragma once

#include <windows.h>
#include <string>

class DialogOpener {
  public:
    DialogOpener(std::string filter = "Text Files (*.txt)\0*.txt\0", std::string ext = "txt");

  public:
    std::string get();

  private:
    std::string filter;
    std::string ext;
    HWND hwnd;
    TCHAR szFileName[MAX_PATH];
};