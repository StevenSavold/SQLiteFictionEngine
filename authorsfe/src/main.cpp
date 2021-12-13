#include <cstdlib>
#include <libsfe.h>
#include <iostream>
#include <argh.h>

void print_usage(const char* prog_name);
int Dump(argh::parser cmdl);
int Create(argh::parser cmdl);

int main(int argc, char** argv)
{
    argh::parser cmdl({"-f"}); // pre-register '-f' to be able to use '-Cf' later
    cmdl.parse(argc, argv, argh::parser::SINGLE_DASH_IS_MULTIFLAG);

    if (cmdl[{"--help"}])
    {
        print_usage(cmdl[0].c_str());
        return EXIT_SUCCESS;
    }

    if ((cmdl[{"-D"}] && cmdl["-C"]) || (!cmdl[{"-D"}] && !cmdl["-C"]))
    {
        print_usage(cmdl[0].c_str());
        return EXIT_FAILURE;
    }

    if (cmdl[{"-D"}])
    {
        return Dump(cmdl);
    }

    if (cmdl[{"-C"}])
    {
        return Create(cmdl);
    }

    return EXIT_FAILURE; // Should be unreachable
}

void print_usage(const char* prog_name)
{
    std::cout << "USAGE:\n";
    std::cout << "\t" << prog_name << " mode [OPTIONS] mode_params...\n";
    std::cout << "\n";
    std::cout << "MODES:\n";
    std::cout << "\t-D sfe_file                         : Used to Dump the contents of the SFE file to stdout\n";
    std::cout << "\t-C sql_file lua_file [sqldiff_file] : Used to create a new SFE file from the sql dump file fromthe database, and the lua file with an optional sqldiff file for current saved progress\n";
    std::cout << "\n";
    std::cout << "OPTIONS:\n";
    std::cout << "\t-a : (when used with -D) Prints all the sections of the SFE file\n";
    std::cout << "\t-c : (when used with -D) Prints the code section of the SFE file\n";
    std::cout << "\t-g : (when used with -D) Prints the game section if the SFE file\n";
    std::cout << "\t-h : (when used with -D) Prints the header information of the SFE file\n";
    std::cout << "\t-p : (when used with -D) Prints the progress section of the SFE file\n";
    std::cout << "\n";
    std::cout << "\t-f filename : (when used with -C) allows the naming of the output file";
    std::cout << "\n";
    std::cout << "\t--help : Print this help message\n";
    std::cout << "\n";
    std::cout << "EXAMPLES:\n";
    std::cout << "\tTo dump all contents of the SFE to stdout:\n";
    std::cout << "\t\t" << prog_name << " -Da game.sfe\n";
    std::cout << "\tTo dump just the game and progress contents of the SFE to stdout:\n";
    std::cout << "\t\t" << prog_name << " -Dgp game.sfe\n";
    std::cout << "\tTo create a new SFE file with no saved progress:\n";
    std::cout << "\t\t" << prog_name << " -C game.sql game.lua\n";
    std::cout << "\tTo create a new SFE file with saved progress:\n";
    std::cout << "\t\t" << prog_name << " -C game.sql game.lua save.sql\n";
    std::cout << "\tTo create a new NAMED SFE file with no saved progress:\n";
    std::cout << "\t\t" << prog_name << " -Cf my_new_game.sfe game.sql game.lua\n";
}

int Dump(argh::parser cmdl)
{
    if (!cmdl(1))
    {
        print_usage(cmdl[0].c_str());
        return EXIT_FAILURE;
    }

    sfe::GameFile data = sfe::Load(cmdl[1].c_str());
    if (!data.isValid())
    {
        std::cerr << "Not a well formed sfe file!\n";
        return EXIT_FAILURE;
    }

    bool did_print_something = false;

    if (cmdl[{"-h"}] || cmdl[{"-a"}])
    {
        did_print_something = true;
        std::cout << "SFE HEADER:\n";
        std::cout << "              magic: 0x" << std::hex << static_cast<int>(data.Header.Magic) << "\n";
        std::cout << "            version: " << std::dec << (int)data.Header.MajorVersion << "." << (int)data.Header.MinorVersion << "\n";
        std::cout << "game section length: " << std::dec << data.Header.LengthOfGameSection << "\n";
        std::cout << "code section length: " << std::dec << data.Header.LengthOfCodeSection << "\n";
        std::cout << "diff section length: " << std::dec << data.Header.LengthOfProgressSection << "\n";
    }

    if (cmdl[{"-g"}] || cmdl[{"-a"}])
    {
        did_print_something = true;
        std::cout << "SFE GAME:\n";
        std::cout << data.GameData << "\n";
    }

    if (cmdl[{"-c"}] || cmdl[{"-a"}])
    {
        did_print_something = true;
        std::cout << "SFE CODE:\n";
        std::cout << data.CodeData << "\n";
    }

    if (cmdl[{"-p"}] || cmdl[{"-a"}])
    {
        did_print_something = true;
        std::cout << "SFE PROGRESS:\n";
        std::cout << data.ProgressData << "\n";
    }

    if (!did_print_something)
    {
        print_usage(cmdl[0].c_str());
        return EXIT_SUCCESS;
    }

    return EXIT_SUCCESS;
}

int Create(argh::parser cmdl)
{
    if (!cmdl(2))
    {
        print_usage(cmdl[0].c_str());
        return EXIT_FAILURE;
    }

    std::string savefile_name;
    cmdl("-f", "game.sfe") >> savefile_name;

    if (cmdl(3))
        return sfe::Save(savefile_name.c_str(), cmdl[1].c_str(), cmdl[2].c_str(), cmdl[3].c_str());
    return sfe::Save(savefile_name.c_str(), cmdl[1].c_str(), cmdl[2].c_str());
}