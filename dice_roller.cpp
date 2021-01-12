#include <iostream>
#include <random>
#include <chrono>
#include <string>
#include <vector>
#include <regex>
#include <ctype.h>

void print_help_text(char *argv[]);
void parse_options(int argc, char *argv[], int &num_dice, int &die_type, int &modifier);

int main (int argc, char *argv[]) {
    int num_dice = 0;
    int die_type = 0;
    int modifier = 0;

    if ( argc == 1 ) {
        // No arguments given
        // start interactive mode
        // TODO: create interactive mode
        print_help_text(argv);
        return 1;
    }
    else {
        // Parse command line options
        try {
            parse_options(argc, argv, num_dice, die_type, modifier);
            std::cout << "Number of dice given is: " << num_dice << std::endl;
            std::cout << "Die type given is: " << die_type << std::endl;
            std::cout << "Modifier given is: " << modifier << std::endl;
        }
        catch ( int e ) {
            if ( e == 1 ) {
                print_help_text(argv);

                return 1;
            }
        }
        catch (...) {
            std::cout << "Unknown exception" << std::endl;

            return 1;
        }

        // Calculate roll results
    }

    return 0;
}

void parse_options (int argc, char *argv[], int &num_dice, int &die_type, int &modifier) {
    // *num_dice = 0;
    // for ( int i = 1; i < argc; i++ ) { // Skip the program name
    //     if ( strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0 ) {
    //         throw 1;
    //     }
    //     else {
    //         for (int j = 0; j < strlen(argv[i]); j++) {
    //             std::cout << "Character is: [" << argv[i][j] << "]" << std::endl;
    //             if ( argv[i][j] == 'd' ) {
    //                 // Only rolling one die
    //                 *num_dice = 1;
    //                 *die_type = argv[i][j++] - '0'; // Convert to int
    //             }
    //         }
    //     }
    // }
    std::vector<std::string> all_args(argv, argv + argc);
    std::string::size_type sz;

    for ( int i = 0; i < all_args.size(); i++ ) {
    // for ( auto &a : all_args ) {
        if ( all_args[i] == "--help" || all_args[i] == "--h" ) {
            std::cerr << "# Got a help argument" << std::endl;
            throw 1;
        }
        if ( all_args[i] == argv[0] ) {
            std::cerr << "# Skipping the program name" << std::endl;
            continue; // Skip the program name
        }
        // else if ( a == "2d6" ) {
        else {
            std::cerr << "# Looping through an argument" << std::endl;
            for ( int j = 0; j < all_args[i].length() - 1; j++ ) {
                std::string a = all_args[i];
                std::cerr << "# Argument being parsed is: [" << a << "]" << std::endl;
                std::cout << "Character is: [" << a[j] << "]" << std::endl;
                if ( i < 2 ) {
                    std::cerr << "# First argument" << std::endl;
                    std::cerr << "# j is: " << j << std::endl;
                    // First argument
                    if ( isdigit(a[j]) && j < 1 ) {
                        // First character should be a number
                        std::cerr << "# First character is a digit" << std::endl;
                        num_dice = a[j] - '0';
                    }
                    else {
                        std::cerr << "# First character is not a digit" << std::endl;
                        throw 1;
                    }
                    if ( a[i] == 'd' ) {
                        std::cerr << "# Second or other character is a d" << std::endl;
                        die_type = a[j++] - '0';
                    }
                    else {
                        std::cerr << "# Second or other character is not a d" << std::endl;
                        throw 1;
                    }
                }
                else {
                    std::cerr << "# Not first argument. i is: " << i << std::endl;
                    if ( a[j] == '+' ) {
                        // Modifier is postive
                        modifier = a[j++];
                    }
                    else if ( a[j] == '-' ) {
                        // Modifier is negative
                        modifier = -a[j++];
                    }
                    else {
                        throw 1;
                    }
                }
            }
        }
        // else {
        //     std::cout << "Argument is: [" << a << "]" << std::endl;
        // }
    }

    return;
}

void print_help_text (char *argv[]) {
    std::string prog_name = argv[0];

    printf("Usage: %s: [--help|h] [die] [modifier]\n", prog_name.c_str());
    printf("Examples:\n");
    printf("\t%s 1d6\n", prog_name.c_str());
    printf("\t\tRolls one d6 and prints the result\n");
    printf("\t%s 2d6 +2\n", prog_name.c_str());
    printf("\t\tRolls two d6, then adds two and prints the result\n");
    printf("\t%s d20\n", prog_name.c_str());
    printf("\t\tRolls one d20 and prints the result\n");
    printf("\t%s\n", prog_name.c_str());
    printf("\t\tRuns the program in interactive mode\n");

    return;
}