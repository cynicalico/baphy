set windows-shell := ["powershell.exe", "-c"]

cog:
    cog -c -r CMakeLists.txt
    cog -c -r include/baphy/baphy.hpp
